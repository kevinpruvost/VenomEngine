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
    static GraphicsApplication * Create();
    static inline int GetCurrentFrameInFlight() { return _currentFrame; }
    static inline vcm::Vec2 GetCurrentExtent() { return _currentExtent; }
    ~GraphicsApplication() override;
    Error Init();
    virtual Error __Init() = 0;
    virtual vc::Error __PostInit() = 0;
    Error Loop();
    virtual Error __Loop() = 0;
    virtual bool ShouldClose() = 0;
    virtual void WaitForDraws() = 0;

private:
    void __LoadRenderingPipelines();

protected:
    ShaderResourceTable * _shaderResourceTable;
    GUI * _gui;
    UPtr<vc::Texture> _dummyTexture;
    static int _currentFrame;
    static vcm::Vec2 _currentExtent;
};
}
}