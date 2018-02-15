
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

constexpr int RevolvingMeshCount = 10000;
constexpr int MaterialGranularityCount = 5;
constexpr int TransformGranularityCount = 100;

struct Playground
{
	Renderer* renderer;
	const char* dataPath;

	RenderKey renderKey;
	RenderKey revolutionKey[RevolvingMeshCount / TransformGranularityCount];

	Mat4 renderTransforms[4];
	Mat4 revolutionTransform[RevolvingMeshCount];
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

	MaterialKey materialKeys[RevolvingMeshCount / MaterialGranularityCount];
	for (int i = 0; i < RevolvingMeshCount / MaterialGranularityCount; i++)
	{
		DefaultMaterial shapeMaterial;
		shapeMaterial.type = MaterialType::Default;
		shapeMaterial.size = sizeof(DefaultMaterial);
		shapeMaterial.shader = AddShader(playground->renderer, ToCStr(&vertShader), ToCStr(&fragShader));
		shapeMaterial.color = { 1.0f, 0.0f, 0.0f, 1.0f };

		materialKeys[i] = AddMaterial(playground->renderer, &shapeMaterial);
	}

	Clear(&vertShader);
	Clear(&fragShader);
	Clear(&fragDir);
	Clear(&vertDir);

	for (int i = 0; i < RevolvingMeshCount / TransformGranularityCount; i++)
	{
		constexpr int stepCount = 1000;
		constexpr float stepSize = 5.0f / stepCount;
		constexpr int vertCount = 6 * stepCount;

		MeshVertex revolution[vertCount];
		for (int i = 0; i < vertCount; i += 6)
		{
			float x = stepSize * (i / 6);
			float y = x * x;

			float nx = stepSize * (i / 6 + 1);
			float ny = nx * nx;

			revolution[i] = { { -x, y, 0.0f },{ 0.0f, 0.0f, 1.0f },{ -x / 50.0f, y / 50.0f } };
			revolution[i + 1] = { { x, y, 0.0f },{ 0.0f, 0.0f, 1.0f },{ x / 50.0f, y / 50.0f } };
			revolution[i + 2] = { { nx, ny, 0.0f },{ 0.0f, 0.0f, 1.0f },{ nx / 50.0f, ny / 50.0f } };
			revolution[i + 3] = { { -x, y, 0.0f },{ 0.0f, 0.0f, 1.0f },{ -x / 50.0f, y / 50.0f } };
			revolution[i + 4] = { { nx, ny, 0.0f },{ 0.0f, 0.0f, 1.0f },{ nx / 50.0f, ny / 50.0f } };
			revolution[i + 5] = { { -nx, ny, 0.0f },{ 0.0f, 0.0f, 1.0f },{ -nx / 50.0f, ny / 50.0f } };
		}

		MeshKey revolutionKey = AddMesh(playground->renderer, revolution, vertCount);
		playground->revolutionKey[i] = BuildRenderKey(camera, materialKeys[i * TransformGranularityCount / MaterialGranularityCount], revolutionKey);
	}

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

	for (int i = 0; i < RevolvingMeshCount; i++)
	{
		float speedup = 1.0f + ((float)i / RevolvingMeshCount);
		playground->revolutionTransform[i] = ToMatrix(AxisAngle({ 0.0f, 1.0f, 0.0f }, (float)clock() / CLOCKS_PER_SEC * speedup));
		playground->revolutionTransform[i][2][3] = 20.0f;
		playground->revolutionTransform[i][1][3] = 20.0f;
	}

	bool randomizers[RevolvingMeshCount / TransformGranularityCount] = {};
	for (int i = 0; i < RevolvingMeshCount / TransformGranularityCount; i++)
	{
		int randomIndex = 0;
		do
		{
			randomIndex = rand() % (RevolvingMeshCount / TransformGranularityCount);
		} while (randomizers[randomIndex]);
		randomizers[randomIndex] = true;

		Submit(playground->renderer, playground->revolutionKey[randomIndex], &playground->revolutionTransform[randomIndex * TransformGranularityCount], TransformGranularityCount);
	}

	Submit(playground->renderer, playground->renderKey, playground->renderTransforms, 4);

	return SystemEventResult::Ok;
}

SystemData InitializeRenderingPlayground(SystemAllocator allocator, const char* dataPath)
{
	Playground* playground = (Playground*)allocator.allocate(allocator.allocatorData, sizeof(Playground));
	playground->dataPath = dataPath;
	
	playground->renderTransforms[0] = Identity();

	playground->renderTransforms[1] = ToMatrix(AxisAngle({ 0.0f, 0.0f, 1.0f }, 3.1415f / 2.0f));
	playground->renderTransforms[1][0][3] = 50.0f;
	playground->renderTransforms[1][1][3] = 50.0f;

	playground->renderTransforms[2] = ToMatrix(AxisAngle({ 0.0f, 0.0f, 1.0f }, -3.1415f / 2.0f));
	playground->renderTransforms[2][0][3] = -50.0f;
	playground->renderTransforms[2][1][3] = 50.0f;

	playground->renderTransforms[3] = ToMatrix(AxisAngle({ 1.0f, 0.0f, 0.0f }, -3.1415f / 2.0f));
	playground->renderTransforms[3][2][3] = 50.0f;
	playground->renderTransforms[3][1][3] = 50.0f;

	for (int i = 0; i < RevolvingMeshCount; i++)
	{
		playground->revolutionTransform[i] = Identity();
	}

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
