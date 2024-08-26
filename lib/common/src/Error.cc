///
/// Project: VenomEngine
/// @file Error.cc
/// @date Aug, 18 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/common/Error.h>

static char errorString[256] = {0};
namespace venom::common
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