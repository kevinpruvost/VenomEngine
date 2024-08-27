///
/// Project: VenomEngine
/// @file GraphicsApplication.h
/// @date Aug, 18 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/common/Error.h>

#include <venom/common/plugin/graphics/GraphicsPlugin.h>
#include "tools/cpp/runfiles/runfiles.h"


/// @brief Contains the entirety of the code of the VenomEngine project.
namespace venom
{
/// @brief High-Level Frontend of the VenomEngine project.
/// It also contains all the code shared between every APIs/Dynamic Libraries.
/// But most importantly, that is where the abstraction is setup for the different APIs contained in this project.
/// Use the alias 'vc' to access the namespace.
namespace common
{
class VENOM_COMMON_API GraphicsApplication : public GraphicsPluginObject
{
protected:
    GraphicsApplication();
public:
    static GraphicsApplication * Create();
    ~GraphicsApplication() override;
    virtual Error Run() = 0;
};
}
}