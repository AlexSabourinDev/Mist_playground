
#include <Mist_Common\include\UtilityMacros.h>

#include <RenderingPlayground\RenderingPlayground.h>

#include <Rendering\Renderer.h>

#include <Core/Utility/SystemEventHandler.h>

MIST_NAMESPACE

struct Playground
{
	Renderer* m_Renderer;
	PipelineHandle m_Pipeline;
};

void ClearScreenPipeline(void* data)
{
	Playground* playground = (Playground*)data;

	ClearScreen(playground->m_Renderer, { 1.0f, 0.0f, 0.0f, 1.0f });
}

SystemData InitializeRenderingPlayground(SystemAllocator allocator)
{
	return allocator(sizeof(Playground));
}

void DeinitializeRenderingPlayground(SystemDeallocator deallocator, SystemData systemData)
{
	deallocator(systemData);
}

void ProvideRenderingPlaygroundDependencies(SystemData data, Renderer* renderer)
{
	Playground* playground = (Playground*)data;
	playground->m_Renderer = renderer;

	Pipeline playgroundPipeline;
	playgroundPipeline.m_PipelineRelease = [](SystemDeallocator, void*) {};
	playgroundPipeline.m_PipelineSetup = ClearScreenPipeline;
	playgroundPipeline.m_PipelineTeardown = [](void*) {};
	playgroundPipeline.m_PipelineData = playground;

	playground->m_Pipeline = AddPipeline(renderer, playgroundPipeline);
}

MIST_NAMESPACE_END
