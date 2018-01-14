
#include <Mist_Common\include\UtilityMacros.h>

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

struct Playground
{
	Renderer* renderer;
	const char* dataPath;

	RenderKey renderKey;
	RenderKey patchKey;

	Mat4 renderTransforms[4];
	Mat4 patchTransform;
};

SystemEventResult StartPlayground(void* data, SystemEventType, SystemEventData)
{
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

	Clear(&vertShader);
	Clear(&fragShader);
	Clear(&fragDir);
	Clear(&vertDir);

	MaterialKey materialHandle = AddMaterial(playground->renderer, &playgroundMaterial);

	MeshVertex floor[VertexCount(10, 10)];
	GenPlane({ 10, 10 }, { 100.0f, 100.0f }, floor, VertexCount(10, 10));
	MeshKey playgroundMesh = AddMesh(playground->renderer, floor, VertexCount(10, 10));

	MeshVertex patch[VertexCount(20, 20)];
	MeshKey patchKey = AddMesh(playground->renderer, patch, VertexCount(20, 20), MeshType::Dynamic);

	CameraKey camera = AddCamera(playground->renderer);
	SetCameraTransform(playground->renderer, camera, { 0.0f, 25.0f, -50.0f }, { 0.0f, 0.0f, 0.0f, 1.0f });
	SetCameraProjection(playground->renderer, camera, 3.1415f / 2.0f, 0.1f, 1000.0f);

	playground->renderKey = BuildRenderKey(camera, materialHandle, playgroundMesh);
	playground->patchKey = BuildRenderKey(camera, materialHandle, patchKey);

	return SystemEventResult::Ok;
}

SystemEventResult RenderPlayground(void* data, SystemEventType, SystemEventData)
{
	Playground* playground = (Playground*)data;

	playground->patchTransform[1][3] = 10.0f;
	playground->patchTransform[0][3] = -25.0f;

	MeshVertex patch[VertexCount(20, 20)];
	float height = 10.0f * sin((float)clock() / (float)CLOCKS_PER_SEC * 2.0f);

	GenPatch({ { -25.0f, 0.0f, 0.0f }, { -25.0f, -height, 10.0f }, { -25.0f, height, 40.0f }, { -25.0f, 0.0f, 50.0f } },
		{ { 25.0f, 0.0f, 0.0f },{ 25.0f, height, 10.0f },{ 25.0f, -height, 40.0f },{ 25.0f, 0.0f, 50.0f } }, {20, 20}, patch, VertexCount(20, 20));
	CalcNormals(patch, VertexCount(20, 20));
	ModifyMesh(playground->renderer, GetMesh(playground->patchKey), patch, VertexCount(20, 20), MeshType::Dynamic);

	Submit(playground->renderer, playground->renderKey, playground->renderTransforms, 4);
	Submit(playground->renderer, playground->patchKey, &playground->patchTransform, 1);

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

	playground->patchTransform = ToMatrix(AxisAngle({0.0f, 1.0f, 0.0f}, 3.1415f / 2.0f));

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
