///
/// Project: VenomEngineWorkspace
/// @file ShaderPipeline.h
/// @date Oct, 22 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/common/plugin/graphics/GraphicsPluginObject.h>
#include <venom/common/plugin/graphics/RenderingPipelineType.h>

namespace venom
{
namespace common
{
class VENOM_COMMON_API ShaderResource : public GraphicsCachedResource
{
public:
    ShaderResource(GraphicsCachedResourceHolder * holder);

    Vector<String> shaderPaths;
};

enum class ShaderVertexFormat
{
    Float,
    Vec2,
    Vec3,
    Vec4,
    Int,
    IVec2,
    IVec3,
    IVec4,
    Uint,
    UVec2,
    UVec3,
    UVec4,
    Mat2,
    Mat3,
    Mat4
};

class VENOM_COMMON_API ShaderPipelineImpl : public PluginObjectImpl, public GraphicsPluginObject, public GraphicsCachedResourceHolder
{
public:
    ShaderPipelineImpl();
    virtual ~ShaderPipelineImpl() = default;

    /**
     * @brief Load a shader from a base path (e.g. ./shader_mesh to load ./shader_mesh.vert and ./shader_mesh.frag)
     * Loading is not done when this function is called, only paths are set
     * @param path
     * @return error
     */
    vc::Error LoadShaderFromFile(const std::string & path);

    struct VertexBufferLayout
    {
        const ShaderVertexFormat format;
        const uint32_t binding;
        const uint32_t location;
        const uint32_t offset;
    };
    /**
     * @brief Add a vertex buffer to the layout
     * @param format Format of the vertex buffer
     * @param binding Binding of the vertex buffer (location where data comes from in the buffer)
     * @param location Location of the vertex buffer (location where data goes to in the shader)
     * @param offset Offset of the vertex buffer
     */
    void AddVertexBufferToLayout(const ShaderVertexFormat format, const uint32_t binding, const uint32_t location, const uint32_t offset);
    /**
     * @brief Add a vertex buffer to the layout
     * @param layout Layout of the vertex buffer
     */
    void AddVertexBufferToLayout(const VertexBufferLayout & layout);
    /**
     * @brief Add a vertex buffer to the layout
     * @param layouts Layouts of the vertex buffer
     */
    void AddVertexBufferToLayout(const std::vector<VertexBufferLayout> & layouts);

    virtual void SetMultiSamplingCount(const int samples) = 0;
    inline vc::Error SetLineWidth(const float width) { _SetLineWidth(width); return _ReloadShaderAfterSettings(); }
    inline vc::Error SetDepthTest(const bool enable) { _SetDepthTest(enable); return _ReloadShaderAfterSettings(); }
    inline vc::Error SetDepthWrite(const bool enable) { _SetDepthWrite(enable); return _ReloadShaderAfterSettings(); }

    inline void SetRenderingPipelineType(const RenderingPipelineType type) { _renderingPipelineType = type; }
    inline void SetRenderingPipelineIndex(const uint32_t index) { _renderingPipelineIndex = index; }

protected:
    virtual void _SetLineWidth(const float width) = 0;
    virtual void _SetDepthTest(const bool enable) = 0;
    virtual void _SetDepthWrite(const bool enable) = 0;
    virtual vc::Error _LoadShader(const std::string & path) = 0;
    virtual void _AddVertexBufferToLayout(const uint32_t vertexSize, const uint32_t binding, const uint32_t location, const uint32_t offset, const ShaderVertexFormat format) = 0;

    virtual vc::Error _ReloadShader() = 0;
    inline vc::Error _ReloadShaderAfterSettings() {
        if (_loaded) return _ReloadShader();
        return vc::Error::Success;
    }
protected:
    RenderingPipelineType _renderingPipelineType;
    uint32_t _renderingPipelineIndex;
    bool _loaded;
};

class VENOM_COMMON_API ShaderPipeline : public PluginObjectImplWrapper
{
public:
    ShaderPipeline();
    ShaderPipeline(const char * path);
    ~ShaderPipeline();

    inline void SetRenderingPipelineIndex(const uint32_t index) { _impl->As<ShaderPipelineImpl>()->SetRenderingPipelineIndex(index); }
    inline void SetRenderingPipelineType(const RenderingPipelineType type) { _impl->As<ShaderPipelineImpl>()->SetRenderingPipelineType(type); }
    inline vc::Error LoadShaderFromFile(const char * path) { return _impl->As<ShaderPipelineImpl>()->LoadShaderFromFile(path); }
    inline void AddVertexBufferToLayout(const ShaderVertexFormat format, const uint32_t binding, const uint32_t location, const uint32_t offset) { _impl->As<ShaderPipelineImpl>()->AddVertexBufferToLayout(format, binding, location, offset); }
    inline void AddVertexBufferToLayout(const ShaderPipelineImpl::VertexBufferLayout & layout) { _impl->As<ShaderPipelineImpl>()->AddVertexBufferToLayout(layout); }
    inline void AddVertexBufferToLayout(const std::vector<ShaderPipelineImpl::VertexBufferLayout> & layouts) { _impl->As<ShaderPipelineImpl>()->AddVertexBufferToLayout(layouts); }
    inline void SetLineWidth(const float width) { _impl->As<ShaderPipelineImpl>()->SetLineWidth(width); }
    inline void SetDepthTest(const bool enable) { _impl->As<ShaderPipelineImpl>()->SetDepthTest(enable); }
    inline void SetDepthWrite(const bool enable) { _impl->As<ShaderPipelineImpl>()->SetDepthWrite(enable); }
};
}
}
