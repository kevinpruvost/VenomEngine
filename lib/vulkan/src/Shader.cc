///
/// Project: VenomEngine
/// @file Shader.cc
/// @date Aug, 22 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/vulkan/plugin/graphics/Shader.h>
#include <venom/vulkan/Allocator.h>

#include <fstream>

#include <venom/common/Resources.h>

#include <venom/common/math/Vector.h>
#include <venom/vulkan/LogicalDevice.h>

#include <venom/common/VenomSettings.h>

#include <venom/vulkan/VulkanApplication.h>

namespace venom::vulkan
{

VulkanShader::VulkanShader()
    : __graphicsPipeline(VK_NULL_HANDLE)
    , __pipelineLayout(VK_NULL_HANDLE)
    , __multisamplingCreateInfo{}
    , __rasterizerCreateInfo{}
{
    // MultisamplingOption: on of the ways to do antialiasing
    __multisamplingCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    __multisamplingCreateInfo.sampleShadingEnable = VK_FALSE;
    __multisamplingCreateInfo.rasterizationSamples = static_cast<VkSampleCountFlagBits>(vc::GraphicsSettings::GetSamplesMultisampling());
    //multisampling.minSampleShading = 1.0f; // Optional
    //multisampling.pSampleMask = nullptr; // Optional
    //multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
    //multisampling.alphaToOneEnable = VK_FALSE; // Optional

    // Rasterizer
    __rasterizerCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    __rasterizerCreateInfo.depthClampEnable = VK_FALSE;
    __rasterizerCreateInfo.rasterizerDiscardEnable = VK_FALSE;
    __rasterizerCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;
    __rasterizerCreateInfo.lineWidth = 1.0f;
    // Cull mode determines the type of face culling, back bit means to cull back faces
    __rasterizerCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
    // Front face is the vertex order that is considered front facing
    __rasterizerCreateInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    // Depth bias is just adding a constant value or a value proportional to the slope of the polygon
    // Can be used for shadow mapping
    // https://learn.microsoft.com/en-us/windows/win32/direct3d11/d3d10-graphics-programming-guide-output-merger-stage-depth-bias
    __rasterizerCreateInfo.depthBiasEnable = VK_FALSE;
    //rasterizer.depthBiasConstantFactor = 0.0f;
    //rasterizer.depthBiasClamp = 0.0f;
    //rasterizer.depthBiasSlopeFactor = 0.0f;
}

VulkanShader::~VulkanShader()
{
    if (__graphicsPipeline != VK_NULL_HANDLE)
        vkDestroyPipeline(LogicalDevice::GetVkDevice(), __graphicsPipeline, Allocator::GetVKAllocationCallbacks());
    if (__pipelineLayout != VK_NULL_HANDLE)
        vkDestroyPipelineLayout(LogicalDevice::GetVkDevice(), __pipelineLayout, Allocator::GetVKAllocationCallbacks());
}

VulkanShader::VulkanShader(VulkanShader&& other) noexcept
    : __graphicsPipeline(other.__graphicsPipeline)
    , __pipelineLayout(other.__pipelineLayout)
{
    other.__graphicsPipeline = VK_NULL_HANDLE;
    other.__pipelineLayout = VK_NULL_HANDLE;
}

VulkanShader& VulkanShader::operator=(VulkanShader&& other) noexcept
{
    if (this != &other) {
        __graphicsPipeline = other.__graphicsPipeline;
        __pipelineLayout = other.__pipelineLayout;
        other.__graphicsPipeline = VK_NULL_HANDLE;
        other.__pipelineLayout = VK_NULL_HANDLE;
    }
    return *this;
}

vc::Error VulkanShader::_LoadShader(const std::string& path)
{
    std::string basePath = vc::Resources::GetShadersFolderPath() + "compiled/";

    // List all stages of the basePath
    for (const auto& entry : std::filesystem::directory_iterator(basePath))
    {
        // Check if entry filename starts with path
        auto filename = entry.path().filename().string();
        if (filename.starts_with(path) == false) continue;

        if (entry.is_regular_file())
        {
            std::string shaderPath = entry.path().string();
            std::string shaderName = entry.path().filename().string();
            if (shaderName.ends_with(".spv"))
            {
                vc::Log::Print("Loading shader: %s", shaderPath.c_str());
                _shaderPaths.emplace_back(std::move(shaderPath));
            }
        }
    }
    if (_shaderPaths.empty()) return vc::Error::Failure;
    return LoadShaders();
}

void VulkanShader::SetLineWidth(const float width)
{
    __rasterizerCreateInfo.lineWidth = width;
}

void VulkanShader::SetMultiSamplingCount(const int samples)
{
    __multisamplingCreateInfo.rasterizationSamples = static_cast<VkSampleCountFlagBits>(samples);
}

VkFormat GetVkFormatFromShaderVertexFormat(const vc::ShaderVertexFormat format)
{
    switch (format)
    {
        case vc::ShaderVertexFormat::Float:
            return VK_FORMAT_R32_SFLOAT;
        case vc::ShaderVertexFormat::Vec2:
            return VK_FORMAT_R32G32_SFLOAT;
        case vc::ShaderVertexFormat::Vec3:
            return VK_FORMAT_R32G32B32_SFLOAT;
        case vc::ShaderVertexFormat::Vec4:
            return VK_FORMAT_R32G32B32A32_SFLOAT;
        case vc::ShaderVertexFormat::Int:
            return VK_FORMAT_R32_SINT;
        case vc::ShaderVertexFormat::IVec2:
            return VK_FORMAT_R32G32_SINT;
        case vc::ShaderVertexFormat::IVec3:
            return VK_FORMAT_R32G32B32_SINT;
        case vc::ShaderVertexFormat::IVec4:
            return VK_FORMAT_R32G32B32A32_SINT;
        case vc::ShaderVertexFormat::Uint:
            return VK_FORMAT_R32_UINT;
        case vc::ShaderVertexFormat::UVec2:
            return VK_FORMAT_R32G32_UINT;
        case vc::ShaderVertexFormat::UVec3:
            return VK_FORMAT_R32G32B32_UINT;
        case vc::ShaderVertexFormat::UVec4:
            return VK_FORMAT_R32G32B32A32_UINT;
        case vc::ShaderVertexFormat::Mat2:
            return VK_FORMAT_R32G32_SFLOAT;
        case vc::ShaderVertexFormat::Mat3:
            return VK_FORMAT_R32G32B32_SFLOAT;
        case vc::ShaderVertexFormat::Mat4:
            return VK_FORMAT_R32G32B32A32_SFLOAT;
        default:
            venom_assert(false, "Unknown vertex format");
            return VK_FORMAT_UNDEFINED;
    }
}

void VulkanShader::_AddVertexBufferToLayout(const uint32_t vertexSize, const uint32_t binding, const uint32_t location,
    const uint32_t offset, const vc::ShaderVertexFormat format)
{
    __bindingDescriptions.push_back({
        .binding = binding,
        .stride = vertexSize,
        .inputRate = VK_VERTEX_INPUT_RATE_VERTEX
    });
    VkFormat vkFormat = GetVkFormatFromShaderVertexFormat(format);
    __attributeDescriptions.push_back({
        .location = location,
        .binding = binding,
        .format = vkFormat,
        .offset = offset
    });
}

vc::Error VulkanShader::LoadShader(const std::string& shaderPath, VkPipelineShaderStageCreateInfo* pipelineCreateInfo)
{
    std::ifstream file(shaderPath, std::ios::ate | std::ios::binary);

    if (!file.is_open())
    {
        vc::Log::Error("Failed to open file: %s", shaderPath.c_str());
        return vc::Error::Failure;
    }

    size_t fileSize = (size_t) file.tellg();
    std::vector<char> buffer(fileSize);
    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();

    // // Reflecting shader, we'd better use that when doing a common shader
    // {
    //     SpvReflectShaderModule module;
    //     SpvReflectResult result = spvReflectCreateShaderModule(buffer.size(), buffer.data(), &module);
    //     assert(result == SPV_REFLECT_RESULT_SUCCESS);
    //
    //     // Enumerate and extract shader's input variables
    //     uint32_t varCount = 0;
    //     result = spvReflectEnumerateInputVariables(&module, &varCount, nullptr);
    //     assert(result == SPV_REFLECT_RESULT_SUCCESS);
    //     // Get input variables
    //     std::vector<SpvReflectInterfaceVariable*> inputVars(varCount, nullptr);
    //     result = spvReflectEnumerateInputVariables(&module, &varCount, inputVars.data());
    //     assert(result == SPV_REFLECT_RESULT_SUCCESS);
    //
    //     // Get descriptor bindings
    //     result = spvReflectEnumerateDescriptorBindings(&module, &varCount, nullptr);
    //     assert(result == SPV_REFLECT_RESULT_SUCCESS);
    //     std::vector<SpvReflectDescriptorBinding*> bindings(varCount, nullptr);
    //     result = spvReflectEnumerateDescriptorBindings(&module, &varCount, bindings.data());
    //     assert(result == SPV_REFLECT_RESULT_SUCCESS);
    //
    //     // Get descriptor sets
    //     result = spvReflectEnumerateDescriptorSets(&module, &varCount, nullptr);
    //     assert(result == SPV_REFLECT_RESULT_SUCCESS);
    //     std::vector<SpvReflectDescriptorSet*> descriptorSets(varCount, nullptr);
    //     result = spvReflectEnumerateDescriptorSets(&module, &varCount, descriptorSets.data());
    //
    //     // Get push constants
    //     result = spvReflectEnumeratePushConstantBlocks(&module, &varCount, nullptr);
    //     assert(result == SPV_REFLECT_RESULT_SUCCESS);
    //     std::vector<SpvReflectBlockVariable*> pushConstants(varCount, nullptr);
    //     result = spvReflectEnumeratePushConstantBlocks(&module, &varCount, pushConstants.data());
    //
    //     // Destroy the reflection data when no longer required.
    //     spvReflectDestroyShaderModule(&module);
    // }

    VkShaderModuleCreateInfo shaderModuleCreateInfo = {};
    shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shaderModuleCreateInfo.codeSize = buffer.size();
    shaderModuleCreateInfo.pCode = reinterpret_cast<const uint32_t*>(buffer.data());

    if (vkCreateShaderModule(LogicalDevice::GetVkDevice(), &shaderModuleCreateInfo, Allocator::GetVKAllocationCallbacks(), &pipelineCreateInfo->module) != VK_SUCCESS)
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

vc::Error VulkanShader::LoadShaders()
{
    // Loading every shader
    std::vector<VkPipelineShaderStageCreateInfo> shaderStages(_shaderPaths.size(), VkPipelineShaderStageCreateInfo{});
    for (int i = 0; i < _shaderPaths.size(); ++i)
    {
        if (LoadShader(_shaderPaths[i], &shaderStages[i]) != vc::Error::Success)
        {
            vc::Log::Error("Failed to load shader: %s", _shaderPaths[i].c_str());
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
                vc::Log::Error("Duplicate shader stages: [%s] | [%s]", _shaderPaths[i].c_str(), _shaderPaths[j].c_str());
                for (int k = 0; k < shaderStages.size(); ++k) {
                    vkDestroyShaderModule(LogicalDevice::GetVkDevice(), shaderStages[k].module, Allocator::GetVKAllocationCallbacks());
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

    // Stencil & Depth testing
    VkPipelineDepthStencilStateCreateInfo depthStencil{};
    depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    // Depth test determines if a fragment is drawn based on its depth value
    depthStencil.depthTestEnable = VK_TRUE;
    // Depth write determines if the depth value of a fragment is written to the depth buffer
    depthStencil.depthWriteEnable = VK_TRUE;
    depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
    depthStencil.depthBoundsTestEnable = VK_FALSE;
    depthStencil.minDepthBounds = 0.0f; // Optional
    depthStencil.maxDepthBounds = 1.0f; // Optional
    depthStencil.stencilTestEnable = VK_FALSE;
    depthStencil.front = {}; // Optional
    depthStencil.back = {}; // Optional

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

    // Push constants
    VkPushConstantRange pushConstantRange{};
    pushConstantRange.offset = 0;
    //pushConstantRange.size = 2 * sizeof(vcm::Mat4); // View and Projection matrices
    //pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

    // Pipeline layout
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = DescriptorPool::GetPool()->GetDescriptorSetLayouts().size(); // Optional
    pipelineLayoutInfo.pSetLayouts = DescriptorPool::GetPool()->GetVkDescriptorSetLayouts().data(); // Optional
    //pipelineLayoutInfo.pushConstantRangeCount = 1; // Optional
    //pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange; // Optional

    if (__pipelineLayout != VK_NULL_HANDLE) {
        vkDestroyPipelineLayout(LogicalDevice::GetVkDevice(), __pipelineLayout, Allocator::GetVKAllocationCallbacks());
        __pipelineLayout = VK_NULL_HANDLE;
    }
    if (vkCreatePipelineLayout(LogicalDevice::GetVkDevice(), &pipelineLayoutInfo, Allocator::GetVKAllocationCallbacks(), &__pipelineLayout) != VK_SUCCESS)
    {
        vc::Log::Error("Failed to create pipeline layout");
        return vc::Error::Failure;
    }

    // Vertex Input: Describes the format of the vertex data that will be passed to the vertex shader
    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexAttributeDescriptionCount = __attributeDescriptions.size();
    vertexInputInfo.pVertexAttributeDescriptions = __attributeDescriptions.data();
    vertexInputInfo.vertexBindingDescriptionCount = __bindingDescriptions.size();
    vertexInputInfo.pVertexBindingDescriptions = __bindingDescriptions.data();

    // Setting up the pipeline
    VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo = {};
    graphicsPipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    graphicsPipelineCreateInfo.stageCount = static_cast<uint32_t>(shaderStages.size());
    graphicsPipelineCreateInfo.pStages = shaderStages.data();
    graphicsPipelineCreateInfo.pVertexInputState = &vertexInputInfo;
    graphicsPipelineCreateInfo.pInputAssemblyState = &inputAssembly;
    graphicsPipelineCreateInfo.pViewportState = &viewportState;
    graphicsPipelineCreateInfo.pRasterizationState = &__rasterizerCreateInfo;
    graphicsPipelineCreateInfo.pMultisampleState = &__multisamplingCreateInfo;
    graphicsPipelineCreateInfo.pDepthStencilState = &depthStencil;
    graphicsPipelineCreateInfo.pColorBlendState = &colorBlending;
    graphicsPipelineCreateInfo.pDynamicState = &dynamicState;
    graphicsPipelineCreateInfo.layout = __pipelineLayout;
    graphicsPipelineCreateInfo.renderPass = RenderPass::MainRenderPass()->GetVkRenderPass();
    graphicsPipelineCreateInfo.subpass = 0; // Index of the subpass in the render pass where this pipeline will be used
    graphicsPipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE; // Pipeline to derive from: Optional
    //graphicsPipelineCreateInfo.basePipelineIndex = -1; // Optional

    if (__graphicsPipeline != VK_NULL_HANDLE) {
        vkDestroyPipeline(LogicalDevice::GetVkDevice(), __graphicsPipeline, Allocator::GetVKAllocationCallbacks());
        __graphicsPipeline = VK_NULL_HANDLE;
    }
    if (VkResult res = vkCreateGraphicsPipelines(LogicalDevice::GetVkDevice(), VK_NULL_HANDLE, 1, &graphicsPipelineCreateInfo, Allocator::GetVKAllocationCallbacks(), &__graphicsPipeline); res != VK_SUCCESS)
    {
        vc::Log::Error("Failed to create graphics pipeline, error code: %d", res);
        return vc::Error::Failure;
    }

    // Cleaning up shader modules
    for (int i = 0; i < shaderStages.size(); ++i) {
        vkDestroyShaderModule(LogicalDevice::GetVkDevice(), shaderStages[i].module, Allocator::GetVKAllocationCallbacks());
    }

    return vc::Error::Success;
}

VkPipeline VulkanShader::GetPipeline() const
{
    return __graphicsPipeline;
}

VkPipelineLayout VulkanShader::GetPipelineLayout() const
{
    return __pipelineLayout;
}
}
