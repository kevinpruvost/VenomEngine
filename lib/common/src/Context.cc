///
/// Project: VenomEngine
/// @file Context.cc
/// @date Aug, 18 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/common/Context.h>
#include <venom/common/Log.h>

#include <venom/common/plugin/graphics/GraphicsSettings.h>

namespace venom::common
{
Context * Context::s_context = nullptr;

Context::Context()
    : __window(nullptr)
    , __keyboardModifierState(0x0000)
    , __mousePos{0.0, 0.0}
    , __mouseLastPos{0.0, 0.0}
    , __shouldClose(false)
{
    venom_assert(s_context == nullptr, "Context::Context() : Context already exists");
    s_context = this;
    for (int i = 0; i < __keyboardState.size(); ++i) __keyboardState[i] = InputState::Unknown;
    for (int i = 0; i < __mouseState.size(); ++i) __mouseState[i] = InputState::Unknown;
}

Context::~Context()
{
    if (__window)
        glfwDestroyWindow(__window);
    glfwTerminate();
    s_context = nullptr;
}

vc::Error Context::InitContext()
{
    if (!glfwInit())
        return vc::Error::Failure;
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    // Get window configurations
    GLFWmonitor * monitor = glfwGetPrimaryMonitor();
    if (!monitor) {
        int count;
        GLFWmonitor ** monitors = glfwGetMonitors(&count);
        if (count == 0) {
            vc::Log::Error("No monitors detected by GLFW.");
            return vc::Error::Failure;
        }
        monitor = monitors[0];
    }
    int count;
    const GLFWvidmode * modes = glfwGetVideoModes(monitor, &count);
    const GLFWvidmode * activeMode = const_cast<GLFWvidmode *>(glfwGetVideoMode(monitor));

    // Sort video modes by removing ones that don't have the same refresh rate as the current active video mode
    for (int i = 0; i < count; ++i)
    {
        if (modes[i].refreshRate == activeMode->refreshRate) {
            __modes.push_back(modes[i]);
        }
    }

#ifdef _DEBUG
    for (int i = 0; i < __modes.size(); i++) {
        Log::LogToFile("Mode: %d: %dx%d | Refresh Rate: %d", i, __modes[i].width, __modes[i].height, __modes[i].refreshRate);
    }
#endif
    __width = std::min(activeMode->width * 3 / 4, 1000);
    __height = std::min(activeMode->height * 3 / 4, 800);
    // Just take the last video mode by default and take 3/4 of the window size
    __window = glfwCreateWindow(
        __width, __height,
        "VenomEngine",
        nullptr,
        nullptr);

    // Input
    glfwSetKeyCallback(__window, [](GLFWwindow * window, int key, int scancode, int action, int mods)
    {
        Context * context = s_context;

        // Keys
        if (action == GLFW_PRESS) {
            context->__keyboardState[context->__ConvertGLFWEnumToKeyboardInput(key)] = InputState::Pressed;
        } else if (action == GLFW_RELEASE) {
            context->__keyboardState[context->__ConvertGLFWEnumToKeyboardInput(key)] = InputState::Released;
            context->__keyReleasedStack.push(context->__ConvertGLFWEnumToKeyboardInput(key));
        } else if (action == GLFW_REPEAT) {
            context->__keyboardState[context->__ConvertGLFWEnumToKeyboardInput(key)] = InputState::Repeat;
        }

        // Mods
        context->__keyboardModifierState = 0x0000;
        if (mods & GLFW_MOD_SHIFT) context->__keyboardModifierState |= KeyboardModifier::KeyboardModShift;
        if (mods & GLFW_MOD_CONTROL) context->__keyboardModifierState |= KeyboardModifier::KeyboardModControl;
        if (mods & GLFW_MOD_ALT) context->__keyboardModifierState |= KeyboardModifier::KeyboardModAlt;
        if (mods & GLFW_MOD_SUPER) context->__keyboardModifierState |= KeyboardModifier::KeyboardModSuper;
    });

    glfwSetMouseButtonCallback(__window, [](GLFWwindow * window, int button, int action, int mods)
    {
        Context * context = s_context;
        if (action == GLFW_PRESS) {
            context->__mouseState[button] = InputState::Pressed;
        } else if (action == GLFW_RELEASE) {
            context->__mouseState[button] = InputState::Released;
            context->__mouseReleasedStack.push(static_cast<MouseButton>(button));
        }
    });

    glfwSetWindowSizeCallback(__window, [](GLFWwindow * window, int width, int height)
    {
        Context * context = s_context;
        context->__width = width;
        context->__height = height;
        vc::GraphicsSettings::SetWindowResolution(width, height);
    });

    // Set Initial pos
    glfwGetCursorPos(__window, &__mousePos[0], &__mousePos[1]);
    memcpy(__mouseLastPos, __mousePos, sizeof(__mousePos));

    return Error::Success;
}

void Context::PollEvents()
{
    // Check for released keys in the previous frame
    while (!__keyReleasedStack.empty()) {
        __keyboardState[__keyReleasedStack.top()] = InputState::Unknown;
        __keyReleasedStack.pop();
    }
    while (!__mouseReleasedStack.empty()) {
        __mouseState[__mouseReleasedStack.top()] = InputState::Unknown;
        __mouseReleasedStack.pop();
    }

    glfwPollEvents();
    
    __shouldClose = glfwWindowShouldClose(__window);

    // Mouse movement
    memcpy(__mouseLastPos, __mousePos, sizeof(__mousePos));
    glfwGetCursorPos(__window, &__mousePos[0], &__mousePos[1]);
}

bool Context::ShouldClose()
{
    return __shouldClose;
}

}
