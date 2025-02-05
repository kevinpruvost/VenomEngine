///
/// Project: VenomEngineWorkspace
/// @file Thread.h
/// @date Feb, 05 2025
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <thread>

namespace venom
{
namespace common
{
using Thread = std::thread;
template<typename T>
using Atomic = std::atomic<T>;
using Mutex = std::mutex;
using LockGuard = std::lock_guard<std::mutex>;
using UniqueLock = std::unique_lock<std::mutex>;
using ConditionVariable = std::condition_variable;
}
}