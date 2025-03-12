///
/// Project: VenomEngine
/// @file Log.mm
/// @date March, 12 2025
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///

#include <venom/common/Log.h>
#include <venom/common/String.h>
#include <venom/common/Resources.h>

#import <Foundation/Foundation.h>

namespace venom::common
{
#if defined(VENOM_PLATFORM_APPLE)
void Log::Print(const char* str)
{
    NSLog(@"%s", str);
}

void Log::Print(FILE* const stream, const char* str)
{
    NSLog(@"%s", str);
}
#endif
}
