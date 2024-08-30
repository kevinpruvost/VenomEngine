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
#include <venom/common/plugin/graphics/Mesh.h>

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

int main(int argc, char** argv)
{
    int errorCode = EXIT_SUCCESS;

#if defined(_WIN32) && defined(_ANALYSIS)
    // Enable memory leak detection
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    _CrtSetReportHook(ReportHook);
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
#endif

    // Run the engine
    const vc::Error error = vc::VenomEngine::RunEngine(argv);

#if defined(_WIN32) && defined(_ANALYSIS)
    // Dump memory leaks to output (typically the Output window in Visual Studio)
    int memoryLeaks = _CrtDumpMemoryLeaks();
    vc::Log::Print("CRT_FINAL_REPORT: Memory leaks: [%s]", memoryLeaks == 1 ? "Yes" : "No");
#endif
    return static_cast<int>(error);
}
