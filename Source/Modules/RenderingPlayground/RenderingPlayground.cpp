
#include <Mist_Common\include\UtilityMacros.h>

#include <Mist_Profiler\Mist_Profiler.h>

#include <RenderingPlayground\RenderingPlayground.h>

#include <Rendering\Renderer.h>
#include <Rendering/MeshGen.h>

#include <Core/Math/Vector.h>
#include <Core/Math/Serialization.h>

#include <Core/Systems/SystemEventHandler.h>
#include <Core/Utility/String.h>
#include <Core/Utility/FileIO.h>

#include <cJson/cJSON.h>

#include <cstdlib>
#include <ctime>
#include <random>

MistNamespace

cJSON* LoadPipeline(const char* executableDir)
{
	String filePath = Create(executableDir);
	Append(&filePath, "/Rendering/Pipelines/PlaygroundPipeline.json");
	String fileContents = ReadFile(ToCStr(&filePath));

	cJSON* configData = cJSON_Parse(fileContents.stringBuffer);

	Clear(&fileContents);
	Clear(&filePath);
	return configData;
}

void LoadMesh(const char* executableDir, const char* meshName, MeshVertex** vertices, size_t* dataSize)
{
	String filePath = Create(executableDir);
	Append(&filePath, "/Rendering/Meshes/");
	Append(&filePath, meshName);
	ReadFile(ToCStr(&filePath), (uint8_t**)vertices, dataSize);

	*dataSize = *dataSize / sizeof(MeshVertex);
	Clear(&filePath);
}

constexpr int SquareCount = 1000000;

struct Playground
{
	Renderer* renderer;
	const char* dataPath;

	RenderKey renderKey;
	RenderKey squareKey;

	Transform renderTransforms[4];
	Transform squareTransforms[SquareCount];
};

SystemEventResult StartPlayground(void* data, SystemEventType, SystemEventData)
{
	MIST_BEGIN_PROFILE("Mist::Rendering Playground", "Start");

	Playground* playground = (Playground*)data;

	// Read and create the shader
	String fragDir = Create(playground->dataPath);
	Append(&fragDir, "/Rendering/Shaders/Playground.frag");

	String vertDir = Create(playground->dataPath);
	Append(&vertDir, "/Rendering/Shaders/Playground.vert");

	String vertShader = ReadFile(ToCStr(&vertDir));
	String fragShader = ReadFile(ToCStr(&fragDir));

	DefaultMaterial playgroundMaterial;
	playgroundMaterial.type = MaterialType::Default;
	playgroundMaterial.size = sizeof(DefaultMaterial);
	playgroundMaterial.shader = AddShader(playground->renderer, ToCStr(&vertShader), ToCStr(&fragShader));
	playgroundMaterial.color = { 1.0f, 1.0f, 1.0f, 1.0f };

	CameraKey camera = AddCamera(playground->renderer);
	SetCameraTransform(playground->renderer, camera, { 0.0f, 25.0f, -50.0f }, { 0.0f, 0.0f, 0.0f, 1.0f });
	SetCameraProjection(playground->renderer, camera, 3.1415f / 2.0f, 0.1f, 1000.0f);

	MaterialKey materialHandle = AddMaterial(playground->renderer, &playgroundMaterial);

	Clear(&vertShader);
	Clear(&fragShader);
	Clear(&fragDir);
	Clear(&vertDir);

	constexpr float Width = 1.0f;
	MeshVertex square[6];
	square[0] = { { -Width, -Width, 0.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f } };
	square[1] = { {  Width, -Width, 0.0f }, { 0.0f, 0.0f, -1.0f }, { 1.0f, 0.0f } };
	square[2] = { {  Width,  Width, 0.0f }, { 0.0f, 0.0f, -1.0f }, { 1.0f, 1.0f } };
	square[3] = { { -Width, -Width, 0.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f } };
	square[4] = { {  Width,  Width, 0.0f }, { 0.0f, 0.0f, -1.0f }, { 1.0f, 1.0f } };
	square[5] = { { -Width,  Width, 0.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 1.0f } };

	MeshKey squareMesh = AddMesh(playground->renderer, square, 6);
	playground->squareKey = BuildRenderKey(camera, materialHandle, squareMesh);

	MeshVertex floor[VertexCount(2, 2)];
	GenPlane({ 2, 2 }, { 100.0f, 100.0f }, floor, VertexCount(2, 2));
	MeshKey playgroundMesh = AddMesh(playground->renderer, floor, VertexCount(2, 2));

	playground->renderKey = BuildRenderKey(camera, materialHandle, playgroundMesh);

	MIST_END_PROFILE("Mist::Rendering Playground", "Start");
	return SystemEventResult::Ok;
}

SystemEventResult RenderPlayground(void* data, SystemEventType, SystemEventData)
{
	Playground* playground = (Playground*)data;

	MIST_BEGIN_PROFILE("Mist::Rendering Playground", "TickRenderer");

	float step = 1.0f / SquareCount;
	float time = (float)clock() / CLOCKS_PER_SEC * step;

	MIST_BEGIN_PROFILE("Mist::Rendering Playground", "TransformLoop");
	for (int i = 0; i < SquareCount; i++)
	{
		playground->squareTransforms[i].rotation = IdentityQuat();
		playground->squareTransforms[i].position = { step * i, step * i + 10.0f + time, step * i + 10.0f };
	}
	MIST_END_PROFILE("Mist::Rendering Playground", "TransformLoop");

	Submit(playground->renderer, playground->renderKey, playground->renderTransforms, 4);
	Submit(playground->renderer, playground->squareKey, playground->squareTransforms, SquareCount);
	MIST_END_PROFILE("Mist::Rendering Playground", "TickRenderer");

	return SystemEventResult::Ok;
}

SystemData InitializeRenderingPlayground(SystemAllocator allocator, const char* dataPath)
{
	Playground* playground = (Playground*)allocator.allocate(allocator.allocatorData, sizeof(Playground));
	playground->dataPath = dataPath;
	
	playground->renderTransforms[0].rotation = IdentityQuat();
	playground->renderTransforms[0].position = {0.0f, 0.0f, 0.0f};

	playground->renderTransforms[1].rotation = AxisAngle({ 0.0f, 0.0f, 1.0f }, 3.1415f / 2.0f);
	playground->renderTransforms[1].position = { 50.0f, 50.0f, 0.0f };

	playground->renderTransforms[2].rotation = AxisAngle({ 0.0f, 0.0f, 1.0f }, -3.1415f / 2.0f);
	playground->renderTransforms[2].position = { -50.0f, 50.0f, 0.0f };

	playground->renderTransforms[3].rotation = AxisAngle({ 1.0f, 0.0f, 0.0f }, -3.1415f / 2.0f);
	playground->renderTransforms[3].position = { 0.0f, 50.0f, 50.0f };

	return playground;
}

void DeinitializeRenderingPlayground(SystemAllocator allocator, SystemData systemData)
{
	allocator.deallocate(allocator.allocatorData, systemData);
}

void ProvideRenderingPlaygroundDependencies(SystemData data, SystemEventDispatch* dispatch, Renderer* renderer)
{
	Playground* playground = (Playground*)data;
	playground->renderer = renderer;

	RegisterHandler(dispatch, SystemEventType::Startup, StartPlayground, playground);
	RegisterHandler(dispatch, SystemEventType::Tick, RenderPlayground, playground);
}

MistNamespaceEnd
