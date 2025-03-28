///
/// Project: VenomEngineWorkspace
/// @file RenderingPipeline.cc
/// @date Nov, 10 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/common/plugin/graphics/RenderingPipeline.h>

#include <venom/common/plugin/graphics/GraphicsPlugin.h>

#include <venom/common/plugin/graphics/Model.h>

namespace venom
{
namespace common
{
static vc::Vector<ShaderPipelineList> __renderingPipelineCache(static_cast<int>(RenderingPipelineType::Count));

RenderingPipelineImpl::~RenderingPipelineImpl()
{
}

const ShaderPipelineList& RenderingPipelineImpl::GetRenderingPipelineCache(const RenderingPipelineType type)
{
    venom_assert(type != RenderingPipelineType::None || type != RenderingPipelineType::Count, "Rendering Pipeline Type is None or Count.");
    return __renderingPipelineCache[static_cast<int>(type)];
}

void RenderingPipelineImpl::SetRenderingPipelineCache(const ShaderPipelineList& list, const RenderingPipelineType type)
{
    venom_assert(type != RenderingPipelineType::None || type != RenderingPipelineType::Count, "Rendering Pipeline Type is None or Count.");
    __renderingPipelineCache[static_cast<int>(type)] = std::move(list);
}

RenderingPipeline::RenderingPipeline()
    : PluginObjectWrapper(GraphicsPlugin::Get()->CreateRenderingPipeline())
{
}

RenderingPipeline::RenderingPipeline(const RenderingPipelineType type)
    : RenderingPipeline()
{
    SetShaderType(type);
}

}
}
