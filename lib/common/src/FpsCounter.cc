///
/// Project: VenomEngine
/// @file FpsCounter.cc
/// @date Aug, 31 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/common/FpsCounter.h>
#include <venom/common/Log.h>

namespace venom
{
namespace common
{
FpsCounter::FpsCounter(uint32_t intervalFrequency)
    : __timer()
    , __frameCount(0)
    , __fps(0)
    , __intervalDelay(1'000'000'000 / intervalFrequency)
    , __intervalFrequency(intervalFrequency)
{
}

FpsCounter::~FpsCounter()
{
}

void FpsCounter::RegisterFrame()
{
    ++__frameCount;
    const uint64_t delay = __timer.GetNanoSeconds();
    if (delay >= __intervalDelay)
    {
        __fps = __frameCount * __intervalFrequency;
        __frameCount = 0;
        __timer.AddNanoseconds(delay);
    }
}

uint32_t FpsCounter::GetFps() const
{
    return __fps;
}
}
}