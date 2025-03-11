///
/// Project: VenomEngineWorkspace
/// @file Context_GLFW.cc
/// @date Mar, 08 2025
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/common/context/ContextGLFW.h>
#if !defined(VENOM_PLATFORM_IOS)

#include <venom/common/plugin/graphics/GraphicsSettings.h>

#include <venom/common/Log.h>

namespace venom
{
namespace context
{
namespace glfw
{
ContextGLFW::ContextGLFW()
    : __window(nullptr)
{
}

ContextGLFW::~ContextGLFW()
{
    if (__window)
        glfwDestroyWindow(__window);
    glfwTerminate();
}

ContextGLFW* ContextGLFW::GetGLFWContext()
{
    return dynamic_cast<ContextGLFW *>(Get());
}

void ContextGLFW::_SetWindowTitle(const char* title)
{
    glfwSetWindowTitle(__window, title);
}

void* ContextGLFW::_GetWindow()
{
    return __window;
}

vc::Error ContextGLFW::_InitContext()
{
    if (!glfwInit())
        return vc::Error::Failure;
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    // Get window configurations
    GLFWmonitor * monitor = glfwGetPrimaryMonitor();
    int countMonitors;
    GLFWmonitor ** monitors = glfwGetMonitors(&countMonitors);
    if (!monitor) {
        if (countMonitors == 0) {
            vc::Log::Error("No monitors detected by GLFW.");
            return vc::Error::Failure;
        }
        monitor = monitors[0];
    }
    int countModes;
    const GLFWvidmode * activeMode = const_cast<GLFWvidmode *>(glfwGetVideoMode(monitor));

    for (int i = 0; i < countMonitors; ++i)
    {
        const GLFWvidmode * modes = glfwGetVideoModes(monitors[i], &countModes);
        vc::Screen & screen = _screens.emplace_back();
        if (monitor == monitors[i]) {
            _currentScreenIndex = i;
        }
        for (int j = 0; j < countModes; ++j)
        {
            const GLFWvidmode & mode = modes[j];
            screen.AddVideoMode(mode.width, mode.height, mode.refreshRate);
        }
        screen.SortVideoModes();
    }

    // Find current video mode and refresh rate
    for (int j = 0; j < _screens[_currentScreenIndex].GetVideoModeCount(); ++j) {
        const vc::ScreenVideoMode & mode = _screens[_currentScreenIndex].GetVideoMode(j);
        if (mode.GetWidth() == activeMode->width && mode.GetHeight() == activeMode->height) {
            _currentVideoModeIndex = j;
            break;
        }
    }
    _currentRefreshRate = activeMode->refreshRate;

    _width = std::min(activeMode->width * 3 / 4, 1000);
    _height = std::min(activeMode->height * 3 / 4, 800);
    // Just take the last video mode by default and take 3/4 of the window size
    __window = glfwCreateWindow(
        _width, _height,
        "VenomEngine",
        nullptr,
        nullptr);

    // Input
    glfwSetKeyCallback(__window, [](GLFWwindow * window, int key, int scancode, int action, int mods)
    {
        ContextGLFW * context = GetGLFWContext();

        // Keys
        if (action == GLFW_PRESS) {
            context->_SetKeyboardState(context->__ConvertGLFWEnumToKeyboardInput(key), vc::InputState::Pressed);
        } else if (action == GLFW_RELEASE) {
            context->_SetKeyboardState(context->__ConvertGLFWEnumToKeyboardInput(key), vc::InputState::Released);
            context->_AddKeyReleasedEvent(context->__ConvertGLFWEnumToKeyboardInput(key));
        } else if (action == GLFW_REPEAT) {
            context->_SetKeyboardState(context->__ConvertGLFWEnumToKeyboardInput(key), vc::InputState::Repeat);
        }

        // Mods
        context->_SetKeyboardModifierState(mods & GLFW_MOD_SHIFT, mods & GLFW_MOD_CONTROL, mods & GLFW_MOD_ALT, mods & GLFW_MOD_SUPER);
    });

    glfwSetMouseButtonCallback(__window, [](GLFWwindow * window, int button, int action, int mods)
    {
        ContextGLFW * context = GetGLFWContext();

        if (action == GLFW_PRESS) {
            context->_SetMouseState(static_cast<vc::MouseButton>(button), vc::InputState::Pressed);
        } else if (action == GLFW_RELEASE) {
            context->_SetMouseState(static_cast<vc::MouseButton>(button), vc::InputState::Released);
            context->_AddMouseReleasedEvent(static_cast<vc::MouseButton>(button));
        }
    });

    glfwSetWindowSizeCallback(__window, [](GLFWwindow * window, int width, int height)
    {
        ContextGLFW * context = GetGLFWContext();
        context->_width = width;
        context->_height = height;
        vc::GraphicsSettings::SetWindowResolution(width, height);
    });

    return vc::Error::Success;
}

vc::Error ContextGLFW::_UpdateVideoMode()
{
    return _UpdateScreen();
}

vc::Error ContextGLFW::_UpdateRefreshRate()
{
    return _UpdateScreen();
}

vc::Error ContextGLFW::_UpdateScreen()
{
    int count;
    GLFWmonitor ** monitors = glfwGetMonitors(&count);
    int x, y;
    glfwGetWindowPos(__window, &x, &y);
    glfwSetWindowMonitor(__window, _fullscreen ? monitors[_currentScreenIndex] : nullptr, x, y, _width, _height, _currentRefreshRate);
    return vc::Error::Success;
}

vc::Error ContextGLFW::_SetFullscreen()
{
    return _UpdateScreen();
}

bool ContextGLFW::_ShouldClose()
{
    return glfwWindowShouldClose(__window);
}

void ContextGLFW::_GetCursorPos(double* pos)
{
    glfwGetCursorPos(__window, &pos[0], &pos[1]);
}

void ContextGLFW::_PollEvents()
{
    glfwPollEvents();
}
}
}
}
#endif
venom::context::glfw::ContextGLFW* CreateContextGLFW()
{
    return new venom::context::glfw::ContextGLFW();
}
