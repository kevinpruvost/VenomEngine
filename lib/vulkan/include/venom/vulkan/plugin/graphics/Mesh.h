///
/// Project: VenomEngine
/// @file Mesh.h
/// @date Aug, 25 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/vulkan/Debug.h>
#include <venom/common/math/Vector.h>
#include <venom/common/plugin/graphics/Mesh.h>
#include <venom/vulkan/VertexBuffer.h>

namespace venom
{
namespace vulkan
{
class VulkanMesh : public vc::Mesh
{
public:
    VulkanMesh();
    ~VulkanMesh();
    void Draw() override;

    vc::Error AddVertexBuffer(const void* data, const uint32_t vertexCount, const uint32_t vertexSize);
    vc::Error AddIndexBuffer(const void* data, const uint32_t indexCount, const uint32_t indexSize);
    const VkBuffer * GetVkVertexBuffers() const;
    VkBuffer GetVkIndexBuffer() const;
    const IndexBuffer & GetIndexBuffer() const;
    uint32_t GetBindingCount() const;
    uint32_t GetVertexCount() const;

private:
    std::vector<VertexBuffer> __vertexBuffers;
    std::vector<VkBuffer> __vkVertexBuffers;
    IndexBuffer __indexBuffer;
};
}
}
