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

    static Texture * Create();

    vc::Error LoadImageFromFile(const char * path);

    virtual vc::Error __LoadImage(unsigned char * pixels, int width, int height, int channels) = 0;
protected:

};

}
}