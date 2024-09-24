///
/// Project: VenomEngine
/// @file FpsCounter.h
/// @date Aug, 31 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/common/Timer.h>

namespace venom
{
namespace common
{
class VENOM_COMMON_API FpsCounter
{
public:
    /// @brief Counts Frames Per Seconds
    /// @param intervalFrequency Refresh rate of the FPS counter (4 = 4 times/second)
    /// the smaller the intervalFrequency, the more accurate the FPS counter
    FpsCounter(uint32_t intervalFrequency = 4);
    ~FpsCounter();

    void RegisterFrame();
    uint32_t GetFps() const;
private:
    Timer __timer;
    uint32_t __frameCount;
    uint32_t __fps;
    const uint32_t __intervalFrequency;
    const uint64_t __intervalDelay;
};
}
}