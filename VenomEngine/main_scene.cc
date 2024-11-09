///
/// Project: VenomEngineWorkspace
/// @file main_scene.cc
/// @date Nov, 08 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include "MainScene.h"

void Scene()
{
    vc::Shader shader_skybox;
    shader_skybox.AddVertexBufferToLayout(vc::ShaderVertexFormat::Vec3, 0, 0, 0);
    shader_skybox.SetDepthWrite(false);
    shader_skybox.LoadShaderFromFile("skybox");
    vc::Entity cubemap = vc::CreateEntity("cubemap")
//        .emplace<vc::Skybox>("cubemap/aerodynamics_workshop.exr")
            .emplace<vc::Skybox>("cubemap/billiard_hall.exr")
        .emplace<vc::Shader>(shader_skybox);

    vc::Shader shader;
    shader.AddVertexBufferToLayout({
        {vc::ShaderVertexFormat::Vec3, 0, 0, 0},
        {vc::ShaderVertexFormat::Vec3, 1, 1, 0},
//            {vc::ShaderVertexFormat::Vec4, 2, 2, 0},
        {vc::ShaderVertexFormat::Vec2, 3, 3, 0},
    });
    shader.LoadShaderFromFile("shader_mesh");

    vc::Entity balls_hd = vc::CreateEntity("balls_hd")
        .emplace<vc::Transform3D>()
//            .emplace<vc::Model>("eye/eye.obj")
//            .emplace<vc::Model>("dead_space_gun/plasmagun_txt.fbx")
        .emplace<vc::Model>("dead_space_gun/gun.fbx")
        .emplace<vc::Shader>(shader)
        ;
    balls_hd.get_mut<vc::Transform3D>()->SetPosition(vcm::Vec3(4.0f, 4.0f, 0.0f));

    vc::Entity face_hd = vc::CreateEntity("face_hd")
        .emplace<vc::Transform3D>()
        .emplace<vc::Model>("face/model_mid.ply")
        .emplace<vc::Shader>(shader)
        ;

    vc::Entity camera = vc::CreateEntity("camera")
        .emplace<vc::Transform3D>()
        .emplace<vc::Camera>();
    camera.get_mut<vc::Camera>()->SetPosition(vcm::Vec3(-2.0f, -2.0f, 1.0f));
    camera.get_mut<vc::Camera>()->LookAt(balls_hd.get<vc::Transform3D>()->GetPosition());

    vc::GraphicsSettings::SetMultiSampling(vc::GraphicsSettings::MultiSamplingModeOption::MSAA, vc::GraphicsSettings::MultiSamplingCountOption::Samples4);
}