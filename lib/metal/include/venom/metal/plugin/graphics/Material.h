///
/// Project: VenomEngine
/// @file Material.h
/// @date Oct, 02 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/common/plugin/graphics/Material.h>

namespace venom
{
namespace metal
{

class MetalMaterial : public vc::MaterialImpl
{
public:
    MetalMaterial();
    ~MetalMaterial();
};

}
}
