///
/// Project: VenomEngine
/// @file Shader.cc
/// @date Aug, 22 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/vulkan/plugin/graphics/ShaderPipeline.h>
#include <venom/vulkan/Allocator.h>

#include <fstream>

#include <venom/common/Resources.h>

#include <venom/common/math/Vector.h>
#include <venom/vulkan/LogicalDevice.h>

#include <venom/common/VenomSettings.h>

#include <venom/vulkan/VulkanApplication.h>

namespace venom::vulkan
{
VulkanShaderResource::VulkanShaderResource(vc::GraphicsCachedResourceHolder* h)
    : ShaderResource(h)
    , pipeline(VK_NULL_HANDLE)
    , pipelineType(PipelineType::Graphics)
    , pipelineLayout(VK_NULL_HANDLE)
    , multisamplingCreateInfo{}
    , rasterizerCreateInfo{}
    , shaderDirty(true)
{
        // MultisamplingOption: on of the ways to do antialiasing
    multisamplingCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisamplingCreateInfo.sampleShadingEnable = VK_FALSE;
    multisamplingCreateInfo.rasterizationSamples = static_cast<VkSampleCountFlagBits>(vc::GraphicsSettings::GetActiveSamplesMultisampling());
    //multisampling.minSampleShading = 1.0f; // Optional
    //multisampling.pSampleMask = nullptr; // Optional
    //multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
    //multisampling.alphaToOneEnable = VK_FALSE; // Optional

    // Rasterizer
    rasterizerCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizerCreateInfo.depthClampEnable = VK_FALSE;
    rasterizerCreateInfo.rasterizerDiscardEnable = VK_FALSE;
    rasterizerCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizerCreateInfo.lineWidth = 1.0f;
    // Cull mode determines the type of face culling, back bit means to cull back faces
    rasterizerCreateInfo.cullMode = VK_CULL_MODE_NONE;
    // Front face is the vertex order that is considered front facing
    rasterizerCreateInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    // Depth bias is just adding a constant value or a value proportional to the slope of the polygon
    // Can be used for shadow mapping
    // https://learn.microsoft.com/en-us/windows/win32/direct3d11/d3d10-graphics-programming-guide-output-merger-stage-depth-bias
    rasterizerCreateInfo.depthBiasEnable = VK_FALSE;
    //rasterizer.depthBiasConstantFactor = 0.0f;
    //rasterizer.depthBiasClamp = 0.0f;
    //rasterizer.depthBiasSlopeFactor = 0.0f;

    // Stencil & Depth testing
    depthStencilCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    // Depth test determines if a fragment is drawn based on its depth value
    depthStencilCreateInfo.depthTestEnable = VK_TRUE;
    // Depth write determines if the depth value of a fragment is written to the depth buffer
    depthStencilCreateInfo.depthWriteEnable = VK_TRUE;
    depthStencilCreateInfo.depthCompareOp = VK_COMPARE_OP_LESS;
    depthStencilCreateInfo.depthBoundsTestEnable = VK_FALSE;
    depthStencilCreateInfo.minDepthBounds = 0.0f; // Optional
    depthStencilCreateInfo.maxDepthBounds = 100.0f; // Optional
    depthStencilCreateInfo.stencilTestEnable = VK_FALSE;
    depthStencilCreateInfo.front = {}; // Optional
    depthStencilCreateInfo.back = {}; // Optional
}

VulkanShaderResource::~VulkanShaderResource()
{
    DestroyShaderModules();
    if (pipeline != VK_NULL_HANDLE)
        vkDestroyPipeline(LogicalDevice::GetVkDevice(), pipeline, Allocator::GetVKAllocationCallbacks());
    if (pipelineLayout != VK_NULL_HANDLE)
        vkDestroyPipelineLayout(LogicalDevice::GetVkDevice(), pipelineLayout, Allocator::GetVKAllocationCallbacks());
}

void VulkanShaderResource::DestroyShaderModules()
{
    for (int i = 0; i < shaderStages.size(); ++i) {
        vkDestroyShaderModule(LogicalDevice::GetVkDevice(), shaderStages[i].module, Allocator::GetVKAllocationCallbacks());
    }
    shaderStages.clear();
}

VulkanShaderPipeline::VulkanShaderPipeline()
{
    _resource.reset(new VulkanShaderResource(this));
}

VulkanShaderPipeline::~VulkanShaderPipeline()
{
}

VulkanShaderPipeline::VulkanShaderPipeline(VulkanShaderPipeline&& other) noexcept
{
    _resource = std::move(other._resource);
}

VulkanShaderPipeline& VulkanShaderPipeline::operator=(VulkanShaderPipeline&& other) noexcept
{
    if (this != &other) {
        _resource = std::move(other._resource);
    }
    return *this;
}

vc::Error VulkanShaderPipeline::_LoadShader(const std::string& path)
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
                _resource->As<VulkanShaderResource>()->shaderPaths.emplace_back(std::move(shaderPath));
            }
        }
    }
    if (_resource->As<VulkanShaderResource>()->shaderPaths.empty()) return vc::Error::Failure;
    return LoadShaders();
}

void VulkanShaderPipeline::_SetLineWidth(const float width)
{
    if (_resource->As<VulkanShaderResource>()->rasterizerCreateInfo.lineWidth == width) return;
    _resource->As<VulkanShaderResource>()->rasterizerCreateInfo.lineWidth = width;
    _resource->As<VulkanShaderResource>()->shaderDirty = true;
}

void VulkanShaderPipeline::SetMultiSamplingCount(const int samples)
{
    if (_resource->As<VulkanShaderResource>()->multisamplingCreateInfo.rasterizationSamples == static_cast<VkSampleCountFlagBits>(samples)) return;
    _resource->As<VulkanShaderResource>()->multisamplingCreateInfo.rasterizationSamples = static_cast<VkSampleCountFlagBits>(samples);
    _resource->As<VulkanShaderResource>()->shaderDirty = true;
}

void VulkanShaderPipeline::_SetDepthTest(const bool enable)
{
    if (_resource->As<VulkanShaderResource>()->depthStencilCreateInfo.depthTestEnable == enable) return;
    _resource->As<VulkanShaderResource>()->depthStencilCreateInfo.depthTestEnable = enable;
    _resource->As<VulkanShaderResource>()->shaderDirty = true;
}

void VulkanShaderPipeline::_SetDepthWrite(const bool enable)
{
    if (_resource->As<VulkanShaderResource>()->depthStencilCreateInfo.depthWriteEnable == enable) return;
    _resource->As<VulkanShaderResource>()->depthStencilCreateInfo.depthWriteEnable = enable;
    _resource->As<VulkanShaderResource>()->shaderDirty = true;
}

vc::Error VulkanShaderPipeline::_ReloadShader()
{
    venom_assert(_renderingPipelineType != vc::RenderingPipelineType::None, "Rendering Pipeline Type is not set");
    venom_assert(_renderingPipelineIndex != std::numeric_limits<uint32_t>::max(), "Rendering Pipeline Index is not set");

    if (_resource->As<VulkanShaderResource>()->shaderDirty == false || _resource->As<VulkanShaderResource>()->shaderStages.empty()) return vc::Error::Success;

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

    if (_resource->As<VulkanShaderResource>()->pipelineLayout != VK_NULL_HANDLE) {
        vkDestroyPipelineLayout(LogicalDevice::GetVkDevice(), _resource->As<VulkanShaderResource>()->pipelineLayout, Allocator::GetVKAllocationCallbacks());
        _resource->As<VulkanShaderResource>()->pipelineLayout = VK_NULL_HANDLE;
    }
    if (vkCreatePipelineLayout(LogicalDevice::GetVkDevice(), &pipelineLayoutInfo, Allocator::GetVKAllocationCallbacks(), &_resource->As<VulkanShaderResource>()->pipelineLayout) != VK_SUCCESS)
    {
        vc::Log::Error("Failed to create pipeline layout");
        return vc::Error::Failure;
    }
    
    // Destroying the pipeline if it exists
    if (_resource->As<VulkanShaderResource>()->pipeline != VK_NULL_HANDLE) {
        vkDestroyPipeline(LogicalDevice::GetVkDevice(), _resource->As<VulkanShaderResource>()->pipeline, Allocator::GetVKAllocationCallbacks());
        _resource->As<VulkanShaderResource>()->pipeline = VK_NULL_HANDLE;
    }

    // Setting up the pipeline
    if (_resource->As<VulkanShaderResource>()->pipelineType == PipelineType::Graphics)
    {
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

        // Color blending
        VkPipelineColorBlendAttachmentState colorBlendAttachment{};
        colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachment.blendEnable = VK_TRUE;
        colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
        colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
        colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
        colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

        vc::Vector<VkPipelineColorBlendAttachmentState> colorBlendAttachments(RenderPass::GetRenderPass(_renderingPipelineType)->GetSubpassDescriptions()[_renderingPipelineIndex].colorAttachmentCount, colorBlendAttachment);

        VkPipelineColorBlendStateCreateInfo colorBlending{};
        colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending.logicOpEnable = VK_FALSE;
        colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
        colorBlending.attachmentCount = colorBlendAttachments.size();
        colorBlending.pAttachments = colorBlendAttachments.data();
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
        
        // Vertex Input: Describes the format of the vertex data that will be passed to the vertex shader
        VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexAttributeDescriptionCount = _resource->As<VulkanShaderResource>()->attributeDescriptions.size();
        vertexInputInfo.pVertexAttributeDescriptions = _resource->As<VulkanShaderResource>()->attributeDescriptions.data();
        vertexInputInfo.vertexBindingDescriptionCount = _resource->As<VulkanShaderResource>()->bindingDescriptions.size();
        vertexInputInfo.pVertexBindingDescriptions = _resource->As<VulkanShaderResource>()->bindingDescriptions.data();
        
        VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo = {};
        graphicsPipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        graphicsPipelineCreateInfo.stageCount = static_cast<uint32_t>(_resource->As<VulkanShaderResource>()->shaderStages.size());
        graphicsPipelineCreateInfo.pStages = _resource->As<VulkanShaderResource>()->shaderStages.data();
        graphicsPipelineCreateInfo.pVertexInputState = &vertexInputInfo;
        graphicsPipelineCreateInfo.pInputAssemblyState = &inputAssembly;
        graphicsPipelineCreateInfo.pViewportState = &viewportState;
        graphicsPipelineCreateInfo.pRasterizationState = &_resource->As<VulkanShaderResource>()->rasterizerCreateInfo;
        graphicsPipelineCreateInfo.pMultisampleState = &_resource->As<VulkanShaderResource>()->multisamplingCreateInfo;
        graphicsPipelineCreateInfo.pDepthStencilState = &_resource->As<VulkanShaderResource>()->depthStencilCreateInfo;
        graphicsPipelineCreateInfo.pColorBlendState = &colorBlending;
        graphicsPipelineCreateInfo.pDynamicState = &dynamicState;
        graphicsPipelineCreateInfo.layout = _resource->As<VulkanShaderResource>()->pipelineLayout;
        graphicsPipelineCreateInfo.renderPass = RenderPass::GetRenderPass(_renderingPipelineType)->GetVkRenderPass();
        graphicsPipelineCreateInfo.subpass = _renderingPipelineIndex; // Index of the subpass in the render pass where this pipeline will be used
        graphicsPipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE; // Pipeline to derive from: Optional
        //graphicsPipelineCreateInfo.basePipelineIndex = -1; // Optional

        if (VkResult res = vkCreateGraphicsPipelines(LogicalDevice::GetVkDevice(), VK_NULL_HANDLE, 1, &graphicsPipelineCreateInfo, Allocator::GetVKAllocationCallbacks(), &_resource->As<VulkanShaderResource>()->pipeline); res != VK_SUCCESS)
        {
            vc::Log::Error("Failed to create graphics pipeline, error code: %d", res);
            return vc::Error::Failure;
        }
    }
    else if (_resource->As<VulkanShaderResource>()->pipelineType == PipelineType::Compute)
    {
        VkComputePipelineCreateInfo computePipelineCreateInfo = {};
        computePipelineCreateInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
        computePipelineCreateInfo.flags = 0;
        computePipelineCreateInfo.stage = _resource->As<VulkanShaderResource>()->shaderStages[0];
        computePipelineCreateInfo.layout = _resource->As<VulkanShaderResource>()->pipelineLayout;

        if (VkResult res = vkCreateComputePipelines(LogicalDevice::GetVkDevice(), VK_NULL_HANDLE, 1, &computePipelineCreateInfo, Allocator::GetVKAllocationCallbacks(), &_resource->As<VulkanShaderResource>()->pipeline); res != VK_SUCCESS)
        {
            vc::Log::Error("Failed to create compute pipeline, error code: %d", res);
            return vc::Error::Failure;
        }
    }
    _resource->As<VulkanShaderResource>()->shaderDirty = false;
    return vc::Error::Success;
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

void VulkanShaderPipeline::_AddVertexBufferToLayout(const uint32_t vertexSize, const uint32_t binding, const uint32_t location,
    const uint32_t offset, const vc::ShaderVertexFormat format)
{
    _resource->As<VulkanShaderResource>()->bindingDescriptions.push_back({
        .binding = binding,
        .stride = vertexSize,
        .inputRate = VK_VERTEX_INPUT_RATE_VERTEX
    });
    VkFormat vkFormat = GetVkFormatFromShaderVertexFormat(format);
    _resource->As<VulkanShaderResource>()->attributeDescriptions.push_back({
        .location = location,
        .binding = binding,
        .format = vkFormat,
        .offset = offset
    });
}

vc::Error VulkanShaderPipeline::LoadShader(const std::string& shaderPath, VkPipelineShaderStageCreateInfo* pipelineCreateInfo)
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
    else if (shaderPath.find("comp") != std::string::npos || shaderPath.find("cs") != std::string::npos || shaderPath.find("compute") != std::string::npos) {
        pipelineCreateInfo->stage = VK_SHADER_STAGE_COMPUTE_BIT;
        _resource->As<VulkanShaderResource>()->pipelineType = PipelineType::Compute;
    } else if (shaderPath.find("geom") != std::string::npos)
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

vc::Error VulkanShaderPipeline::LoadShaders()
{
    // Loading every shader
    _resource->As<VulkanShaderResource>()->DestroyShaderModules();
    _resource->As<VulkanShaderResource>()->shaderStages.resize(_resource->As<VulkanShaderResource>()->shaderPaths.size(), VkPipelineShaderStageCreateInfo{});
    for (int i = 0; i < _resource->As<VulkanShaderResource>()->shaderPaths.size(); ++i)
    {
        if (LoadShader(_resource->As<VulkanShaderResource>()->shaderPaths[i], &_resource->As<VulkanShaderResource>()->shaderStages[i]) != vc::Error::Success)
        {
            vc::Log::Error("Failed to load shader: %s", _resource->As<VulkanShaderResource>()->shaderPaths[i].c_str());
            return vc::Error::Failure;
        }
    }
    // Check if duplicate stages
    for (int i = 0; i < _resource->As<VulkanShaderResource>()->shaderStages.size(); ++i) {
        for (int j = i + 1; j < _resource->As<VulkanShaderResource>()->shaderStages.size(); ++j)
        {
            if (_resource->As<VulkanShaderResource>()->shaderStages[i].stage == _resource->As<VulkanShaderResource>()->shaderStages[j].stage)
            {
                vc::Log::Error("Duplicate shader stages: [%s] | [%s]", _resource->As<VulkanShaderResource>()->shaderPaths[i].c_str(), _resource->As<VulkanShaderResource>()->shaderPaths[j].c_str());
                for (int k = 0; k < _resource->As<VulkanShaderResource>()->shaderStages.size(); ++k) {
                    vkDestroyShaderModule(LogicalDevice::GetVkDevice(), _resource->As<VulkanShaderResource>()->shaderStages[k].module, Allocator::GetVKAllocationCallbacks());
                }
                return vc::Error::Failure;
            }
        }
    }

    _resource->As<VulkanShaderResource>()->shaderDirty = true;
    return _ReloadShader();
}

VkPipeline VulkanShaderPipeline::GetPipeline() const
{
    return _resource->As<VulkanShaderResource>()->pipeline;
}

VkPipelineLayout VulkanShaderPipeline::GetPipelineLayout() const
{
    return _resource->As<VulkanShaderResource>()->pipelineLayout;
}
}
