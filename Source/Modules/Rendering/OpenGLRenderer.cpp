#include <Mist_Common\include\UtilityMacros.h>

#include <Mist_Profiler\Mist_Profiler.h>

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
Mesh CreateMesh(Renderer* renderer, MeshVertex* vertices, size_t vertexCount, MeshType meshType);
void ModifyMesh(unsigned int mesh, MeshVertex* vertices, size_t vertexCount, MeshType meshType);
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
	Transform* transforms;
	size_t transformCount;
};

struct MaterialData
{
	MistMaterialData;
};


// -Renderer-

constexpr uint32_t InstanceCount = 1001000;
constexpr uint32_t SingleBuffer = InstanceCount * sizeof(Transform);
constexpr uint8_t BufferCount = 3;
constexpr uint32_t BufferSize = SingleBuffer * BufferCount;

constexpr uint8_t MaxMeshCount = 255;
constexpr uint16_t MaxShaderCount = 10000;
constexpr uint8_t MaxCameraCount = 8;
constexpr uint8_t MaxSubmissionCount = 255;
constexpr uint16_t MaterialBufferSize = 1024 * 50;
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

	unsigned int instancingBuffer;
	void* instancingMemory;
	GLsync locks[3];

	unsigned int activeBuffer = 0;
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

MeshKey AddMesh(Renderer* renderer, MeshVertex* vertices, size_t vertexCount, MeshType meshType)
{
	MistAssert(renderer->nextMeshIndex < MaxMeshCount);
	renderer->meshes[renderer->nextMeshIndex] = CreateMesh(renderer, vertices, vertexCount, meshType);
	return renderer->nextMeshIndex++;
}

void ModifyMesh(Renderer* renderer, MeshKey key, MeshVertex* vertices, size_t vertexCount, MeshType meshType)
{
	MistAssert(renderer->nextMeshIndex > key);
	ModifyMesh(renderer->meshes[key].meshHandle, vertices, vertexCount, meshType);
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
	trans[0][3] = -position.x;
	trans[1][3] = -position.y;
	trans[2][3] = -position.z;
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
void Submit(Renderer* renderer, RenderKey renderKey, Transform* transforms, size_t transformCount)
{
	MistAssert(renderer->nextSubmissionIndex < MaxSubmissionCount);
	renderer->submissions[renderer->nextSubmissionIndex] = { renderKey, transforms, transformCount };
	renderer->nextSubmissionIndex++;
}

// -Mesh Implementation-
Mesh CreateMesh(Renderer* renderer, MeshVertex* vertices, size_t vertexCount, MeshType meshType)
{
	GLuint vao;
	glGenVertexArrays(1, &vao);
	// Add some error checking here if needed
	glBindVertexArray(vao);

	GLuint vbo;
	glGenBuffers(1, &vbo);
	// Error checking?

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	GLenum glType;
	switch (meshType)
	{
	case MeshType::Static:
		glType = GL_STATIC_DRAW;
		break;
	case MeshType::Dynamic:
		glType = GL_DYNAMIC_DRAW;
		break;
	case MeshType::Stream:
		glType = GL_STREAM_DRAW;
		break;
	}

	glBufferData(GL_ARRAY_BUFFER, sizeof(MeshVertex) * vertexCount, vertices, glType);

	// Position, needs to be set to 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (void*)(sizeof(float) * 6));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, renderer->instancingBuffer);

	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Transform), 0);
	glVertexAttribDivisor(3, 1);
	glEnableVertexAttribArray(3);

	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(Transform), (void*)(sizeof(Vec3)));
	glVertexAttribDivisor(4, 1);
	glEnableVertexAttribArray(4);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	Mesh mesh;
	mesh.meshHandle = vbo;
	mesh.meshPipeline = vao;
	mesh.vertexCount = vertexCount;

	return mesh;
}

void ModifyMesh(unsigned int mesh, MeshVertex* vertices, size_t vertexCount, MeshType meshType)
{
	MistAssert(meshType != MeshType::Static);

	GLenum glType;
	switch (meshType)
	{
	case MeshType::Dynamic:
		glType = GL_DYNAMIC_DRAW;
		break;
	case MeshType::Stream:
		glType = GL_STREAM_DRAW;
		break;
	}

	glBindBuffer(GL_ARRAY_BUFFER, mesh);
	glBufferData(GL_ARRAY_BUFFER, sizeof(MeshVertex) * vertexCount, vertices, glType);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
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

		glUniform4fv(4, 1, &defaultMaterial->color.x);
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
	MIST_BEGIN_PROFILE("Mist::Renderer", "RenderTick");
	Renderer* renderer = (Renderer*)system;

#if MIST_HINT_SORT_RENDER_SUBMISSIONS
	MIST_BEGIN_PROFILE("Mist::Renderer", "Sort");
	qsort(renderer->submissions, renderer->nextSubmissionIndex, sizeof(Submission), [](const void* left, const void* right)->int
	{
		return (int)(((int64_t)((Submission*)left)->key) - ((int64_t)((Submission*)right)->key));
	});
	MIST_END_PROFILE("Mist::Renderer", "Sort");
#endif // MIST_HINT_SORT_RENDER_SUBMISSIONS

	MIST_BEGIN_PROFILE("Mist::Renderer", "RenderingMeshes");
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	MaterialKey previousMaterialKey = (~(uint16_t)0);
	MeshKey previousMeshKey = (~(uint8_t)0);

	uint32_t currentOffset = 0;

	MIST_BEGIN_PROFILE("Mist::Renderer", "Wait");
	GLbitfield waitFlags = 0;
	GLuint64 waitDuration = 0;
	while (1 && renderer->locks[renderer->activeBuffer]) {
		GLenum waitRet = glClientWaitSync(renderer->locks[renderer->activeBuffer], waitFlags, waitDuration);
		if (waitRet == GL_ALREADY_SIGNALED || waitRet == GL_CONDITION_SATISFIED) {
			break;
		}

		if (waitRet == GL_WAIT_FAILED) {
			assert(!"Not sure what to do here. Probably raise an exception or something.");
			return SystemEventResult::Error;
		}

		// After the first time, need to start flushing, and wait for a looong time.
		waitFlags = GL_SYNC_FLUSH_COMMANDS_BIT;
		waitDuration = 1000000000;
	}
	MIST_END_PROFILE("Mist::Renderer", "Wait");

	for (size_t i = 0; i < renderer->nextSubmissionIndex; ++i)
	{
		CameraKey cameraKey = (CameraKey)(renderer->submissions[i].key >> 24);
		MaterialKey materialKey = (MaterialKey)(renderer->submissions[i].key >> 8);
		MeshKey meshKey = (MeshKey)(renderer->submissions[i].key);

		if (previousMaterialKey != materialKey)
		{
			DisableMaterial(renderer, &renderer->materialBuffer[previousMaterialKey]);
			EnableMaterial(renderer, &renderer->materialBuffer[materialKey]);

			previousMaterialKey = materialKey;
		}

		Mat4 vp = renderer->cameras[cameraKey].projection * renderer->cameras[cameraKey].view;
		glUniformMatrix4fv(0, 1, GL_FALSE, (float*)&vp);

		if (previousMeshKey != meshKey)
		{
			glBindVertexArray(renderer->meshes[meshKey].meshPipeline);
			previousMeshKey = meshKey;
		}

		uint32_t bufferOffset = SingleBuffer * renderer->activeBuffer + currentOffset;

		MIST_BEGIN_PROFILE("Mist::Renderer", "Buffer Data");
		memcpy((uint8_t*)renderer->instancingMemory + bufferOffset, renderer->submissions[i].transforms, sizeof(Transform) * renderer->submissions[i].transformCount);
		MIST_END_PROFILE("Mist::Renderer", "Buffer Data");

		MIST_BEGIN_PROFILE("Mist::Renderer", "Draw");
		glDrawArraysInstancedBaseInstance(GL_TRIANGLES, 0, renderer->meshes[meshKey].vertexCount, renderer->submissions[i].transformCount, InstanceCount * renderer->activeBuffer + currentOffset / sizeof(Transform));
		MIST_END_PROFILE("Mist::Renderer", "Draw");
		currentOffset += sizeof(Transform) * renderer->submissions[i].transformCount;
	}

	MIST_BEGIN_PROFILE("Mist::Renderer", "Sync");
	if (renderer->locks[renderer->activeBuffer])
	{
		glDeleteSync(renderer->locks[renderer->activeBuffer]);
	}
	MIST_BEGIN_PROFILE("Mist::Renderer", "Create Fence");
	renderer->locks[renderer->activeBuffer] = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
	MIST_END_PROFILE("Mist::Renderer", "Create Fence");

	renderer->activeBuffer = (renderer->activeBuffer + 1) % BufferCount;
	MIST_END_PROFILE("Mist::Renderer", "Sync");

	DisableMaterial(renderer, &renderer->materialBuffer[previousMaterialKey]);
	glBindVertexArray(0);

	MIST_END_PROFILE("Mist::Renderer", "RenderingMeshes");

	renderer->nextSubmissionIndex = 0;

	MIST_BEGIN_PROFILE("Mist::Renderer", "Clear Screen");
	DispatchEvent(renderer->eventSystem, SystemEventType::ClearScreen);
	MIST_END_PROFILE("Mist::Renderer", "Clear Screen");

	MIST_END_PROFILE("Mist::Renderer", "RenderTick");
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
	glEnable(GL_CULL_FACE);

	glEnable(GL_MULTISAMPLE);

	Renderer* renderer = (Renderer*)allocator.allocate(allocator.allocatorData, sizeof(Renderer));
	memset(renderer, 0, sizeof(Renderer));

	glGenBuffers(1, &renderer->instancingBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, renderer->instancingBuffer);

	const GLbitfield flags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
	glBufferStorage(GL_ARRAY_BUFFER, BufferSize, nullptr, flags);
	renderer->instancingMemory = glMapBufferRange(GL_ARRAY_BUFFER, 0, BufferSize, flags);

	return renderer;
}

void DestroyRenderer(SystemAllocator allocator, Renderer* renderer)
{
	for (size_t i = 0; i < renderer->nextMeshIndex; i++)
	{
		ReleaseMesh(renderer->meshes[i]);
	}

	for (size_t i = 0; i < renderer->nextShaderIndex; i++)
	{
		ReleaseShader(renderer->shaders[i]);
	}
	glDeleteBuffers(1, &renderer->instancingBuffer);

	allocator.deallocate(allocator.allocatorData, renderer);
}

MistNamespaceEnd

