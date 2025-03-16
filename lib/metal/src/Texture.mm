///
/// Project: VenomEngine
/// @file Texture.cc
/// @date Sep, 17 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/metal/plugin/graphics/Texture.h>

#include "venom/common/plugin/graphics/GraphicsSettings.h"

namespace venom
{
namespace metal
{
MetalTextureResource::MetalTextureResource()
{
}

MetalTexture::MetalTexture()
    : TextureImpl()
{
    _ResetResource();
}

MetalTexture::~MetalTexture()
{
}

static MetalTexture * s_dummyTexture = nullptr;
MetalTexture* MetalTexture::GetDummyTexture()
{
    return s_dummyTexture;
}

void MetalTexture::SetDummyTexture(MetalTexture* texture)
{
    s_dummyTexture = texture;
}

void MetalTexture::_ResetResource()
{
    _resource.reset(new MetalTextureResource());
}

vc::Error MetalTexture::LoadImage(unsigned char* pixels, int width, int height, int channels)
{
    return vc::Error::Success;
}

vc::Error MetalTexture::LoadImageRGBA(unsigned char* pixels, int width, int height, int channels)
{
    return vc::Error::Success;
}

vc::Error MetalTexture::LoadImage(uint16_t* pixels, int width, int height, int channels)
{
    return vc::Error::Success;
}

vc::Error MetalTexture::_InitDepthBuffer(int width, int height)
{
    return vc::Error::Success;
}

vc::Error MetalTexture::_CreateAttachment(int width, int height, int imageCount, vc::ShaderVertexFormat format)
{
    switch (format) {
        case vc::ShaderVertexFormat::Float:
            break;
        case vc::ShaderVertexFormat::Vec2:
            break;
        case vc::ShaderVertexFormat::Vec3:
            break;
        case vc::ShaderVertexFormat::Vec4:
            break;
        default:
            vc::Log::Error("Unsupported format for attachment");
            return vc::Error::Failure;
    };

    return vc::Error::Success;
}

vc::Error MetalTexture::_CreateReadWriteTexture(int width, int height, vc::ShaderVertexFormat format, int mipLevels, int arrayLayers)
{
    switch (format) {
        case vc::ShaderVertexFormat::Float:
            break;
        case vc::ShaderVertexFormat::Vec2:
            break;
        case vc::ShaderVertexFormat::Vec3:
            break;
        case vc::ShaderVertexFormat::Vec4:
            break;
        default:
            vc::Log::Error("Unsupported format for read write texture");
            return vc::Error::Failure;
    };
    return vc::Error::Success;
}

vc::Error MetalTexture::_CreateShadowMaps(int dimension)
{
    return vc::Error::Success;
}

vc::Error MetalTexture::_CreateShadowCubeMaps(int dimension)
{
    return vc::Error::Success;
}

vc::Error MetalTexture::_SaveImageToFile(const char *path)
{
    return vc::Error::Success;
}

vc::Error MetalTexture::_SetMemoryAccess(const vc::TextureMemoryAccess access)
{
    if (_textureUsage & vc::TextureUsage::Storage) {
        switch (access) {
            case vc::TextureMemoryAccess::ReadOnly:
                break;
            case vc::TextureMemoryAccess::ReadWrite:
                break;
            default:
                vc::Log::Error("Unsupported memory access");
                return vc::Error::Failure;
        };
    }
    return vc::Error::Success;
}

vc::Error MetalTexture::MetalGUITexture::_LoadTextureToGUI(vc::TextureImpl* impl, void** ptrToGuiTextureId)
{
//    const MetalTexture * vulkanTexture = impl->ConstAs<MetalTexture>();
//    *ptrToGuiTextureId = ImGui_ImplMetal_AddTexture(Sampler::GetMainSampler()->GetVkSampler(), vulkanTexture->GetImageView().GetVkImageView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);//, vulkanTexture->GetImage().GetLayout());
//    if (!*ptrToGuiTextureId) {
//        vc::Log::Error("Failed to load texture to GUI");
//        return vc::Error::Failure;
//    }
    return vc::Error::Success;
}

vc::Error MetalTexture::MetalGUITexture::_UnloadTextureFromGUI(void* guiTextureId)
{
//    VkDescriptorSet descriptorSet = static_cast<VkDescriptorSet>(guiTextureId);
//    ImGui_ImplMetal_RemoveTexture(descriptorSet);
    return vc::Error::Success;
}

vc::TextureImpl::GUITexture* MetalTexture::_NewGuiTextureInstance()
{
    return new MetalGUITexture();
}

int MetalTexture::GetHeight() const
{
    return 0;
}

int MetalTexture::GetWidth() const
{
    return 0;
}

void MetalTexture::SetDimensions(int width, int height)
{
    if (width == GetWidth() && height == GetHeight())
        return;

    // TODO: Implement
    // Moves image here to destroy after
    // Image image = std::move(GetImage());
    // ImageView imageView = std::move(GetImageView());
}
}
}
