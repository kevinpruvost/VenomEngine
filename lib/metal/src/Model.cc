///
/// Project: VenomEngine
/// @file Model.cc
/// @date Oct, 01 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/metal/plugin/graphics/Model.h>

namespace venom
{
namespace metal
{
MetalModel::MetalModel()
    : vc::ModelImpl()
{
    _ResetResource();
}

MetalModel::~MetalModel()
{
}

void MetalModel::_ResetResource()
{
    _resource.reset(new MetalModelResource());
}

void MetalModel::Draw()
{
}
}
}