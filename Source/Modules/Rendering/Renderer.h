#pragma once

#include <Mist_Common\include\UtilityMacros.h>

#include <Core/Systems/System.h>

#include <Core/Math/Vector.h>
#include <Core/Math/Quaternion.h>

#include <stdint.h>

MistNamespace

#define MIST_HINT_SORT_RENDER_SUBMISSIONS 1

struct Renderer;

// -Meshes-
struct MeshVertex
{
	Vec3 position;
	Vec3 normal;
	Vec2 uv;
};

enum class MeshType
{
	Static,
	Dynamic,
	Stream
};

using MeshKey = uint8_t;
// Mesh vertices should be passed in counterclockwise order, otherwise, they will be thought of as backfaces.
MeshKey AddMesh(Renderer* renderer, MeshVertex* vertices, size_t vertexCount, MeshType meshType = MeshType::Static);
void ModifyMesh(Renderer* renderer, MeshKey key, MeshVertex* vertices, size_t vertexCount, MeshType meshType = MeshType::Static);

// TODO: Add support for index buffers

// -Materials-
using MaterialKey = uint16_t;
MaterialKey AddMaterial(Renderer* renderer, void* materialData);

using ShaderKey = uint16_t;
ShaderKey AddShader(Renderer* renderer, const char* vertShader, const char* fragShader);

using CameraKey = uint8_t;
CameraKey AddCamera(Renderer* renderer);

void SetCameraTransform(Renderer* renderer, CameraKey key, Vec3 position, Quaternion rotation);
void SetCameraProjection(Renderer* renderer, CameraKey key, float fov, float nearPlane, float farPlane);


// -RenderKey-
// Build a renderkey for the collection of material + mesh that you would like to use for your object. This will allow the renderer
// to sort all the rendering submissions by RenderKey thus minimizing state changes in the rendering architecture.
using RenderKey = uint32_t;

inline RenderKey BuildRenderKey(CameraKey cameraKey, MaterialKey materialKey, MeshKey meshKey)
{
	MistAssert((RenderKey)materialKey <= ((1 << 16) - 1));
	MistAssert((RenderKey)meshKey <= ((1 << 8) - 1));
	MistAssert((RenderKey)cameraKey <= ((1 << 8) - 1));

	return ((RenderKey)cameraKey << 24) | ((RenderKey)materialKey << 8) | (RenderKey)meshKey;
}

inline MeshKey GetMesh(RenderKey renderKey) { return (MeshKey)renderKey; }
inline MaterialKey GetMaterial(RenderKey renderKey) { return (MaterialKey)(renderKey >> 8); }
inline CameraKey GetCamera(RenderKey renderKey) { return (CameraKey)(renderKey >> 24); }

struct Transform
{
	Vec3 position;
	Quaternion rotation;
};

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
	Vec4 color;
};

MistNamespaceEnd
