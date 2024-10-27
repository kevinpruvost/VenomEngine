///
/// Project: VenomEngine
/// @file main.cc
/// @date Aug, 18 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/common/plugin/graphics/GraphicsApplication.h>
#include <venom/common/VenomEngine.h>
#include <venom/common/Log.h>
#include <venom/common/plugin/graphics/Model.h>

#if defined(_WIN32) && defined(_ANALYSIS)
#define _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

int ReportHook(int reportType, char *message, int * returnValue)
{
    if (reportType == _CRT_WARN) {
        vc::Log::Print("CRT_WARN: %s", message);
    } else if (reportType == _CRT_ERROR) {
        vc::Log::Print("CRT_ERROR: %s", message);
    } else if (reportType == _CRT_ASSERT) {
        vc::Log::Print("CRT_ASSERT: %s", message);
    }
    return TRUE;
}
#endif

#include <thread>

#include "venom/common/plugin/graphics/Camera.h"

int main(int argc, char** argv)
{
    int errorCode = EXIT_SUCCESS;

#if defined(_WIN32) && defined(_ANALYSIS)
    // Enable memory leak detection
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    _CrtSetReportHook(ReportHook);
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);

    // Mark the beginning of the memory check block
    _CrtMemState memStateStart, memStateEnd, memStateDiff;
    _CrtMemCheckpoint(&memStateStart);
#endif

    // Run the engine
    vc::VenomEngine::SetScene([]()
    {
        vc::Entity balls_hd = vc::CreateEntity("balls_hd")
            .emplace<vc::Transform3D>()
//            .emplace<vc::Model>("eye/eye.obj");
//            .emplace<vc::Model>("dead_space_gun/plasmagun_txt.fbx");
            .emplace<vc::Model>("dead_space_gun/gun.fbx");

        vc::Entity camera = vc::CreateEntity("camera")
            .emplace<vc::Transform3D>()
            .emplace<vc::Camera>();
        camera.get_mut<vc::Camera>()->SetPosition(vcm::Vec3(0.0f, 0.0f, 0.0f));
    });
    const vc::Error error = vc::VenomEngine::RunEngine(argc, argv);

#if defined(_WIN32) && defined(_ANALYSIS)
    // Mark the end of the memory check block
    _CrtMemCheckpoint(&memStateEnd);
    if (_CrtMemDifference(&memStateDiff, &memStateStart, &memStateEnd)) {
        // Report any memory leaks between the start and end checkpoints
        _CrtMemDumpStatistics(&memStateDiff);
        vc::Log::Print("CRT_FINAL_REPORT: Memory leaks detected.");
    } else {
        vc::Log::Print("CRT_FINAL_REPORT: No memory leaks detected.");
    }

    // Disable the leak check flag before program exits to avoid confusion with static variables
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
#endif
    return static_cast<int>(error);
}
