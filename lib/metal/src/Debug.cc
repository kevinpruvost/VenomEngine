///
/// Project: VenomEngine
/// @file Debug.cc
/// @date Aug, 19 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/metal/Debug.h>

#include <vector>

namespace venom::metal
{

DebugApplication::DebugApplication()
{
}

DebugApplication::~DebugApplication()
{
}

vc::Error DebugApplication::InitDebug()
{
    return vc::Error::Success;
}

void DebugApplication::DestroyDebug()
{
}

vc::Error DebugApplication::_PostInstance_SetDebugParameters()
{
    return vc::Error::Success;
}

double DebugApplication::_GetTheoreticalFPS(double fps)
{
    // Average time waited on VSync per frame
    uint64_t timePassed = __fpsTimer.GetMicroSeconds();
    __fpsTimer.Reset();
    uint64_t averageProcessingTimePerFrame = (timePassed - __microsecondsWaitedOnVSync) / fps;
    //printf("Time passed: %lu, Micros; %lu, Average Processing Time Per Frame: %lu\n", timePassed, __microsecondsWaitedOnVSync, averageProcessingTimePerFrame);
    __microsecondsWaitedOnVSync = 0;
    return static_cast<double>(timePassed / averageProcessingTimePerFrame);
}

void DebugApplication::_UpdateTheoreticalFPS(uint32_t microsecondsWaitedOnVSync)
{
    __microsecondsWaitedOnVSync += microsecondsWaitedOnVSync;
}

vc::Error DebugApplication::__InitValidationLayers()
{
    return vc::Error::Success;
}

}
