///
/// Project: VenomEngineWorkspace
/// @file MainScene.h
/// @date Nov, 08 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/common/plugin/graphics/GraphicsApplication.h>
#include <venom/common/VenomEngine.h>
#include <venom/common/Log.h>
#include <venom/common/plugin/graphics/Model.h>
#include <venom/common/plugin/graphics/RenderingPipeline.h>
#include <venom/common/plugin/graphics/Camera.h>
#include <venom/common/plugin/graphics/GUI.h>
#include <venom/common/plugin/graphics/Skybox.h>
#include <venom/common/plugin/graphics/RenderTarget.h>
#include <venom/common/Timer.h>
#include <venom/common/plugin/graphics/Light.h>
#include <venom/common/SceneSettings.h>

#include <thread>

void Scene(const vc::ScenePhase phase);
void SceneInput(vc::Context * context);
void SceneGUI();