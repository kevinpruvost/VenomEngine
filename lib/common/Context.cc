///
/// Project: Bazel_Vulkan_Metal
/// File: Context.cc
/// Date: 8/18/2024
/// Description: 
/// Author: Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <common/Context.h>
#include <common/Log.h>

namespace venom
{

Context::Context()
    : __window(nullptr)
{
}

Context::~Context()
{
    if (__window) {
        glfwDestroyWindow(__window);
    }
    glfwTerminate();
}

Error Context::InitContext()
{
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwInit();

    // Get window configurations
    GLFWmonitor * monitor = glfwGetPrimaryMonitor();
    int count;
    const GLFWvidmode * modes = glfwGetVideoModes(monitor, &count);
    const GLFWvidmode * activeMode = const_cast<GLFWvidmode *>(glfwGetVideoMode(monitor));

    // Sort video modes by removing ones that don't have the same refresh rate as the current active video mode
    for (int i = 0; i < count; i++)
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
    // Just take the last video mode by default and take 3/4 of the window size
    __window = glfwCreateWindow(
        __modes[__modes.size() - 1].width * 3 / 4,
        __modes[__modes.size() - 1].height * 3 / 4,
        "Vulkan",
        nullptr,
        nullptr);
    return Error::Success;
}

bool Context::ShouldClose()
{
    return glfwWindowShouldClose(__window);
}

void Context::PollEvents()
{
    glfwPollEvents();
}

GLFWwindow* Context::GetWindow()
{
    return __window;
}

const GLFWwindow* Context::GetWindow() const
{
    return __window;
}
}
