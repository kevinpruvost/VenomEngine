///
/// Project: VenomEngine
/// @file Timer.h
/// @date Aug, 30 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/common/Debug.h>
#include <venom/common/Export.h>
#include <chrono>

namespace venom
{
namespace common
{
class VenomEngine;

class VENOM_COMMON_API Timer
{
public:
    Timer();
    ~Timer();
    void AddMilliseconds(uint64_t milliseconds);
    void AddMicroseconds(uint64_t microseconds);
    void AddNanoseconds(uint64_t nanoseconds);
    uint64_t GetNanoSeconds() const;
    uint64_t GetMicroSeconds() const;
    uint64_t GetMilliSeconds() const;
    void Reset();

    static void ResetLoopTimer();
    static uint64_t GetLambdaMicroseconds();
    static uint64_t GetLambdaMilliseconds();
    static double GetLambdaSeconds();
private:
    std::chrono::time_point<std::chrono::steady_clock> __start;

    friend class VenomEngine;
    static void __PassFrame();
};
}
}
