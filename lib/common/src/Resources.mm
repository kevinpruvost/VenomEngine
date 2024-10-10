///
/// Project: VenomEngine
/// @file Resources.cc
/// @date Aug, 22 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/common/Resources.h>

#include <Foundation/Foundation.h>

std::string getResourcePath() {
    // Get the path to the resource directory in the application bundle
    NSBundle *bundle = [NSBundle mainBundle];
    NSString *resourcePath = [bundle resourcePath];

    // Convert NSString to std::string
    return std::string([resourcePath UTF8String]);
}