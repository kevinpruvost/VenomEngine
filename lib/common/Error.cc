///
/// Project: Bazel_Vulkan_Metal
/// File: Error.cc
/// Date: 8/18/2024
/// Description: 
/// Author: Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include "include/common/Error.h"

static char errorString[256] = {0};
namespace venom
{
void setErrorString(const char* str)
{
    memset(errorString, 0, sizeof(errorString));
    strcpy(errorString, str);
}

const char* getErrorString()
{
    return errorString;
}
}