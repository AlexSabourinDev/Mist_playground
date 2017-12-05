#pragma once

#include <Mist_Common\include\UtilityMacros.h>

#include <Core/Systems/System.h>

#include <Core/Math/Vector.h>
#include <Core/Math/Rect.h>

#include <cstdint>

MIST_NAMESPACE

struct Renderer;

struct TransformView
{
	Vec3* m_Transforms;
	size_t m_ActiveTransforms;
};

struct MeshVertex
{
	Vec3 m_Position;
};

struct Mesh
{
	// This probably only works for opengl but it works for this simple system
	unsigned int m_MeshPipeline;
	unsigned int m_MeshHandle;

	size_t m_VertexCount;

	// Allow others to modify our transform view
	TransformView* m_Transforms;
};

using PipelineHandle = unsigned int;
struct Pipeline
{
	void(*m_PipelineSetup)(Renderer* renderer, void* pipelineData);
	void(*m_PipelineTeardown)(Renderer* renderer, void* pipelineData);

	// Release the data associated to the pipeline
	void(*m_PipelineRelease)(Renderer* renderer, SystemDeallocator deallocator, void* pipelineData);
	void* m_PipelineData;
};

using MaterialHandle = unsigned int;
struct Material
{
	void(*m_MaterialSetup)(Renderer* renderer, void* materialData);
	void(*m_MaterialTeardown)(Renderer* renderer, void* materialData);

	// Release the data associated to the material
	void(*m_MaterialRelease)(Renderer* renderer, SystemDeallocator deallocator, void* materialData);
	void* m_MaterialData;
};

struct Shader
{
	// This probably only works well for OpenGL but it works for this simple system
	unsigned int m_ShaderHandle;
	unsigned int m_VertHandle;
	unsigned int m_FragHandle;
};

struct ShaderBuffer
{
	unsigned int m_BufferIndex;
	unsigned int m_BufferHandle;
};

// State Calls
void ClearScreen(Renderer* renderer, Vec4 color);
void SetViewport(Renderer* renderer, RectInt viewport);

ShaderBuffer CreateShaderBuffer(Renderer* renderer, unsigned int bufferIndex);
void ReleaseShaderBuffer(Renderer* renderer, ShaderBuffer buffer);
void SetShaderData(Renderer* renderer, ShaderBuffer buffer, void* bufferData, size_t bufferSize);
void BindShaderBuffer(Renderer* renderer, ShaderBuffer buffer);


// Meshes
Mesh CreateMesh(Renderer* renderer, MeshVertex* vertices, size_t vertexCount, TransformView* transforms);
void AddMesh(Renderer* renderer, PipelineHandle pipeline, MaterialHandle material, Mesh mesh);

// Pipelines
PipelineHandle AddPipeline(Renderer* renderer, Pipeline pipeline);

// Materials
MaterialHandle AddMaterial(Renderer* renderer, PipelineHandle pipeline, Material material);

// Shaders, Currently we only support shader source as strings. In the future, we'll probably make it take bytecode
Shader CreateShader(Renderer* renderer, const char* vertShaderSource, const char* fragShaderSource);
void ReleaseShader(Renderer* renderer, Shader shader);

void SetActiveShader(Renderer* renderer, Shader shader);
void ClearActiveShader(Renderer* renderer);

MIST_NAMESPACE_END
