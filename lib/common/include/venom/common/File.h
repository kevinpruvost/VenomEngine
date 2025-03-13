///
/// Project: VenomEngineWorkspace
/// @file File.h
/// @date Mar, 13 2025
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once
#include <fstream>

namespace venom
{
namespace common
{
using FileStream = std::fstream;
using OFileStream = std::ofstream;
using IFileStream = std::ifstream;
} // namespace common
}