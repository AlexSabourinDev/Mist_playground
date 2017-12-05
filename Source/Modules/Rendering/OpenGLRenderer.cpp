#include <Mist_Common\include\UtilityMacros.h>

#include <Rendering\Renderer.h>

#include <Utility\SystemEventHandler.h>

#include <Systems/System.h>

#include <cstdlib>
#include <cstring>
#include <cstdio>

#define GL3_PROTOTYPES 1
#include <GL/glew.h>

#include <GL/GL.h>

MIST_NAMESPACE

// Renderer structures

struct MaterialInstance
{
	Material m_Material;

	Mesh m_Meshes[10];
	size_t m_ActiveMeshes;
};

struct PipelineInstance
{
	Pipeline m_Pipeline;

	MaterialInstance m_Materials[4];
	size_t m_ActiveMaterials;
};

struct Renderer
{
	SystemEventDispatch* m_EventSystem;

	// 2 rendering passes is a good max for now
	PipelineInstance m_Pipelines[2];
	size_t m_ActivePipelines;
};


// API Implementation

void ClearScreen(Renderer*, Vec4 color)
{
	glClearColor(color.x, color.y, color.z, color.w);
	glClear(GL_COLOR_BUFFER_BIT);
}

void SetViewport(Renderer*, RectInt viewport)
{
	glViewport(viewport.x, viewport.y, viewport.width, viewport.height);
}

ShaderBuffer CreateShaderBuffer(Renderer* renderer, unsigned int bufferIndex)
{
	GLuint shaderBuffer;
	glGenBuffers(1, &shaderBuffer);

	ShaderBuffer buffer;
	buffer.m_BufferHandle = shaderBuffer;
	buffer.m_BufferIndex = bufferIndex;

	return buffer;
}

void ReleaseShaderBuffer(Renderer*, ShaderBuffer buffer)
{
	glDeleteBuffers(1, &buffer.m_BufferHandle);
}

void SetShaderData(Renderer*, ShaderBuffer buffer, void* bufferData, size_t bufferSize)
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffer.m_BufferHandle);
	glBufferData(GL_SHADER_STORAGE_BUFFER, bufferSize, bufferData, GL_DYNAMIC_COPY);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void BindShaderBuffer(Renderer*, ShaderBuffer buffer)
{
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, buffer.m_BufferHandle, buffer.m_BufferIndex);
}


Mesh CreateMesh(Renderer*, MeshVertex* vertices, size_t vertexCount, TransformView* transforms)
{
	GLuint vao;
	glGenVertexArrays(1, &vao);
	// Add some error checking here if needed
	glBindVertexArray(vao);

	GLuint vbo;
	glGenBuffers(1, &vbo);
	// Error checking?

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// TODO: Add Dynamic and stream setup
	glBufferData(GL_ARRAY_BUFFER, sizeof(MeshVertex) * vertexCount, vertices, GL_STATIC_DRAW);

	// Position, needs to be set to 0
	// Last param is actually offset, use was changed later on (Pretty gross)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	Mesh mesh;
	mesh.m_MeshHandle = vbo;
	mesh.m_MeshPipeline = vao;
	mesh.m_VertexCount = vertexCount;
	mesh.m_Transforms = transforms;

	return mesh;
}

PipelineHandle AddPipeline(Renderer* renderer, Pipeline pipeline)
{
	PipelineHandle pipelineHandle = renderer->m_ActivePipelines;

	// TODO: assure that we don't add too many pipelines
	renderer->m_Pipelines[pipelineHandle] = { pipeline };
	renderer->m_ActivePipelines++;

	return pipelineHandle;
}

MaterialHandle AddMaterial(Renderer* renderer, PipelineHandle pipelineHandle, Material material)
{
	MIST_ASSERT(pipelineHandle < renderer->m_ActivePipelines);
	PipelineInstance* pipeline = &renderer->m_Pipelines[pipelineHandle];

	// TODO: assure that we don't add too many materials
	MaterialHandle materialHandle = pipeline->m_ActiveMaterials;
	pipeline->m_Materials[materialHandle] = { material };
	pipeline->m_ActiveMaterials++;

	return materialHandle;
}

void AddMesh(Renderer* renderer, PipelineHandle pipelineHandle, MaterialHandle materialHandle, Mesh mesh)
{
	MIST_ASSERT(pipelineHandle < renderer->m_ActivePipelines);
	PipelineInstance* pipeline = &renderer->m_Pipelines[pipelineHandle];

	MIST_ASSERT(materialHandle < pipeline->m_ActiveMaterials);
	MaterialInstance* material = &pipeline->m_Materials[materialHandle];

	// TODO: assure that we don't add too many meshes
	material->m_Meshes[material->m_ActiveMeshes] = mesh;
	material->m_ActiveMeshes++;
}

Shader CreateShader(Renderer*, const char* vertShaderSource, const char* fragShaderSource)
{
	// Vert Shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertShaderSource, 0);
	glCompileShader(vertexShader);

	GLint vertCompilationResults = GL_TRUE;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertCompilationResults);

	if (vertCompilationResults == GL_FALSE)
	{
		MIST_ASSERT(false);
		glDeleteShader(vertexShader);

		// TODO: Log?
		return {};
	}

	// Frag Shader
	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, &fragShaderSource, 0);
	glCompileShader(fragShader);

	GLint fragCompilationResults = GL_TRUE;
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &fragCompilationResults);

	if (fragCompilationResults == GL_FALSE)
	{
		MIST_ASSERT(false);
		glDeleteShader(fragShader);

		// TODO: Log?
		return {};
	}

	// Link
	GLuint completeShader = glCreateProgram();

	glAttachShader(completeShader, vertexShader);
	glAttachShader(completeShader, fragShader);

	glLinkProgram(completeShader);

	GLint programLinkResults = GL_TRUE;
	glGetProgramiv(completeShader, GL_LINK_STATUS, &programLinkResults);

	if (programLinkResults == GL_FALSE)
	{
		/* The maxLength includes the NULL character */
		int messageLength;
		glGetProgramiv(completeShader, GL_INFO_LOG_LENGTH, &messageLength);
		char* log = (char *)malloc(messageLength);

		glGetProgramInfoLog(completeShader, messageLength, &messageLength, log);
		printf("Shader Error: %s", log);

		free(log);

		glDeleteProgram(completeShader);
		glDeleteShader(fragShader);
		glDeleteShader(vertexShader);

		MIST_ASSERT(false);
		return {};
	}

	glDetachShader(completeShader, vertexShader);
	glDetachShader(completeShader, fragShader);

	Shader shader;
	shader.m_ShaderHandle = completeShader;
	shader.m_VertHandle = vertexShader;
	shader.m_FragHandle = fragShader;

	return shader;
}

void ReleaseShader(Renderer*, Shader shader)
{
	glDeleteProgram(shader.m_ShaderHandle);
	glDeleteShader(shader.m_VertHandle);
	glDeleteShader(shader.m_FragHandle);
}

void SetActiveShader(Renderer*, Shader shader)
{
	glUseProgram(shader.m_ShaderHandle);
}

void ClearActiveShader(Renderer*)
{
	glUseProgram(0);
}

void Render(Renderer* renderer)
{
	for (size_t pipelineHandle = 0; pipelineHandle < renderer->m_ActivePipelines; pipelineHandle++)
	{
		PipelineInstance* pipelineInstance = &renderer->m_Pipelines[pipelineHandle];
		pipelineInstance->m_Pipeline.m_PipelineSetup(renderer, pipelineInstance->m_Pipeline.m_PipelineData);

		for (size_t materialHandle = 0; materialHandle < pipelineInstance->m_ActiveMaterials; materialHandle++)
		{
			MaterialInstance* materialInstance = &pipelineInstance->m_Materials[materialHandle];
			materialInstance->m_Material.m_MaterialSetup(renderer, materialInstance->m_Material.m_MaterialData);

			for (size_t meshHandle = 0; meshHandle < materialInstance->m_ActiveMeshes; meshHandle++)
			{
				Mesh mesh = materialInstance->m_Meshes[meshHandle];

				glBindVertexArray(mesh.m_MeshHandle);
				for (size_t transformHandle = 0; transformHandle < mesh.m_Transforms->m_ActiveTransforms; transformHandle++)
				{
					glUniform3fv(0, 1, (float*)&mesh.m_Transforms->m_Transforms[transformHandle]);

					glDrawArrays(GL_TRIANGLES, 0, mesh.m_VertexCount);
				}
				glBindVertexArray(0);
			}

			materialInstance->m_Material.m_MaterialSetup(renderer, materialInstance->m_Material.m_MaterialData);
		}

		pipelineInstance->m_Pipeline.m_PipelineTeardown(renderer, pipelineInstance->m_Pipeline.m_PipelineData);
	}
}

SystemEventResult TickRenderer(void* system, SystemEventType, SystemData)
{
	Renderer* renderingSystem = (Renderer*)system;
	Render(renderingSystem);

	DispatchEvent(renderingSystem->m_EventSystem, SystemEventType::ClearScreen);
	return SystemEventResult::Ok;
}

void ProvideEventDispatchToRenderer(Renderer* renderingSystem, SystemEventDispatch* eventSystem)
{
	RegisterHandler(eventSystem, SystemEventType::Tick, TickRenderer, renderingSystem);

	renderingSystem->m_EventSystem = eventSystem;
}

Renderer* CreateRenderer(SystemAllocator allocator)
{
#ifndef __APPLE__
	glewExperimental = GL_TRUE;
	glewInit();
#endif

	Renderer* renderer = (Renderer*)allocator(sizeof(Renderer));
	memset(renderer, 0, sizeof(Renderer));
	return renderer;
}

void DestroyRenderer(SystemDeallocator deallocator, Renderer* renderer)
{
	for (size_t pipelineHandle = 0; pipelineHandle < renderer->m_ActivePipelines; pipelineHandle++)
	{
		PipelineInstance* pipelineInstance = &renderer->m_Pipelines[pipelineHandle];
		pipelineInstance->m_Pipeline.m_PipelineRelease(renderer, deallocator, pipelineInstance->m_Pipeline.m_PipelineData);

		for (size_t materialHandle = 0; materialHandle < pipelineInstance->m_ActiveMaterials; materialHandle++)
		{
			MaterialInstance* materialInstance = &pipelineInstance->m_Materials[materialHandle];
			materialInstance->m_Material.m_MaterialRelease(renderer, deallocator, materialInstance->m_Material.m_MaterialData);

			for (size_t meshHandle = 0; meshHandle < materialInstance->m_ActiveMeshes; meshHandle++)
			{
				Mesh mesh = materialInstance->m_Meshes[meshHandle];
				glDeleteBuffers(1, &mesh.m_MeshHandle);
				glDeleteVertexArrays(1, &mesh.m_MeshPipeline);
			}
		}
	}

	deallocator(renderer);
}

MIST_NAMESPACE_END

