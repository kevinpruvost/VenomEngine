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

class VENOM_COMMON_API Texture : public GraphicsPluginObject
{
protected:
    Texture();
public:
    ~Texture();

    static Texture * CreateRawTexture();
    static Texture * Create(const std::string & path);

    vc::Error LoadImageFromFile(const char * path);
    vc::Error InitDepthBuffer(int width, int height);

    virtual vc::Error __LoadImage(unsigned char * pixels, int width, int height, int channels) = 0;
    virtual vc::Error __InitDepthBuffer(int width, int height) = 0;
protected:

};

}
}