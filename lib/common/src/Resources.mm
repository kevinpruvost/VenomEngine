///
/// Project: VenomEngine
/// @file Resources.cc
/// @date Aug, 22 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/common/Resources.h>

#include <Foundation/Foundation.h>

vc::String getAppleResourcePath() {
    // Get the path to the resource directory in the application bundle
    NSBundle *bundle = [NSBundle mainBundle];
    NSString *resourcePath = [bundle resourcePath];

    // Convert NSString to vc::String
    return vc::String([resourcePath UTF8String]);
}

vc::String getAppleLogPath() {
    // Get the path to the Documents directory (writable)
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsPath = [paths firstObject];

    // Convert NSString to vc::String
    return vc::String([documentsPath UTF8String]);
}
