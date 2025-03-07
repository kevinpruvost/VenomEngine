///
/// Project: VenomEngine
/// @file Model.h
/// @date Oct, 01 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/common/plugin/graphics/Model.h>

namespace venom
{
namespace metal
{
class MetalModelResource : public vc::ModelResource
{
};

class MetalModel : public vc::ModelImpl
{
public:
    MetalModel();
    virtual ~MetalModel();

    void _ResetResource() override;

    void Draw() override;
};
}
}
