#pragma once

#include <Mist_common\include\UtilityMacros.h>

#include <Modules/Rendering/Renderer.h>

#include <Core/Math/Vector.h>

#include <cstdint>

MistNamespace

constexpr size_t VertexCount(size_t width, size_t height)
{
	return (width + 1) * (height + 1) * 6;
}

void GenPlane(Vec2Int dimensions, Vec2 size, MeshVertex* vertices, size_t bufferSize);

void GenPatch(const Vec3(&left)[4], const Vec3(&right)[4], Vec2Int dimensions, MeshVertex* vertices, size_t bufferSize);

void CalcNormals(MeshVertex* vertices, size_t vertexCount);

MistNamespaceEnd

