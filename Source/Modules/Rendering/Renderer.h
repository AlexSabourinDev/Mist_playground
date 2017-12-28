#pragma once

#include <Mist_Common\include\UtilityMacros.h>

#include <Core/Systems/System.h>

#include <Core/Math/Vector.h>

#include <cstdint>

MIST_NAMESPACE

struct Renderer;


// -Meshes-
struct MeshVertex
{
	Vec3 m_Position;
};

using MeshKey = uint16_t;
MeshKey AddMesh(Renderer* renderer, MeshVertex* vertices, size_t vertexCount);


// -Materials-
using MaterialKey = uint16_t;
MaterialKey AddMaterial(Renderer* renderer, void* materialData);


// -Material Structs-
enum class MaterialType : uint8_t
{
	Invalid = 0,
	Default,
	Max
};

// MIST_MATERIAL_DATA must be at the top of the material structure
#define MIST_MATERIAL_DATA \
	size_t m_Size; \
	MaterialType m_Type

struct DefaultMaterial
{
	MIST_MATERIAL_DATA;
};


// -RenderKey-
using RenderKey = uint32_t;

const int MAX_MATERIAL_BIT_COUNT = sizeof(MaterialKey) / 8;
const int MAX_MESH_BIT_COUNT = sizeof(MeshKey) / 8;

const RenderKey MAX_MATERIAL_COUNT = ((1 << MAX_MATERIAL_BIT_COUNT) - 1);
const RenderKey MAX_MESH_COUNT = ((1 << MAX_MESH_BIT_COUNT) - 1);

inline RenderKey BuildRenderKey(uint8_t materialKey, uint32_t meshKey)
{
	MIST_ASSERT((RenderKey)materialKey <= MAX_MATERIAL_COUNT);
	MIST_ASSERT((RenderKey)meshKey <= MAX_MESH_COUNT);

	return ((RenderKey)materialKey << (32 - MAX_MATERIAL_BIT_COUNT)) | ((RenderKey)meshKey & MAX_MESH_COUNT);
}


MIST_NAMESPACE_END
