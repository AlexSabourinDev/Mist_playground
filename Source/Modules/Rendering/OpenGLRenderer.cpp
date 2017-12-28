#include <Mist_Common\include\UtilityMacros.h>

#include <Rendering\Renderer.h>

#include <Utility\SystemEventHandler.h>

#include <Systems/System.h>

#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cstdint>

#define GL3_PROTOTYPES 1
#include <GL/glew.h>

#include <GL/GL.h>

MIST_NAMESPACE

// Renderer structures

// -Meshes-
struct Mesh
{
	unsigned int m_MeshHandle;
	unsigned int m_MeshPipeline;
	unsigned int m_VertexCount;
};
Mesh CreateMesh(MeshVertex* vertices, size_t vertexCount);
void ReleaseMesh(Mesh mesh);

// -Shaders-
struct Shader
{
	unsigned int m_ShaderHandle;
	unsigned int m_VertHandle;
	unsigned int m_FragHandle;
};
Shader CreateShader(const char* vertShaderSource, const char* fragShaderSource);
void ReleaseShader(Shader shader);

// -Renderer-
struct Renderer
{
	SystemEventDispatch* m_EventSystem;

	Shader m_Shaders[256];
	Mesh m_Meshes[256];

	// Support a variety of Materials
	uint8_t m_MaterialBuffer[1024];
};


// API Implementation

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
	mesh.m_MeshHandle = vbo;
	mesh.m_MeshPipeline = vao;
	mesh.m_VertexCount = vertexCount;

	return mesh;
}

void ReleaseMesh(Mesh mesh)
{
	glDeleteBuffers(1, &mesh.m_MeshHandle);
	glDeleteVertexArrays(1, &mesh.m_MeshPipeline);
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
		MIST_ASSERT(false);

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
		MIST_ASSERT(false);

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

void ReleaseShader(Shader shader)
{
	glDeleteProgram(shader.m_ShaderHandle);
	glDeleteShader(shader.m_VertHandle);
	glDeleteShader(shader.m_FragHandle);
}

SystemEventResult TickRenderer(void* system, SystemEventType, SystemData)
{
	Renderer* renderingSystem = (Renderer*)system;

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
	deallocator(renderer);
}

MIST_NAMESPACE_END

