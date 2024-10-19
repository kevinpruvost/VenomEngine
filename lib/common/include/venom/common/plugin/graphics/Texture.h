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
class VENOM_COMMON_API TextureImpl : public PluginObjectImpl
{
public:
    TextureImpl();
    virtual ~TextureImpl() = default;

    vc::Error LoadImageFromFile(const char * path);
    vc::Error InitDepthBuffer(int width, int height);

protected:
    virtual vc::Error __LoadImage(unsigned char * pixels, int width, int height, int channels) = 0;
    virtual vc::Error __InitDepthBuffer(int width, int height) = 0;
};

class VENOM_COMMON_API Texture : public GraphicsPluginObject
{
protected:
    Texture();
public:
    ~Texture();

    static Texture * CreateRawTexture();
    static Texture * Create(const std::string & path);

    inline vc::Error LoadImageFromFile(const char * path) { return _impl->As<TextureImpl>()->LoadImageFromFile(path); }
    inline vc::Error InitDepthBuffer(int width, int height) { return _impl->As<TextureImpl>()->InitDepthBuffer(width, height); }
};

}
}