#include <Mist_Common\include\UtilityMacros.h>

#include <Rendering\Renderer.h>

#include <Core/Systems/SystemEventHandler.h>

#include <Core/Math/Matrix.h>

#include <Systems/System.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>

#define GL3_PROTOTYPES 1
#include <GL/glew.h>

#include <GL/GL.h>

MistNamespace

// Renderer structures

// -Meshes-
struct Mesh
{
	unsigned int meshHandle;
	unsigned int meshPipeline;
	unsigned int vertexCount;
};
Mesh CreateMesh(MeshVertex* vertices, size_t vertexCount);
void ReleaseMesh(Mesh mesh);

// -Shaders-
struct Shader
{
	unsigned int shaderHandle;
	unsigned int vertHandle;
	unsigned int fragHandle;
};
Shader CreateShader(const char* vertShaderSource, const char* fragShaderSource);
void ReleaseShader(Shader shader);

struct Camera
{
	Mat4 view;
	Mat4 projection;
};

struct Submission
{
	RenderKey key;
	Mat4* transforms;
	size_t transformCount;
};

struct MaterialData
{
	MistMaterialData;
};


// -Renderer-

const uint16_t MaxMeshCount = 256;
const uint16_t MaxShaderCount = 256;
const uint16_t MaxCameraCount = 8;
const uint16_t MaxSubmissionCount = 10;
const uint16_t MaterialBufferSize = 1024;
struct Renderer
{
	SystemEventDispatch* eventSystem;

	Shader shaders[MaxShaderCount];
	ShaderKey nextShaderIndex;

	Mesh meshes[MaxMeshCount];
	MeshKey nextMeshIndex;

	Camera cameras[MaxCameraCount];
	CameraKey nextCameraIndex;

	// Support a variety of Materials
	uint8_t materialBuffer[MaterialBufferSize];
	uint16_t nextMaterialIndex;

	Submission submissions[MaxSubmissionCount];
	uint16_t nextSubmissionIndex;
};


// API Implementation

MaterialKey AddMaterial(Renderer* renderer, void* material)
{
	MaterialData* materialData = (MaterialData*)material;
	MistAssert(materialData->size > 0);
	MistAssert(renderer->nextMaterialIndex + materialData->size < MaterialBufferSize);
	MistAssert(materialData->type != MaterialType::Invalid);

	memcpy(&renderer->materialBuffer[renderer->nextMaterialIndex], materialData, materialData->size);

	MaterialKey previousMaterialKey = renderer->nextMaterialIndex;
	renderer->nextMaterialIndex += materialData->size;
	return previousMaterialKey;
}

MeshKey AddMesh(Renderer* renderer, MeshVertex* vertices, size_t vertexCount)
{
	MistAssert(renderer->nextMeshIndex < MaxMeshCount);
	renderer->meshes[renderer->nextMeshIndex] = CreateMesh(vertices, vertexCount);
	return renderer->nextMeshIndex++;
}

ShaderKey AddShader(Renderer* renderer, const char* vertShader, const char* fragShader)
{
	MistAssert(renderer->nextShaderIndex < MaxShaderCount);
	renderer->shaders[renderer->nextShaderIndex] = CreateShader(vertShader, fragShader);
	return renderer->nextShaderIndex++;
}

CameraKey AddCamera(Renderer* renderer)
{
	MistAssert(renderer->nextCameraIndex < MaxCameraCount);
	renderer->cameras[renderer->nextCameraIndex].projection = Identity();
	renderer->cameras[renderer->nextCameraIndex].view = Identity();

	return renderer->nextCameraIndex++;
}

void SetCameraTransform(Renderer* renderer, CameraKey key, Vec3 position, Quaternion rotation)
{
	Mat4 trans = ToMatrix(Conjugate(rotation));
	trans[0][3] = position.x;
	trans[1][3] = position.y;
	trans[2][3] = position.z;
	renderer->cameras[key].view = trans;
}

void SetCameraProjection(Renderer* renderer, CameraKey key, float fov, float nearPlane, float farPlane)
{
	float s = 1.0f / tan((fov / 2.0f));

	Mat4 proj = Identity();
	proj[0][0] = s;
	proj[1][1] = s;
	proj[2][2] = farPlane / (farPlane - nearPlane);
	proj[2][3] = (farPlane * nearPlane) / (nearPlane - farPlane);
	proj[3][2] = 1.0f;
	proj[3][3] = 0.0f;
	renderer->cameras[key].projection = proj;
}

// Submit a camera and a renderer submission to be rendered by that camera.
void Submit(Renderer* renderer, RenderKey renderKey, Mat4* transforms, size_t transformCount)
{
	MistAssert(renderer->nextSubmissionIndex < MaxSubmissionCount);
	renderer->submissions[renderer->nextSubmissionIndex] = { renderKey, transforms, transformCount };
	renderer->nextSubmissionIndex++;
}

// -Mesh Implementation-
Mesh CreateMesh(MeshVertex* vertices, size_t vertexCount)
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
	mesh.meshHandle = vbo;
	mesh.meshPipeline = vao;
	mesh.vertexCount = vertexCount;

	return mesh;
}

void ReleaseMesh(Mesh mesh)
{
	glDeleteBuffers(1, &mesh.meshHandle);
	glDeleteVertexArrays(1, &mesh.meshPipeline);
}

// -Shader implementations-
Shader CreateShader(const char* vertShaderSource, const char* fragShaderSource)
{
	// Vert Shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertShaderSource, 0);
	glCompileShader(vertexShader);

	GLint vertCompilationResults = GL_TRUE;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertCompilationResults);

	if (vertCompilationResults == GL_FALSE)
	{
		int messageLength;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &messageLength);
		char* log = (char *)malloc(messageLength);

		glGetShaderInfoLog(vertexShader, messageLength, &messageLength, log);
		printf("Shader Error: %s", log);

		free(log);

		glDeleteShader(vertexShader);
		MistAssert(false);

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
		int messageLength;
		glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &messageLength);
		char* log = (char *)malloc(messageLength);

		glGetShaderInfoLog(fragShader, messageLength, &messageLength, log);
		printf("Shader Error: %s", log);

		free(log);

		glDeleteShader(fragShader);
		MistAssert(false);

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
		int messageLength;
		glGetProgramiv(completeShader, GL_INFO_LOG_LENGTH, &messageLength);
		char* log = (char *)malloc(messageLength);

		glGetProgramInfoLog(completeShader, messageLength, &messageLength, log);
		printf("Shader Error: %s", log);

		free(log);

		glDeleteProgram(completeShader);
		glDeleteShader(fragShader);
		glDeleteShader(vertexShader);

		MistAssert(false);
		return {};
	}

	glDetachShader(completeShader, vertexShader);
	glDetachShader(completeShader, fragShader);

	Shader shader;
	shader.shaderHandle = completeShader;
	shader.vertHandle = vertexShader;
	shader.fragHandle = fragShader;

	return shader;
}

void ReleaseShader(Shader shader)
{
	glDeleteProgram(shader.shaderHandle);
	glDeleteShader(shader.vertHandle);
	glDeleteShader(shader.fragHandle);
}

void EnableMaterial(Renderer* renderer, void* materialData)
{
	MaterialData* data = (MaterialData*)materialData;
	switch (data->type)
	{
	case MaterialType::Default:
		DefaultMaterial* defaultMaterial = (DefaultMaterial*)materialData;
		glUseProgram(renderer->shaders[defaultMaterial->shader].shaderHandle);
		break;
	}
}

void DisableMaterial(Renderer* renderer, void* materialData)
{
	MaterialData* data = (MaterialData*)materialData;
	switch (data->type)
	{
	case MaterialType::Default:
		glUseProgram(0);
		break;
	}
}

SystemEventResult TickRenderer(void* system, SystemEventType, SystemData)
{
	Renderer* renderer = (Renderer*)system;

	qsort(renderer->submissions, renderer->nextSubmissionIndex, sizeof(Submission), [](const void* left, const void* right)->int
	{
		return (int)(((int64_t)((Submission*)left)->key) - ((int64_t)((Submission*)right)->key));
	});

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for (size_t i = 0; i < renderer->nextSubmissionIndex; ++i)
	{
		CameraKey cameraKey = (CameraKey)(renderer->submissions[i].key >> 32);
		MaterialKey materialKey = (MaterialKey)(renderer->submissions[i].key >> 16);
		MeshKey meshKey = (MeshKey)(renderer->submissions[i].key);

		// Find the camera
		Mat4 vp = renderer->cameras[cameraKey].projection * renderer->cameras[cameraKey].view;

		EnableMaterial(renderer, &renderer->materialBuffer[materialKey]);

		glUniformMatrix4fv(0, 1, GL_FALSE, (float*)&vp);
		glBindVertexArray(renderer->meshes[meshKey].meshPipeline);

		for (size_t transformIndex = 0; transformIndex < renderer->submissions[i].transformCount; transformIndex++)
		{
			glUniformMatrix4fv(1, 1, GL_FALSE, (float*)&renderer->submissions[i].transforms[transformIndex]);
			glDrawArrays(GL_TRIANGLES, 0, renderer->meshes[meshKey].vertexCount);
		}

		glBindVertexArray(0);
		DisableMaterial(renderer, &renderer->materialBuffer[materialKey]);
	}

	renderer->nextSubmissionIndex = 0;

	DispatchEvent(renderer->eventSystem, SystemEventType::ClearScreen);
	return SystemEventResult::Ok;
}

void ProvideEventDispatchToRenderer(Renderer* renderingSystem, SystemEventDispatch* eventSystem)
{
	RegisterHandler(eventSystem, SystemEventType::Tick, TickRenderer, renderingSystem);

	renderingSystem->eventSystem = eventSystem;
}

Renderer* CreateRenderer(SystemAllocator allocator)
{
#ifndef __APPLE__
	glewExperimental = GL_TRUE;
	glewInit();
#endif

	glEnable(GL_DEPTH_TEST);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	glEnable(GL_MULTISAMPLE);

	Renderer* renderer = (Renderer*)allocator(sizeof(Renderer));
	memset(renderer, 0, sizeof(Renderer));
	return renderer;
}

void DestroyRenderer(SystemDeallocator deallocator, Renderer* renderer)
{
	for (size_t i = 0; i < renderer->nextMeshIndex; i++)
	{
		ReleaseMesh(renderer->meshes[i]);
	}

	for (size_t i = 0; i < renderer->nextShaderIndex; i++)
	{
		ReleaseShader(renderer->shaders[i]);
	}
	deallocator(renderer);
}

MistNamespaceEnd

