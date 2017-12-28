#pragma once

#include <Mist_Common\include\UtilityMacros.h>

#include <Core/Systems/System.h>

#include <Core/Math/Vector.h>
#include <Core/Math/Transform.h>

#include <stdint.h>

MistNamespace

struct Renderer;

// -Meshes-
struct MeshVertex
{
	Vec3 m_Position;
};

using MeshKey = uint16_t;
// Mesh vertices should be passed in counterclockwise order, otherwise, they will be thought of as backfaces.
MeshKey AddMesh(Renderer* renderer, MeshVertex* vertices, size_t vertexCount);

// -Materials-
using MaterialKey = uint16_t;
MaterialKey AddMaterial(Renderer* renderer, void* materialData);

using ShaderKey = uint16_t;
ShaderKey AddShader(Renderer* renderer, const char* vertShader, const char* fragShader);

using CameraKey = uint16_t;
CameraKey AddCamera(Renderer* renderer);


// -RenderKey-
// Build a renderkey for the collection of material + mesh that you would like to use for your object. This will allow the renderer
// to sort all the rendering submissions by RenderKey thus minimizing state changes in the rendering architecture.
using RenderKey = uint64_t;

inline RenderKey BuildRenderKey(CameraKey cameraKey, MaterialKey materialKey, MeshKey meshKey)
{
	MistAssert((RenderKey)materialKey <= ((1 << 16) - 1));
	MistAssert((RenderKey)meshKey <= ((1 << 16) - 1));
	MistAssert((RenderKey)cameraKey <= ((1 << 16) - 1));

	return ((RenderKey)cameraKey << 32) | ((RenderKey)materialKey << 16) | ((RenderKey)meshKey & ((1 << 16) - 1));
}
// Submit a camera and a renderer submission to be rendered by that camera.
void Submit(Renderer* renderer, RenderKey renderKey, Transform* transforms, size_t transformCount);


// -Material Structs-
enum class MaterialType : uint8_t
{
	Invalid = 0,
	Default,
	Max
};

// MIST_MATERIAL_DATA must be at the top of the material structure
#define MistMaterialData \
	uint16_t size; \
	MaterialType type

// All material APIs are to be defined here. Their construction should set their material type and size on construction.
// This should only be handled at material construction time, this should not happen at runtime but in an earlier step in the pipeline and save directly to a file.
struct DefaultMaterial
{
	MistMaterialData;
	ShaderKey shader;
};

MistNamespaceEnd
