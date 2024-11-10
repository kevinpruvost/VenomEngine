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
#include <venom/common/Timer.h>

#include <thread>

void Scene();
void SceneInput(vc::Context * context);
void SceneGUI();