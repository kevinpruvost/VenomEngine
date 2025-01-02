///
/// Project: VenomEngineWorkspace
/// @file ComponentManager.h
/// @date Dec, 07 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once
#include <venom/common/ECS.h>

namespace venom
{
namespace common
{
class VENOM_COMMON_API ComponentManager : public Component
{
public:
    ComponentManager();
    ~ComponentManager();

    void Update(Entity entity) override;
    void _GUI(const Entity entity) override;
    vc::String _GetComponentTitle() override;
};
}
}