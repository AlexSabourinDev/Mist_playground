#include <Modules\Rendering\MeshGen.h>

#include <Core/Math/Vector.h>

#include <Core/Math/Bezier.h>

MistNamespace

void GenPlane(Vec2Int dimensions, Vec2 size, MeshVertex* vertices, size_t bufferSize)
{
	MistAssert(bufferSize == VertexCount(dimensions[0], dimensions[1]));

	float uInc = 1.0f / dimensions[0];
	float vInc = 1.0f / dimensions[1];

	float xInc = 1.0f / dimensions[0] * size[0];
	float yInc = 1.0f / dimensions[1] * size[1];

	for (size_t height = 0; height < (size_t)dimensions[1]; height++)
	{
		for (size_t width = 0; width < (size_t)dimensions[0]; width++)
		{
			float u = uInc * width;
			float v = vInc * height;

			float x = xInc * width - size[0] / 2.0f;
			float y = yInc * height - size[1] / 2.0f;

			size_t square = (height * dimensions[1] + width) * 6;

			vertices[square + 0] = { { x, 0.0f, y },				{ 0.0f, 1.0f, 0.0f },	{u, v} };
			vertices[square + 1] = { { x + xInc, 0.0f, y },			{ 0.0f, 1.0f, 0.0f },	{ u + uInc, v } };
			vertices[square + 2] = { { x, 0.0f, y + yInc},			{ 0.0f, 1.0f, 0.0f },	{ u, v + vInc } };
			vertices[square + 3] = { { x + xInc, 0.0f, y },			{ 0.0f, 1.0f, 0.0f },	{ u + uInc, v } };
			vertices[square + 4] = { { x + xInc, 0.0f, y + yInc },	{ 0.0f, 1.0f, 0.0f },	{ u + uInc, v + vInc } };
			vertices[square + 5] = { { x, 0.0f, y + yInc },			{ 0.0f, 1.0f, 0.0f },	{ u, v + vInc } };
		}
	}
}

void GenPatch(const Vec3(&left)[4], const Vec3(&right)[4], Vec2Int dimensions, MeshVertex* vertices, size_t bufferSize)
{
	MistAssert(bufferSize == VertexCount(dimensions[0], dimensions[1]));

	float uInc = 1.0f / dimensions[0];
	float vInc = 1.0f / dimensions[1];

	for (size_t height = 0; height < (size_t)dimensions[1]; height++)
	{
		for (size_t width = 0; width < (size_t)dimensions[0]; width++)
		{
			float u = uInc * width;
			float v = vInc * height;

			Vec3 l1 = Evaluate(left[0], left[1], left[2], left[3], v);
			Vec3 l2 = Evaluate(left[0], left[1], left[2], left[3], v + vInc);

			Vec3 r1 = Evaluate(right[0], right[1], right[2], right[3], v);
			Vec3 r2 = Evaluate(right[0], right[1], right[2], right[3], v + vInc);

			size_t square = (height * dimensions[1] + width) * 6;

			vertices[square + 0] = { { Lerp(l1, r1, u) },		 { 0.0f, 1.0f, 0.0f }, { u, v } };
			vertices[square + 1] = { { Lerp(l1, r1, u + uInc) }, { 0.0f, 1.0f, 0.0f }, { u + uInc, v } };
			vertices[square + 2] = { { Lerp(l2, r2, u) },		 { 0.0f, 1.0f, 0.0f }, { u, v + vInc } };
			vertices[square + 3] = { { Lerp(l1, r1, u + uInc) }, { 0.0f, 1.0f, 0.0f }, { u + uInc, v } };
			vertices[square + 4] = { { Lerp(l2, r2, u + uInc) }, { 0.0f, 1.0f, 0.0f }, { u + uInc, v + vInc } };
			vertices[square + 5] = { { Lerp(l2, r2, u) },		 { 0.0f, 1.0f, 0.0f }, { u, v + vInc } };
		}
	}
}

void CalcNormals(MeshVertex* vertices, size_t vertexCount)
{
	for (size_t i = 0; i < vertexCount / 3; i++)
	{
		Vec3 cross = Cross(vertices[i * 3 + 2].position - vertices[i * 3].position, vertices[i * 3 + 1].position - vertices[i * 3].position);
		cross = Normalize(cross);

		vertices[i * 3].normal = cross;
		vertices[i * 3 + 1].normal = cross;
		vertices[i * 3 + 2].normal = cross;
	}
}

MistNamespaceEnd

