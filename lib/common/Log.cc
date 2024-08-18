///
/// Project: Bazel_Vulkan_Metal
/// File: Log.cc
/// Date: 8/18/2024
/// Description: 
/// Author: Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///

#include "include/common/Log.h"
#include "include/common/String.h"

#include <fstream>
#include <filesystem>

#include <ctime>

static venom::String getTimeString()
{
    // Example of the very popular RFC 3339 format UTC time
    std::time_t time = std::time({});
    char timeString[std::size("yyyy_mm_ddThh_mm_ssZ")] = {0};
    std::strftime(std::data(timeString), std::size(timeString),
                  "%Y_%m_%dT%H_%MZ", std::localtime(&time));
    return timeString;
}

namespace venom
{

void Log::LogToFile(const char* str)
{
    // Check if log file is open
    static std::ofstream logFile;
    if (!logFile.is_open()) {
        // Name with datetime and create directory if doesn't exist
        std::filesystem::create_directory("logs");
        String date_time = getTimeString();
        String name = format("logs/Venom_{}.txt", date_time.c_str());
        logFile.open(name.c_str(), std::ios::out);
    }
    logFile << str;
    logFile.flush();
}

void Log::Print(const char* str)
{
    printf("%s", str);
}

void Log::Print(FILE* const stream, const char* str)
{
    fprintf(stream, str);
}
}
