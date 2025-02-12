///
/// Project: VenomEngineWorkspace
/// @file DeferredTrash.h
/// @date Jan, 21 2025
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/common/Containers.h>
#include <venom/common/VenomSettings.h>

namespace venom
{
namespace common
{
class DeferredTrashBin;

class VENOM_COMMON_API DeferredTrashBin
{
public:
    DeferredTrashBin();
    virtual ~DeferredTrashBin();

    struct TrashBlock
    {
        int counter;
        void * trash;
        void (*destructor)(void *);
    };

    template<typename T>
    static void AddDeferredTrash(T * trash) {
        if (!trash) return;
        // Important to check because s_instance is nullptr when the graphics engine is shutting down
        if (s_instance) {
            s_instance->__trash.emplace_back(VENOM_MAX_FRAMES_IN_FLIGHT, trash, [](void * trash) { delete static_cast<T *>(trash); });
        } else {
            delete trash;
        }
    }
    template<typename T>
    static void AddDeferredTrash(T * trash, void (*destructor)(void *)) {
        if (!trash) return;
        // Important to check because s_instance is nullptr when the graphics engine is shutting down
        if (s_instance) {
            s_instance->__trash.emplace_back(VENOM_MAX_FRAMES_IN_FLIGHT, trash, destructor);
        } else {
            delete trash;
        }
    }

    void EmptyDeferredTrash();

private:
    static DeferredTrashBin * s_instance;
    vc::Vector<TrashBlock> __trash;
};

template<typename T>
class VENOM_COMMON_API DeferredTrash
{
public:
    DeferredTrash(T * trash)
        : __trash(trash)
    {
    }
    ~DeferredTrash()
    {
        DeferredTrashBin::AddDeferredTrash(__trash);
    }
    const T * operator->() const { return __trash; }
    T * operator->() { return __trash; }
    T & operator*() { return *__trash; }
    const T & operator*() const { return *__trash; }

    void Reset(T * trash) {
        DeferredTrashBin::AddDeferredTrash(__trash);
        __trash = trash;
    }
private:
    T * __trash;
};
}
}
