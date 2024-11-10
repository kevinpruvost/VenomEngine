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

GraphicsApplication::GraphicsApplication()
    : _shaderResourceTable(GraphicsPlugin::Get()->CreateShaderResourceTable())
{
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

    // All default shader pipelines
    // Loading basic model shaders
    {
        ShaderPipelineList basicModelShaders;
        ShaderPipeline & shader = basicModelShaders.emplace_back();
        shader.AddVertexBufferToLayout({
            {vc::ShaderVertexFormat::Vec3, 0, 0, 0},
            {vc::ShaderVertexFormat::Vec3, 1, 1, 0},
        //            {vc::ShaderVertexFormat::Vec4, 2, 2, 0},
            {vc::ShaderVertexFormat::Vec2, 3, 3, 0},
        });
        shader.LoadShaderFromFile("shader_mesh");
        RenderingPipelineImpl::SetRenderingPipelineCache(basicModelShaders, RenderingPipelineType::BasicModel);
    }

    // Loading shadow shaders
    {
        ShaderPipelineList shadowModelShaders;
        ShaderPipeline & shader = shadowModelShaders.emplace_back();
        shader.AddVertexBufferToLayout({
            {vc::ShaderVertexFormat::Vec3, 0, 0, 0},
            {vc::ShaderVertexFormat::Vec3, 1, 1, 0},
            {vc::ShaderVertexFormat::Vec2, 2, 2, 0},
        });
        shader.LoadShaderFromFile("shader_gbuffer");

        RenderingPipelineImpl::SetRenderingPipelineCache(shadowModelShaders, RenderingPipelineType::ShadowModel);
    }

    // Loading skybox shaders
    {
        ShaderPipelineList skyboxShaders;
        ShaderPipeline & shader = skyboxShaders.emplace_back();
        shader.AddVertexBufferToLayout(vc::ShaderVertexFormat::Vec3, 0, 0, 0);
        shader.SetDepthWrite(false);
        shader.LoadShaderFromFile("skybox");
        RenderingPipelineImpl::SetRenderingPipelineCache(skyboxShaders, RenderingPipelineType::Skybox);
    }

    // Post Init
    err = __PostInit();
    return err;
}

Error GraphicsApplication::Loop()
{
    vc::Context::Get()->PollEvents();
    return __Loop();
}
}
