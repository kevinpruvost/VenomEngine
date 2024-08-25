///
/// Project: VenomEngine
/// @file Context.h
/// @date Aug, 18 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <common/Error.h>

#include <GLFW/glfw3.h>
#include <vector>

namespace venom
{
namespace common
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
}