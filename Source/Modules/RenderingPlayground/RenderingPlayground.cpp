
#include <Mist_Common\include\UtilityMacros.h>

#include <RenderingPlayground\RenderingPlayground.h>

#include <Rendering\Renderer.h>

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

struct Playground
{
	Renderer* renderer;
	const char* dataPath;
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

	const float s = 0.25f;
	MeshVertex vertices[] = { { -s, -s, -s }, { s, -s, -s }, { s, s, -s }, { -s, -s, -s }, { s, s, -s }, {-s, s, -s},
								{ s, -s, -s },{ s, s, -s },{ s, s, s },{ s, -s, -s },{ s, s, s },{ s, -s, s },
								{ -s, -s, -s },{ -s, s, -s },{ -s, s, s },{ -s, -s, -s },{ -s, s, s },{ -s, -s, s },
								{ -s, -s, s },{ s, -s, s },{ s, s, s },{ -s, -s, s },{ s, s, s },{ -s, s, s },
								{ -s, s, -s },{ s, s, -s },{ s, s, s },{ -s, s, -s },{ s, s, s },{ -s, s, s },
								{ -s, -s, -s },{ s, -s, -s },{ s, -s, s },{ -s, -s, -s },{ s, -s, s },{ -s, -s, s } };
	MeshKey playgroundMesh = AddMesh(playground->renderer, vertices, 36);

	CameraKey camera = AddCamera(playground->renderer);
	SetCameraTransform(playground->renderer, camera, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 1.0f });
	SetCameraProjection(playground->renderer, camera, 3.1415f / 2.0f, 0.1f, 10.0f);

	return SystemEventResult::Ok;
}

SystemEventResult RenderPlayground(void* data, SystemEventType, SystemEventData)
{
	Playground* playground = (Playground*)data;
	static Mat4 t;
	t = ToMatrix(AxisAngle({ 0.707f, 0.707f, 0.0f }, (float)clock() / CLOCKS_PER_SEC));
	t[2][3] = 1.0f;
	Submit(playground->renderer, 0, &t, 1);

	return SystemEventResult::Ok;
}

SystemData InitializeRenderingPlayground(SystemAllocator allocator, const char* dataPath)
{
	Playground* playground = (Playground*)allocator(sizeof(Playground));
	playground->dataPath = dataPath;

	return playground;
}

void DeinitializeRenderingPlayground(SystemDeallocator deallocator, SystemData systemData)
{
	deallocator(systemData);
}

void ProvideRenderingPlaygroundDependencies(SystemData data, SystemEventDispatch* dispatch, Renderer* renderer)
{
	Playground* playground = (Playground*)data;
	playground->renderer = renderer;

	RegisterHandler(dispatch, SystemEventType::Startup, StartPlayground, playground);
	RegisterHandler(dispatch, SystemEventType::Tick, RenderPlayground, playground);
}

MistNamespaceEnd
