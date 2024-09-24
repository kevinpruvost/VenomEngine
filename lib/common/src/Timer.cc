///
/// Project: VenomEngine
/// @file Timer.cc
/// @date Aug, 31 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/common/Timer.h>

namespace venom
{
namespace common
{
Timer::Timer()
    : __start(std::chrono::steady_clock::now())
{
}
Timer::~Timer()
{
}

void Timer::AddMilliseconds(uint64_t milliseconds)
{
    __start += std::chrono::milliseconds(milliseconds);
}

void Timer::AddMicroseconds(uint64_t microseconds)
{
    __start += std::chrono::microseconds(microseconds);
}

void Timer::AddNanoseconds(uint64_t nanoseconds)
{
    __start += std::chrono::nanoseconds(nanoseconds);
}

uint64_t Timer::GetNanoSeconds() const
{
    const std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    const uint64_t res = std::chrono::duration_cast<std::chrono::nanoseconds>(end - __start).count();
    return res;
}

uint64_t Timer::GetMicroSeconds() const
{
    const std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    const uint64_t res = std::chrono::duration_cast<std::chrono::microseconds>(end - __start).count();
    return res;
}

uint64_t Timer::GetMilliSeconds() const
{
    const std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    const uint64_t res = std::chrono::duration_cast<std::chrono::milliseconds>(end - __start).count();
    return res;
}

void Timer::Reset()
{
    __start = std::chrono::steady_clock::now();
}
}
}