///
/// Project: VenomEngineWorkspace
/// @file RenderingPipeline.h
/// @date Nov, 10 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once
#include <venom/common/plugin/graphics/ShaderPipeline.h>

#include <venom/common/ECS.h>

namespace venom
{
namespace common
{
typedef vc::Vector<ShaderPipeline> ShaderPipelineList;

class VENOM_COMMON_API RenderingPipelineImpl : public PluginObjectImpl, public GraphicsPluginObject
{
public:
    RenderingPipelineImpl(const RenderingPipelineType type = RenderingPipelineType::None) : __type(type) {}
    virtual ~RenderingPipelineImpl();

    inline void SetShaderType(const RenderingPipelineType type) { __type = type; }
    inline RenderingPipelineType GetShaderType() const { return __type; }
    inline const ShaderPipelineList & GetRenderingPipelineCache() const { return GetRenderingPipelineCache(__type); }

    static const ShaderPipelineList & GetRenderingPipelineCache(const RenderingPipelineType type);
    /**
     * @brief Sets the list of shader pipelines in cache
     * @warning Beware, this function MOVES the list inside the cache
     * @param list
     * @param type
     */
    static void SetRenderingPipelineCache(const ShaderPipelineList & list, const RenderingPipelineType type);
private:
    RenderingPipelineType __type;
};

/**
 * @brief Interface with shader pipelines and shader settings
 */
class VENOM_COMMON_API RenderingPipeline : public Component, public PluginObjectImplWrapper
{
public:
    RenderingPipeline();
    RenderingPipeline(const RenderingPipelineType type);
    ~RenderingPipeline() = default;

    void Update(Entity entity) override;
    void _GUI(const Entity entity) override;
    vc::String _GetComponentTitle() override;
    bool CanRemove(Entity entity) override;

    inline void SetShaderType(const RenderingPipelineType type) { _impl->As<RenderingPipelineImpl>()->SetShaderType(type); }
    inline RenderingPipelineType GetShaderType() const { return _impl->As<RenderingPipelineImpl>()->GetShaderType(); }
    inline const ShaderPipelineList & GetRenderingPipelineCache() const { return _impl->As<RenderingPipelineImpl>()->GetRenderingPipelineCache(); }
    inline static const ShaderPipelineList & GetRenderingPipelineCache(const RenderingPipelineType type) { return RenderingPipelineImpl::GetRenderingPipelineCache(type); }
};

}
}
