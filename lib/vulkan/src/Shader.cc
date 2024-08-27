///
/// Project: VenomEngine
/// @file Shader.cc
/// @date Aug, 22 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/vulkan/Shader.h>

#include <fstream>

#include <venom/common/Resources.h>

#include "venom/common/math/Vector.h"

namespace venom::vulkan
{

ShaderPipeline::ShaderPipeline()
    : __graphicsPipeline(VK_NULL_HANDLE)
    , __pipelineLayout(VK_NULL_HANDLE)
    , __logicalDevice(VK_NULL_HANDLE)
{
}

ShaderPipeline::~ShaderPipeline()
{
    if (__graphicsPipeline != VK_NULL_HANDLE)
        vkDestroyPipeline(__logicalDevice, __graphicsPipeline, nullptr);
    if (__pipelineLayout != VK_NULL_HANDLE)
        vkDestroyPipelineLayout(__logicalDevice, __pipelineLayout, nullptr);
}

VkPipelineVertexInputStateCreateInfo ShaderPipeline::SetupVertexInput()
{
    // Vertex Input: Describes the format of the vertex data that will be passed to the vertex shader
    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

    // Define binding descriptions
    VkVertexInputBindingDescription bindingDescriptions[2] = {};

    // Binding 0: positions
    bindingDescriptions[0].binding = 0;
    bindingDescriptions[0].stride = sizeof(vc::Vec3);
    bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    // Binding 1: colors
    bindingDescriptions[1].binding = 1;
    bindingDescriptions[1].stride = sizeof(vc::Vec3);
    bindingDescriptions[1].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    // Define attribute descriptions
    VkVertexInputAttributeDescription attributeDescriptions[2] = {};

    // Attribute 0: position at binding 0
    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[0].offset = 0;

    // Attribute 1: color at binding 1
    attributeDescriptions[1].binding = 1;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[1].offset = 0;

    vertexInputInfo.vertexAttributeDescriptionCount = 2;
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions;
    vertexInputInfo.vertexBindingDescriptionCount = 2;
    vertexInputInfo.pVertexBindingDescriptions = bindingDescriptions;

    return vertexInputInfo;
}

vc::Error ShaderPipeline::LoadShader(const VkDevice logicalDevice,
                                     const std::string& shaderPath,
                                     VkPipelineShaderStageCreateInfo* pipelineCreateInfo)
{
    const auto folder_shaderPath = std::string("shaders/compiled/") + shaderPath + ".spv";
    const std::string path = vc::Resources::GetResourcePath(folder_shaderPath);
    std::ifstream file(path, std::ios::ate | std::ios::binary);

    if (!file.is_open())
    {
        vc::Log::Error("Failed to open file: %s", path.c_str());
        return vc::Error::Failure;
    }

    size_t fileSize = (size_t) file.tellg();
    std::vector<char> buffer(fileSize);
    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();

    VkShaderModuleCreateInfo shaderModuleCreateInfo = {};
    shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shaderModuleCreateInfo.codeSize = buffer.size();
    shaderModuleCreateInfo.pCode = reinterpret_cast<const uint32_t*>(buffer.data());

    if (vkCreateShaderModule(logicalDevice, &shaderModuleCreateInfo, nullptr, &pipelineCreateInfo->module) != VK_SUCCESS)
    {
        vc::Log::Error("Failed to create shader module");
        return vc::Error::Failure;
    }
    pipelineCreateInfo->sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    pipelineCreateInfo->pName = "main";
    if (shaderPath.find("vert") != std::string::npos || shaderPath.find("vertex") != std::string::npos || shaderPath.find("vs") != std::string::npos)
        pipelineCreateInfo->stage = VK_SHADER_STAGE_VERTEX_BIT;
    else if (shaderPath.find("frag") != std::string::npos || shaderPath.find("fs") != std::string::npos || shaderPath.find("pixel") != std::string::npos || shaderPath.find("ps") != std::string::npos)
        pipelineCreateInfo->stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    else if (shaderPath.find("comp") != std::string::npos || shaderPath.find("cs") != std::string::npos || shaderPath.find("compute") != std::string::npos)
        pipelineCreateInfo->stage = VK_SHADER_STAGE_COMPUTE_BIT;
    else if (shaderPath.find("geom") != std::string::npos)
        pipelineCreateInfo->stage = VK_SHADER_STAGE_GEOMETRY_BIT;
    else if (shaderPath.find("tesc") != std::string::npos)
        pipelineCreateInfo->stage = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
    else if (shaderPath.find("tese") != std::string::npos)
        pipelineCreateInfo->stage = VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
    else {
        vc::Log::Error("Unknown shader type: %s", shaderPath.c_str());
        return vc::Error::Failure;
    }
    return vc::Error::Success;
}

vc::Error ShaderPipeline::LoadShaders(const VkDevice logicalDevice, const SwapChain* swapChain,
    const RenderPass * renderPass, const std::vector<std::string>& shaderPaths)
{
    // Loading every shader
    std::vector<VkPipelineShaderStageCreateInfo> shaderStages(shaderPaths.size(), VkPipelineShaderStageCreateInfo{});
    for (int i = 0; i < shaderPaths.size(); ++i)
    {
        if (LoadShader(logicalDevice, shaderPaths[i], &shaderStages[i]) != vc::Error::Success)
        {
            vc::Log::Error("Failed to load shader: %s", shaderPaths[i].c_str());
            return vc::Error::Failure;
        }
    }
    // Check if duplicate stages
    for (int i = 0; i < shaderStages.size(); ++i)
    {
        for (int j = i + 1; j < shaderStages.size(); ++j)
        {
            if (shaderStages[i].stage == shaderStages[j].stage)
            {
                vc::Log::Error("Duplicate shader stages: [%s] | [%s]", shaderPaths[i].c_str(), shaderPaths[j].c_str());
                for (int k = 0; k < shaderStages.size(); ++k) {
                    vkDestroyShaderModule(logicalDevice, shaderStages[k].module, nullptr);
                }
                return vc::Error::Failure;
            }
        }
    }

    // Input Assembly: Describes how primitives are assembled
    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    // Primitive Restart allows you to break up lines and triangles in the strip topology, to break
    // up a line strip, you can insert a special index value that tells the GPU to start a new line
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.scissorCount = 1;

    // Rasterizer
    VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    // Cull mode determines the type of face culling, back bit means to cull back faces
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    // Front face is the vertex order that is considered front facing
    rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
    // Depth bias is just adding a constant value or a value proportional to the slope of the polygon
    // Can be used for shadow mapping
    // https://learn.microsoft.com/en-us/windows/win32/direct3d11/d3d10-graphics-programming-guide-output-merger-stage-depth-bias
    rasterizer.depthBiasEnable = VK_FALSE;
    //rasterizer.depthBiasConstantFactor = 0.0f;
    //rasterizer.depthBiasClamp = 0.0f;
    //rasterizer.depthBiasSlopeFactor = 0.0f;

    // Multisampling: on of the ways to do antialiasing
    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT;
    //multisampling.minSampleShading = 1.0f; // Optional
    //multisampling.pSampleMask = nullptr; // Optional
    //multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
    //multisampling.alphaToOneEnable = VK_FALSE; // Optional

    // TODO: Stencil & Depth testing
    // VkPipelineDepthStencilStateCreateInfo depthStencil{};
    // depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    // Depth test determines if a fragment is drawn based on its depth value
    // depthStencil.depthTestEnable = VK_TRUE;
    // Depth write determines if the depth value of a fragment is written to the depth buffer
    // depthStencil.depthWriteEnable = VK_TRUE;

    // Color blending
    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;
    //colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    //colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    //colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
    //colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    //colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    //colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

    VkPipelineColorBlendStateCreateInfo colorBlending{};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f; // Optional
    colorBlending.blendConstants[1] = 0.0f; // Optional
    colorBlending.blendConstants[2] = 0.0f; // Optional
    colorBlending.blendConstants[3] = 0.0f; // Optional

    // Dynamic States are to specify which states can be changed without recreating the pipeline
    std::vector<VkDynamicState> dynamicStates = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };
    VkPipelineDynamicStateCreateInfo dynamicState{};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
    dynamicState.pDynamicStates = dynamicStates.data();

    // Pipeline layout
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0; // Optional
    //pipelineLayoutInfo.pSetLayouts = nullptr; // Optional
    pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
    //pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

    if (vkCreatePipelineLayout(logicalDevice, &pipelineLayoutInfo, nullptr, &__pipelineLayout) != VK_SUCCESS)
    {
        vc::Log::Error("Failed to create pipeline layout");
        return vc::Error::Failure;
    }
    __logicalDevice = logicalDevice;

    // Vertex Input: Describes the format of the vertex data that will be passed to the vertex shader
    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

    // Define binding descriptions
    VkVertexInputBindingDescription bindingDescriptions[2] = {};

    // Binding 0: positions
    bindingDescriptions[0].binding = 0;
    bindingDescriptions[0].stride = sizeof(vc::Vec3);
    bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    // Binding 1: colors
    bindingDescriptions[1].binding = 1;
    bindingDescriptions[1].stride = sizeof(vc::Vec3);
    bindingDescriptions[1].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    // Define attribute descriptions
    VkVertexInputAttributeDescription attributeDescriptions[2] = {};

    // Attribute 0: position at binding 0
    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[0].offset = 0;

    // Attribute 1: color at binding 1
    attributeDescriptions[1].binding = 1;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[1].offset = 0;

    vertexInputInfo.vertexAttributeDescriptionCount = 2;
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions;
    vertexInputInfo.vertexBindingDescriptionCount = 2;
    vertexInputInfo.pVertexBindingDescriptions = bindingDescriptions;

    // Setting up the pipeline
    VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo = {};
    graphicsPipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    graphicsPipelineCreateInfo.stageCount = static_cast<uint32_t>(shaderStages.size());
    graphicsPipelineCreateInfo.pStages = shaderStages.data();
    graphicsPipelineCreateInfo.pVertexInputState = &vertexInputInfo;
    graphicsPipelineCreateInfo.pInputAssemblyState = &inputAssembly;
    graphicsPipelineCreateInfo.pViewportState = &viewportState;
    graphicsPipelineCreateInfo.pRasterizationState = &rasterizer;
    graphicsPipelineCreateInfo.pMultisampleState = &multisampling;
    //graphicsPipelineCreateInfo.pDepthStencilState = &depthStencil;
    graphicsPipelineCreateInfo.pColorBlendState = &colorBlending;
    graphicsPipelineCreateInfo.pDynamicState = &dynamicState;
    graphicsPipelineCreateInfo.layout = __pipelineLayout;
    graphicsPipelineCreateInfo.renderPass = renderPass->GetRenderPass();
    graphicsPipelineCreateInfo.subpass = 0; // Index of the subpass in the render pass where this pipeline will be used
    graphicsPipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE; // Pipeline to derive from: Optional
    //graphicsPipelineCreateInfo.basePipelineIndex = -1; // Optional

    if (vkCreateGraphicsPipelines(logicalDevice, VK_NULL_HANDLE, 1, &graphicsPipelineCreateInfo, nullptr, &__graphicsPipeline) != VK_SUCCESS)
    {
        vc::Log::Error("Failed to create graphics pipeline");
        return vc::Error::Failure;
    }

    // Cleaning up shader modules
    for (int i = 0; i < shaderStages.size(); ++i) {
        vkDestroyShaderModule(logicalDevice, shaderStages[i].module, nullptr);
    }

    return vc::Error::Success;
}

VkPipeline ShaderPipeline::GetPipeline() const
{
    return __graphicsPipeline;
}
}