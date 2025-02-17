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
vcm::Vec2 GraphicsApplication::_currentExtent = {0, 0};
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
        gbuffer_shader.SetRenderingPipelineShaderType(RenderingPipelineShaderType::Graphics);
        gbuffer_shader.SetRenderingPipelineType(RenderingPipelineType::PBRModel);
        gbuffer_shader.SetRenderingPipelineIndex(0);
        gbuffer_shader.LoadShaderFromFile("pbr_mesh/lighting");

        RenderingPipelineImpl::SetRenderingPipelineCache(shadowModelShaders, RenderingPipelineType::PBRModel);
    }

    // Loading compute shader for Forward+ Light Culling
    {
        ShaderPipelineList lightCullingShaders;
        ShaderPipeline & shader = lightCullingShaders.emplace_back();
        shader.SetRenderingPipelineShaderType(RenderingPipelineShaderType::Compute);
        shader.SetRenderingPipelineType(RenderingPipelineType::ForwardPlusLightCulling);
        shader.LoadShaderFromFile("pbr_mesh/forwardplus_lightculling");

        RenderingPipelineImpl::SetRenderingPipelineCache(lightCullingShaders, RenderingPipelineType::ForwardPlusLightCulling);
    }

    // Loading compute shader for BRDF LUT
    {
        ShaderPipelineList brdfLUTShaders;
        ShaderPipeline & shader = brdfLUTShaders.emplace_back();
        shader.SetRenderingPipelineShaderType(RenderingPipelineShaderType::Compute);
        shader.SetRenderingPipelineType(RenderingPipelineType::BRDF_LUT);
        shader.LoadShaderFromFile("pbr_mesh/brdf_lut");

        RenderingPipelineImpl::SetRenderingPipelineCache(brdfLUTShaders, RenderingPipelineType::BRDF_LUT);
    }

    // Loading compute shader for Irradiance Map
    {
        ShaderPipelineList irradianceMapShaders;
        ShaderPipeline & shader = irradianceMapShaders.emplace_back();
        shader.SetRenderingPipelineShaderType(RenderingPipelineShaderType::Compute);
        shader.SetRenderingPipelineType(RenderingPipelineType::IrradianceMap);
        shader.LoadShaderFromFile("pbr_mesh/irradiance_map");

        RenderingPipelineImpl::SetRenderingPipelineCache(irradianceMapShaders, RenderingPipelineType::IrradianceMap);
    }

    // Loading compute shader for Radiance Map
    {
        ShaderPipelineList radianceMapShaders;
        ShaderPipeline & shader = radianceMapShaders.emplace_back();
        shader.SetRenderingPipelineShaderType(RenderingPipelineShaderType::Compute);
        shader.SetRenderingPipelineType(RenderingPipelineType::RadianceMap);
        shader.LoadShaderFromFile("pbr_mesh/radiance_map");

        RenderingPipelineImpl::SetRenderingPipelineCache(radianceMapShaders, RenderingPipelineType::RadianceMap);
    }

    // Loading compute shader for Blur Map
    {
        ShaderPipelineList blurMapShaders;
        ShaderPipeline & shader = blurMapShaders.emplace_back();
        shader.SetRenderingPipelineShaderType(RenderingPipelineShaderType::Compute);
        shader.SetRenderingPipelineType(RenderingPipelineType::BlurMap);
        shader.LoadShaderFromFile("pbr_mesh/blur_map");

        RenderingPipelineImpl::SetRenderingPipelineCache(blurMapShaders, RenderingPipelineType::BlurMap);
    }

    // Loading compute shader for Shadow Maps
    {
        ShaderPipelineList shadowMapShaders;
        ShaderPipeline & shader = shadowMapShaders.emplace_back();
        shader.SetRenderingPipelineShaderType(RenderingPipelineShaderType::Graphics);
        shader.SetRenderingPipelineType(RenderingPipelineType::CascadedShadowMapping);
        shader.SetRenderingPipelineIndex(0);
        shader.SetCustomMultiSamplingCount(1);
        shader.AddVertexBufferToLayout({
            {vc::ShaderVertexFormat::Vec3, 0, 0, 0}, // Position
            {vc::ShaderVertexFormat::Vec3, 1, 1, 0}, // Normal
        });
        shader.LoadShaderFromFile("pbr_mesh/shadow_map");

        RenderingPipelineImpl::SetRenderingPipelineCache(shadowMapShaders, RenderingPipelineType::CascadedShadowMapping);
    }

    // Loading skybox shaders
    {
        ShaderPipelineList skyboxShaders;
        ShaderPipeline & shader = skyboxShaders.emplace_back();
        shader.AddVertexBufferToLayout(vc::ShaderVertexFormat::Vec3, 0, 0, 0);
        shader.SetDepthWrite(false);
        shader.SetRenderingPipelineShaderType(RenderingPipelineShaderType::Graphics);
        shader.SetRenderingPipelineType(RenderingPipelineType::Skybox);
        shader.SetRenderingPipelineIndex(0);
        shader.LoadShaderFromFile("skybox");
        RenderingPipelineImpl::SetRenderingPipelineCache(skyboxShaders, RenderingPipelineType::Skybox);
    }

    // Loading Reflection shaders
    {
        ShaderPipelineList reflectionShaders;
        ShaderPipeline & shader = reflectionShaders.emplace_back();
        shader.AddVertexBufferToLayout({
            {vc::ShaderVertexFormat::Vec3, 0, 0, 0}, // Position
            {vc::ShaderVertexFormat::Vec3, 1, 1, 0}, // Normal
            {vc::ShaderVertexFormat::Vec2, 2, 2, 0}, // UV
            {vc::ShaderVertexFormat::Vec3, 3, 3, 0}, // Tangent
            {vc::ShaderVertexFormat::Vec3, 4, 4, 0}, // Bitangent
        });
        shader.SetRenderingPipelineShaderType(RenderingPipelineShaderType::Graphics);
        shader.SetRenderingPipelineType(RenderingPipelineType::Reflection);
        shader.SetRenderingPipelineIndex(0);
        shader.LoadShaderFromFile("pbr_mesh/reflection");

        RenderingPipelineImpl::SetRenderingPipelineCache(reflectionShaders, RenderingPipelineType::Reflection);
    }

    // Loading Additive Lighting with MS shaders
    {
        ShaderPipelineList additiveLightingShaders;
        ShaderPipeline & shader = additiveLightingShaders.emplace_back();
        shader.AddVertexBufferToLayout({
            {vc::ShaderVertexFormat::Vec3, 0, 0, 0}, // Position
        });
        shader.SetDepthWrite(false);
        shader.SetRenderingPipelineShaderType(RenderingPipelineShaderType::Graphics);
        shader.SetRenderingPipelineType(RenderingPipelineType::AdditiveLightingMS);
        shader.SetRenderingPipelineIndex(0);
        shader.LoadShaderFromFile("pbr_mesh/additive_lighting_ms");

        RenderingPipelineImpl::SetRenderingPipelineCache(additiveLightingShaders, RenderingPipelineType::AdditiveLightingMS);
    }

    // Loading Additive Lighting without MS shaders
    {
        ShaderPipelineList additiveLightingShaders;
        ShaderPipeline & shader = additiveLightingShaders.emplace_back();
        shader.AddVertexBufferToLayout({
            {vc::ShaderVertexFormat::Vec3, 0, 0, 0}, // Position
        });
        shader.SetDepthWrite(false);
        shader.SetRenderingPipelineShaderType(RenderingPipelineShaderType::Graphics);
        shader.SetRenderingPipelineType(RenderingPipelineType::AdditiveLighting);
        shader.SetRenderingPipelineIndex(0);
        shader.LoadShaderFromFile("pbr_mesh/additive_lighting");

        RenderingPipelineImpl::SetRenderingPipelineCache(additiveLightingShaders, RenderingPipelineType::AdditiveLighting);
    }
}
}
