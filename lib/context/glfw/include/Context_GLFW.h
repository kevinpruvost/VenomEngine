///
/// Project: VenomEngineWorkspace
/// @file Context_GLFW.h
/// @date Mar, 08 2025
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/common/plugin/context/Context.h>
#include <GLFW/glfw3.h>

namespace venom
{
namespace context
{
namespace glfw
{
class VENOM_CONTEXT_API ContextGLFW : public vc::Context
{
public:
    ContextGLFW();
    ~ContextGLFW();

    void _SetWindowTitle(const char* title) override;
    void * _GetWindow() override;
    vc::Error _InitContext() override;
    vc::Error _UpdateVideoMode() override;
    vc::Error _UpdateRefreshRate() override;
    vc::Error _UpdateScreen() override;
    vc::Error _SetFullscreen() override;

protected:
    bool _ShouldClose() override;
    void _GetCursorPos(double * pos) override;
    void _PollEvents() override;

private:
    int __ConvertKeyboardInputToGLFWEnum(vc::KeyboardInput key) const;
    vc::KeyboardInput __ConvertGLFWEnumToKeyboardInput(int key) const;

private:
    GLFWwindow * __window;
};
}
}
}