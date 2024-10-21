///
/// Project: VenomEngine
/// @file Texture.h
/// @date Sep, 17 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/common/plugin/graphics/GraphicsPlugin.h>

namespace venom
{
namespace common
{
class Texture;

class VENOM_COMMON_API TextureResource : public GraphicsCachedResource
{
public:
};

class VENOM_COMMON_API TextureImpl : public PluginObjectImpl, public GraphicsPluginObject, public GraphicsCachedResourceHolder
{
public:
    TextureImpl();
    virtual ~TextureImpl() = default;

    vc::Error LoadImageFromFile(const char * path);
    vc::Error InitDepthBuffer(int width, int height);
    virtual bool HasTexture() const = 0;
protected:
    virtual vc::Error _LoadImage(unsigned char * pixels, int width, int height, int channels) = 0;
    virtual vc::Error _InitDepthBuffer(int width, int height) = 0;
private:
    friend class Texture;
};

class VENOM_COMMON_API Texture : public PluginObjectImplWrapper
{
public:
    Texture();
    Texture(const char * path);
    ~Texture();

    inline vc::Error LoadImageFromFile(const char * path) { return _impl->As<TextureImpl>()->LoadImageFromFile(path); }
    inline vc::Error InitDepthBuffer(int width, int height) { return _impl->As<TextureImpl>()->InitDepthBuffer(width, height); }
    inline bool HasTexture() const { return _impl->As<TextureImpl>()->HasTexture(); }
};

}
}