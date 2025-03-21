///
/// Project: VenomEngine
/// @file GraphicsApplication.h
/// @date Aug, 18 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/common/Error.h>

#include <venom/common/Context.h>
#include <venom/common/plugin/graphics/GraphicsPlugin.h>
#include <venom/common/plugin/graphics/ShaderResourceTable.h>
#include <venom/common/plugin/graphics/GraphicsSettings.h>
#include <venom/common/plugin/graphics/RenderPass.h>

/// @brief Contains the entirety of the code of the VenomEngine project.
namespace venom
{
/// @brief High-Level Frontend of the VenomEngine project.
/// It also contains all the code shared between every APIs/Dynamic Libraries.
/// But most importantly, that is where the abstraction is setup for the different APIs contained in this project.
/// Use the alias 'vc' to access the namespace.
namespace common
{
class VENOM_COMMON_API GraphicsApplication : public GraphicsPluginObject, public GraphicsSettings
{
protected:
    GraphicsApplication();
public:
    static GraphicsApplication * Get();
    static GraphicsApplication * Create(int argc, const char* argv[]);
    static inline int GetCurrentFrameInFlight() { return _currentFrame; }
    static inline int GetPreviousFrameInFlight() { return (_currentFrame + VENOM_MAX_FRAMES_IN_FLIGHT - 1) % VENOM_MAX_FRAMES_IN_FLIGHT; }
    static inline vcm::Vec2 GetCurrentExtent() { return _currentExtent; }
    ~GraphicsApplication() override;
    Error Init();
    virtual Error __Init() = 0;
    virtual vc::Error __PostInit() = 0;
    Error Loop();
    virtual Error __Loop() = 0;
    virtual bool ShouldClose() = 0;
    virtual void WaitForDraws() = 0;

    /**
     * @brief Called before the application closes.
     * @warning DO NOT FORGET MUST BE CALLED BEFORE CLOSING
     */
    virtual void PreClose() = 0;

    static inline RenderPass * GetSkyboxRenderPass() { return &Get()->_skyboxRenderPass; }
    static inline RenderPass * GetGraphicsRenderPass() { return &Get()->_graphicsRenderPass; }
    static inline RenderPass * GetGuiRenderPass() { return &Get()->_guiRenderPass; }
    static inline RenderPass * GetShadowMapRenderPass() { return &Get()->_shadowMapRenderPass; }

private:
    void __LoadRenderingPipelines();

protected:
    ShaderResourceTable * _shaderResourceTable;
    GUI * _gui;
    UPtr<vc::Texture> _dummyTexture;
    static int _currentFrame;
    static vcm::Vec2 _currentExtent;

    // Render Passes
    RenderPass _skyboxRenderPass;
    RenderPass _graphicsRenderPass;
    RenderPass _guiRenderPass;
    RenderPass _shadowMapRenderPass;
};
}
}
