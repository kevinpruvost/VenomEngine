///
/// Project: Bazel_Vulkan_Metal
/// File: vulkan.h
/// Date: 8/18/2024
/// Description:
/// Author: Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <vulkan/vulkan.hpp>
#include <common/Application.h>
#include <GLFW/glfw3.h>

class VulkanApplication : public venom::ApplicationBackend
{
public:
    venom::Error run() override;
};

extern "C" EXPORT venom::ApplicationBackend* createApplication();