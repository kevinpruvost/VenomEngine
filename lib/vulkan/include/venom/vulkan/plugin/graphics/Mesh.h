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
class VulkanMesh : public vc::MeshImpl
{
public:
    VulkanMesh();
    ~VulkanMesh();
    void Draw() override;
    vc::Error __LoadMeshFromCurrentData() override;

    vc::Error AddVertexBuffer(const void* data, const uint32_t vertexCount, const uint32_t vertexSize, int binding);
    vc::Error AddIndexBuffer(const void* data, const uint32_t indexCount, const uint32_t indexSize);
    VkBuffer GetVkIndexBuffer() const;
    const IndexBuffer & GetIndexBuffer() const;
    uint32_t GetBindingCount() const;
    uint32_t GetVertexCount() const;
    VkDeviceSize * GetOffsets() const;

    struct BoundVkBuffer
    {
        int binding;
        VkBuffer buffer;
    };
    const std::vector<BoundVkBuffer> & GetVkVertexBuffers() const;

private:
    std::vector<VertexBuffer> __vertexBuffers;
    std::vector<BoundVkBuffer> __vkVertexBuffers;
    std::vector<VkDeviceSize> __offsets;
    IndexBuffer __indexBuffer;
};
}
}
