///
/// Project: VenomEngineWorkspace
/// @file ECS.h
/// @date Oct, 10 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/common/DLL.h>
#include <flecs.h>

namespace venom
{
namespace common
{
/**
 * @brief Entity Component System
 * Will mainly encapsulate the fabulous flecs library (https://github.com/SanderMertens/flecs.git)
 * This class will be contained in the VenomEngine class.
 */
class VENOM_COMMON_API ECS
{
public:
    ECS();
    ~ECS();

private:
    flecs::world __world;
};
}
}