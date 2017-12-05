
#include <Mist_Common\include\UtilityMacros.h>

#include <RenderingPlayground\RenderingPlayground.h>

#include <Rendering\Renderer.h>

#include <Core/Math/Vector.h>
#include <Core/Math/Serialization.h>

#include <Core/Utility/SystemEventHandler.h>
#include <Core/Utility/String.h>
#include <Core/Utility/FileIO.h>

#include <cJson/cJSON.h>

#include <cstdlib>

MIST_NAMESPACE

cJSON* LoadPipeline(const char* executableDir)
{
	String filePath = Create(executableDir);
	Append(&filePath, "/Rendering/Pipelines/PlaygroundPipeline.json");
	String fileContents = ReadFile(ToCStr(&filePath));

	cJSON* configData = cJSON_Parse(fileContents.m_StringBuffer);

	Clear(&fileContents);
	Clear(&filePath);
	return configData;
}

struct Playground
{
	SystemAllocator m_Allocator;
	Renderer* m_Renderer;
	const char* m_DataPath;

	TransformView m_Transforms;
	Vec3 m_PlaygroundTransform;
};

struct PlaygroundPipelineData
{
	Vec4 m_PipelineColor;
};

struct PlaygroundMaterialData
{
	Shader m_Shader;
};

void PlaygroundPipeline(Renderer* renderer, void* data)
{
	PlaygroundPipelineData* pipeline = (PlaygroundPipelineData*)data;

	ClearScreen(renderer, pipeline->m_PipelineColor);
}

void SetupPlaygroundMaterial(Renderer* renderer, void* data)
{
	PlaygroundMaterialData* material = (PlaygroundMaterialData*)data;
	SetActiveShader(renderer, material->m_Shader);
}

void ReleasePlaygroundMaterial(Renderer* renderer, SystemDeallocator deallocator, void* data)
{
	PlaygroundMaterialData* materialData = (PlaygroundMaterialData*)data; 
	ReleaseShader(renderer, materialData->m_Shader); 
	deallocator(data);
}

SystemEventResult StartPlayground(void* data, SystemEventType, SystemEventData)
{
	Playground* playground = (Playground*)data;

	Pipeline playgroundPipeline;
	playgroundPipeline.m_PipelineRelease = [](Renderer*, SystemDeallocator deallocator, void* data) { deallocator(data); };
	playgroundPipeline.m_PipelineSetup = PlaygroundPipeline;
	playgroundPipeline.m_PipelineTeardown = [](Renderer*, void*) {};

	PlaygroundPipelineData* pipelineData = (PlaygroundPipelineData*)playground->m_Allocator(sizeof(PlaygroundPipelineData));
	playgroundPipeline.m_PipelineData = pipelineData;

	cJSON* pipeline = LoadPipeline(playground->m_DataPath);
	Deserialize(cJSON_GetObjectItem(pipeline, "Color"), &pipelineData->m_PipelineColor);

	PipelineHandle pipelineHandle = AddPipeline(playground->m_Renderer, playgroundPipeline);
	cJSON_Delete(pipeline);

	Material playgroundMaterial;
	playgroundMaterial.m_MaterialRelease = ReleasePlaygroundMaterial;
	playgroundMaterial.m_MaterialSetup = SetupPlaygroundMaterial;
	playgroundMaterial.m_MaterialTeardown = [](Renderer* renderer, void*) { ClearActiveShader(renderer);  };

	PlaygroundMaterialData* materialData = (PlaygroundMaterialData*)playground->m_Allocator(sizeof(PlaygroundMaterialData));
	playgroundMaterial.m_MaterialData = materialData;

	// Read and create the shader
	String fragDir = Create(playground->m_DataPath);
	Append(&fragDir, "/Rendering/Shaders/Playground.frag");
	
	String vertDir = Create(playground->m_DataPath);
	Append(&vertDir, "/Rendering/Shaders/Playground.vert");

	String vertShader = ReadFile(ToCStr(&vertDir));
	String fragShader = ReadFile(ToCStr(&fragDir));
	materialData->m_Shader = CreateShader(playground->m_Renderer, ToCStr(&vertShader), ToCStr(&fragShader));

	Clear(&vertShader);
	Clear(&fragShader);
	Clear(&fragDir);
	Clear(&vertDir);

	MaterialHandle materialHandle = AddMaterial(playground->m_Renderer, pipelineHandle, playgroundMaterial);

	playground->m_Transforms.m_Transforms = &playground->m_PlaygroundTransform;
	playground->m_Transforms.m_ActiveTransforms = 1;

	MeshVertex vertices[] = { { -0.5f, -0.5f, 0.0f }, { 0.5f, -0.5f, 0.0f }, { 0.0f, 0.5f, 0.0f } };
	Mesh playgroundMesh = CreateMesh(playground->m_Renderer, vertices, 3, &playground->m_Transforms);
	AddMesh(playground->m_Renderer, pipelineHandle, materialHandle, playgroundMesh);

	return SystemEventResult::Ok;
}

SystemData InitializeRenderingPlayground(SystemAllocator allocator, const char* dataPath)
{
	Playground* playground = (Playground*)allocator(sizeof(Playground));
	playground->m_Allocator = allocator;
	playground->m_DataPath = dataPath;

	return playground;
}

void DeinitializeRenderingPlayground(SystemDeallocator deallocator, SystemData systemData)
{
	deallocator(systemData);
}

void ProvideRenderingPlaygroundDependencies(SystemData data, SystemEventDispatch* dispatch, Renderer* renderer)
{
	Playground* playground = (Playground*)data;
	playground->m_Renderer = renderer;

	RegisterHandler(dispatch, SystemEventType::Startup, StartPlayground, playground);
}

MIST_NAMESPACE_END
