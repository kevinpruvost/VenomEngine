///
/// Project: VenomEngineWorkspace
/// @file ContextApple.h
/// @date Mar, 08 2025
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/common/Context.h>

#if defined(VENOM_PLATFORM_APPLE)

#ifdef __OBJC__

#include <venom/common/context/apple/ContextAppleInfo.h>

struct ContextAppleData
{
    __strong id __window;
    __strong id __view;
    __strong id __delegate;
    id __layer;
};

#endif

namespace venom
{
namespace context
{
namespace apple
{
class VENOM_COMMON_API ContextApple : public vc::Context
{
public:
    ContextApple();
    ~ContextApple();

    static ContextApple * GetAppleContext();
    void _SetWindowTitle(const char* title) override;
    void * _GetWindow() override;
    vc::Error _InitContext() override;
    vc::Error _UpdateVideoMode() override;
    vc::Error _UpdateRefreshRate() override;
    vc::Error _UpdateScreen() override;
    vc::Error _SetFullscreen() override;

    vc::Error Run(int argc, const char * argv[]) override;
    inline vc::Error Loop() { return _runLoopFunction(); }
    static inline vc::Error RunLoop() { return GetAppleContext()->Loop(); }

#ifdef __OBJC__
    static inline CAMetalLayer * GetMetalLayer() { return GetAppleContext()->__contextAppleData.__layer; }
    void __GiveMetalLayer(CAMetalLayer * layer);
    void __UpdateWindowSize(CGSize size);

    AppleWindow * GetAppleWindow();
    AppleView * GetAppleView();
#endif

protected:
    bool _ShouldClose() override;
    void _GetCursorPos(double * pos) override;
    void _PollEvents() override;

private:
    int __ConvertKeyboardInputToAppleEnum(vc::KeyboardInput key) const;
    vc::KeyboardInput __ConvertAppleEnumToKeyboardInput(int key) const;

private:
#ifdef __OBJC__
    ContextAppleData __contextAppleData;
    ContextAppleData * GetData();
#endif
};

}
}
}

venom::context::apple::ContextApple * VENOM_COMMON_API CreateContextApple();

#endif
