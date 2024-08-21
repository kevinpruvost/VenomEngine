///
/// Project: Bazel_Vulkan_Metal
/// File: Context.h
/// Date: 8/18/2024
/// Description: 
/// Author: Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <common/Error.h>

#include <GLFW/glfw3.h>
#include <vector>

namespace venom
{
class VENOM_COMMON_API Context
{
public:
    Context();
    ~Context();
public:
    Error InitContext();
    bool ShouldClose();
    void PollEvents();
    GLFWwindow * GetWindow();
    const GLFWwindow * GetWindow() const;

private:
    GLFWwindow * __window;
    std::vector<GLFWvidmode> __modes;
};
}