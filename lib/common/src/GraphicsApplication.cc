///
/// Project: VenomEngine
/// @file GraphicsApplication.cc
/// @date Aug, 18 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/common/plugin/graphics/GraphicsApplication.h>
#include <venom/common/plugin/graphics/GraphicsPlugin.h>
#include <venom/common/DLL.h>

#include <iostream>

namespace venom::common
{

GraphicsApplication::GraphicsApplication()
{
}

GraphicsApplication* GraphicsApplication::Create()
{
    return GraphicsPlugin::Get()->CreateGraphicsApplication();
}
}