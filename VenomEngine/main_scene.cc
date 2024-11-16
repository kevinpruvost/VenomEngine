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
    vc::Entity cubemap = vc::CreateEntity("cubemap")
//        .emplace<vc::Skybox>("cubemap/aerodynamics_workshop.exr")
            .emplace<vc::Skybox>("cubemap/billiard_hall.exr")
        .emplace<vc::RenderingPipeline>(vc::RenderingPipelineType::Skybox);

    vc::Entity balls_hd = vc::CreateEntity("balls_hd")
        .emplace<vc::Transform3D>()
//            .emplace<vc::Model>("eye/eye.obj")
//            .emplace<vc::Model>("dead_space_gun/plasmagun_txt.fbx")
        .emplace<vc::Model>("dead_space_gun/gun.fbx")
        .emplace<vc::RenderingPipeline>(vc::RenderingPipelineType::ShadowModel);
        ;
    balls_hd.get_mut<vc::Transform3D>()->SetPosition(vcm::Vec3(4.0f, 4.0f, 0.0f));

    vc::Entity face_hd = vc::CreateEntity("face_hd")
        .emplace<vc::Transform3D>()
        .emplace<vc::Model>("face/model_mid.ply")
        .emplace<vc::RenderingPipeline>(vc::RenderingPipelineType::ShadowModel);
        ;

    vc::Entity camera = vc::CreateEntity("camera")
        .emplace<vc::Transform3D>()
        .emplace<vc::Camera>();
    camera.get_mut<vc::Camera>()->SetPosition(vcm::Vec3(-2.0f, -2.0f, 1.0f));
    camera.get_mut<vc::Camera>()->LookAt(balls_hd.get<vc::Transform3D>()->GetPosition());

    vc::Entity light1 = vc::CreateEntity("light1")
        .emplace<vc::Light>();
    light1.get_mut<vc::Light>()->SetType(vc::LightType::Directional);
    light1.get_mut<vc::Light>()->SetPosition({6.0f, 10.0f, 2.0f});
    light1.get_mut<vc::Light>()->SetColor({0.0f, 1.0f, 0.0f});
    light1.get_mut<vc::Light>()->SetIntensity(10.0f);

    //vc::GraphicsSettings::SetMultiSampling(vc::GraphicsSettings::MultiSamplingModeOption::MSAA, vc::GraphicsSettings::MultiSamplingCountOption::Samples4);
}
