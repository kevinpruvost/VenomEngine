///
/// Project: VenomEngine
/// @file Log.cc
/// @date Aug, 18 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///

#include <venom/common/Log.h>
#include <venom/common/String.h>

#include <fstream>
#include <filesystem>

#include <ctime>

static venom::common::String getTimeString()
{
    // Example of the very popular RFC 3339 format UTC time
    std::time_t time = std::time({});
    char timeString[std::size("yyyy_mm_ddThh_mm_ssZ")] = {0};
    std::strftime(std::data(timeString), std::size(timeString),
                  "%Y_%m_%dT%H_%MZ", std::localtime(&time));
    return timeString;
}

namespace venom::common
{

void Log::LogToFile(const char* str)
{
    // Check if log file is open
    static std::ofstream logFile;
    if (!logFile.is_open()) {
        // Name with datetime and create directory if doesn't exist
        std::filesystem::create_directory("logs");
        String date_time = getTimeString();
        String name = format("logs/Venom_%s.txt", date_time.c_str());
        logFile.open(name.c_str(), std::ios::out);
    }
    // Log time first [hh:mm:ss]
    std::time_t time = std::time({});
    static char timeString[std::size("hh:mm:ss")] = {0};
    std::strftime(std::data(timeString), std::size(timeString), "%H:%M:%S", std::localtime(&time));
    logFile << "[" << timeString << "] ";
    logFile << str << std::endl;
}

void Log::Print(const char* str)
{
    printf("%s\n", str);
    // weirdly, we actually need to do that manually
    fflush(stdout);
}

void Log::Print(FILE* const stream, const char* str)
{
    fprintf(stream, "%s\n", str);
}
}
