///
/// Project: VenomEngine
/// @file GraphicsApplication.cc
/// @date Aug, 18 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/common/plugin/graphics/GraphicsApplication.h>
#include <venom/common/plugin/graphics/GraphicsPlugin.h>
#include <venom/common/plugin/graphics/GUI.h>
#include <venom/common/DLL.h>

#include <iostream>

#include <venom/common/Log.h>

#include "venom/common/plugin/graphics/RenderingPipeline.h"

namespace venom::common
{

int GraphicsApplication::_currentFrame = 0;
static GraphicsApplication * s_graphicsApplication = nullptr;

GraphicsApplication::GraphicsApplication()
    : _shaderResourceTable(GraphicsPlugin::Get()->CreateShaderResourceTable())
{
    venom_assert(s_graphicsApplication == nullptr, "Graphics Application already set static.");
    s_graphicsApplication = this;
}

GraphicsApplication* GraphicsApplication::Get()
{
    return s_graphicsApplication;
}

GraphicsApplication* GraphicsApplication::Create()
{
    GraphicsApplication * const app = GraphicsPlugin::Get()->CreateGraphicsApplication();
    if (app->_shaderResourceTable == nullptr)
    {
        vc::Log::Print("Shader Resource Table is not set in the Graphics Application.");
        delete app;
        return nullptr;
    }
    return app;
}

GraphicsApplication::~GraphicsApplication()
{
    vc::Log::Print("Destroying Graphics Application...");
    s_graphicsApplication = nullptr;
}

Error GraphicsApplication::Init()
{
    Error err = __Init();
    if (err != Error::Success) return err;

    // GUI Initialization is done after
    _gui = GraphicsPlugin::Get()->CreateGUI();
    _gui->SetGraphicsApplication(this);
    _gui->Initialize();
    if (_gui == nullptr)
    {
        vc::Log::Print("GUI is not set in the Graphics Application.");
        return vc::Error::Failure;
    }

    // Dummy Texture
    if (_dummyTexture == nullptr) {
        _dummyTexture.reset(new vc::Texture());
        _dummyTexture->__CreateDummyTexture();
    }

    // Loading Rendering Pipelines
    __LoadRenderingPipelines();

    // Post Init
    err = __PostInit();
    return err;
}

Error GraphicsApplication::Loop()
{
    vc::Context::Get()->PollEvents();
    return __Loop();
}

void GraphicsApplication::__LoadRenderingPipelines()
{
    // All default shader pipelines
    // Loading shadow able shaders
    {
        ShaderPipelineList shadowModelShaders;
        ShaderPipeline & gbuffer_shader = shadowModelShaders.emplace_back();
        gbuffer_shader.AddVertexBufferToLayout({
            {vc::ShaderVertexFormat::Vec3, 0, 0, 0}, // Position
            {vc::ShaderVertexFormat::Vec3, 1, 1, 0}, // Normal
            {vc::ShaderVertexFormat::Vec2, 2, 2, 0}, // UV
            {vc::ShaderVertexFormat::Vec3, 3, 3, 0}, // Tangent
            {vc::ShaderVertexFormat::Vec3, 4, 4, 0}, // Bitangent
        });
        gbuffer_shader.SetRenderingPipelineType(RenderingPipelineType::ShadowModel);
        gbuffer_shader.SetRenderingPipelineIndex(0);
        gbuffer_shader.LoadShaderFromFile("shader_gbuffer");

        ShaderPipeline & lighting_shader = shadowModelShaders.emplace_back();
        lighting_shader.AddVertexBufferToLayout({
            {vc::ShaderVertexFormat::Vec4, 0, 0, 0}, // Position
        });
        lighting_shader.SetRenderingPipelineType(RenderingPipelineType::ShadowModel);
        lighting_shader.SetRenderingPipelineIndex(1);
        lighting_shader.LoadShaderFromFile("shader_lighting");

        ShaderPipeline & lightCulling_shader = shadowModelShaders.emplace_back();
        lightCulling_shader.SetRenderingPipelineType(RenderingPipelineType::ShadowModel);
        lightCulling_shader.SetRenderingPipelineIndex(2);
        lightCulling_shader.LoadShaderFromFile("shader_lightculling");

        RenderingPipelineImpl::SetRenderingPipelineCache(shadowModelShaders, RenderingPipelineType::ShadowModel);
    }

    // Loading skybox shaders
    {
        ShaderPipelineList skyboxShaders;
        ShaderPipeline & shader = skyboxShaders.emplace_back();
        shader.AddVertexBufferToLayout(vc::ShaderVertexFormat::Vec3, 0, 0, 0);
        shader.SetDepthWrite(false);
        shader.SetRenderingPipelineType(RenderingPipelineType::Skybox);
        shader.SetRenderingPipelineIndex(0);
        shader.LoadShaderFromFile("skybox");
        RenderingPipelineImpl::SetRenderingPipelineCache(skyboxShaders, RenderingPipelineType::Skybox);
    }
}
}
