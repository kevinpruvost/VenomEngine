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

#ifdef VENOM_DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

int ReportHook( int reportType, char *message, int *returnValue )
{
    if (reportType == _CRT_WARN) {
        vc::Log::Print("CRT_WARN: %s\n", message);
    } else if (reportType == _CRT_ERROR) {
        vc::Log::Print("CRT_ERROR: %s\n", message);
    } else if (reportType == _CRT_ASSERT) {
        vc::Log::Print("CRT_ASSERT: %s\n", message);
    }
    return TRUE;
}

int main(int argc, char** argv)
{
    int errorCode = EXIT_SUCCESS;

    // Enable memory leak detection
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    _CrtSetReportHook(ReportHook);
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);

    // Run the engine
    const vc::Error error = vc::VenomEngine::RunEngine(argv);

    // Dump memory leaks to output (typically the Output window in Visual Studio)
    int memoryLeaks = _CrtDumpMemoryLeaks();
    vc::Log::Print("Memory leaks: %d\n", memoryLeaks);
    return errorCode;
}
