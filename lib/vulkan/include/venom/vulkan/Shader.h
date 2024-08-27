///
/// Project: VenomEngine
/// @file Shader.h
/// @date Aug, 22 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/vulkan/PhysicalDevice.h>
#include <venom/vulkan/SwapChain.h>
#include <venom/vulkan/RenderPass.h>
#include <venom/vulkan/VertexBuffer.h>

#include "tools/cpp/runfiles/runfiles.h"

namespace venom
{
namespace vulkan
{
class ShaderPipeline
{
public:
    ShaderPipeline();
    ~ShaderPipeline();
    ShaderPipeline(const ShaderPipeline&) = delete;
    ShaderPipeline& operator=(const ShaderPipeline&) = delete;

    vc::Error AddVertexBufferToLayout(const uint32_t vertexCount, const uint32_t vertexSize, const VkBufferUsageFlags flags,
        const VkSharingMode sharingMode, const VkMemoryPropertyFlags memoryProperties, const void *data,
        const uint32_t binding, const uint32_t location, const uint32_t offset);
    vc::Error LoadShaders(const SwapChain * swapChain, const RenderPass * renderPass, const std::vector<std::string>& shaderPaths);
    VkPipeline GetPipeline() const;

private:
    vc::Error LoadShader(const std::string& shaderPath, VkPipelineShaderStageCreateInfo * pipelineCreateInfo);

private:
    VkPipeline __graphicsPipeline;
    VkPipelineLayout __pipelineLayout;
    std::vector<std::unique_ptr<VertexBuffer>> __vertexBuffers;

    std::vector<VkVertexInputBindingDescription> __bindingDescriptions;
    std::vector<VkVertexInputAttributeDescription> __attributeDescriptions;
};

}
}