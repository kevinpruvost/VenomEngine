///
/// Project: VenomEngine
/// @file Timer.cc
/// @date Aug, 31 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/common/Timer.h>

#include <venom/common/Ptr.h>
#include <venom/common/Log.h>

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

static UPtr<Timer> s_loopTimer(nullptr);
void Timer::ResetLoopTimer()
{
    s_loopTimer.reset(new Timer());
}

uint64_t Timer::GetLambdaMicroseconds()
{
    return s_loopTimer->GetMicroSeconds();
}

uint64_t Timer::GetLambdaMilliseconds()
{
    return s_loopTimer->GetMilliSeconds();
}

double Timer::GetLambdaSeconds()
{
    return (double)(s_loopTimer->GetMilliSeconds()) / 1000.0f;
}

void Timer::__PassFrame()
{
    venom_assert(s_loopTimer, "Timer::PassFrame() : Loop Timer is not set.");
    s_loopTimer->Reset();
}
}
}
