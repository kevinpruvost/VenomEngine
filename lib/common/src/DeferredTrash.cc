///
/// Project: VenomEngineWorkspace
/// @file DeferredTrash.cc
/// @date Jan, 21 2025
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/common/DeferredTrash.h>
#include <venom/common/VenomSettings.h>

namespace venom
{
namespace common
{
DeferredTrashBin * DeferredTrashBin::s_instance = nullptr;

DeferredTrashBin::DeferredTrashBin()
{
    s_instance = this;
}

DeferredTrashBin::~DeferredTrashBin()
{
    s_instance = nullptr;
    // Empties everything on app exit
    for (TrashBlock & block : __trash) {
        block.destructor(block.trash);
    }
}

void DeferredTrashBin::EmptyDeferredTrash()
{
    for (TrashBlock & block : __trash)
    {
        if (block.counter == 0) {
            block.destructor(block.trash);
        }
        --block.counter;
    }
    std::erase_if(__trash, [](const TrashBlock & block) { return block.counter < 0; });
}
}
}
