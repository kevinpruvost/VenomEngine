///
/// Project: VenomEngineWorkspace
/// @file ShaderResourceTable.cc
/// @date Oct, 22 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/common/plugin/graphics/ShaderResourceTable.h>
#include <venom/common/Ptr.h>

#include <stack>

namespace venom
{
namespace common
{

#ifdef VENOM_EXTERNAL_PACKED_MODEL_MATRIX
// Model matrix buffers

class ExternalModelMatrixManager
{
public:
    ExternalModelMatrixManager()
    {
        for (int i = VENOM_MAX_ENTITIES - 1; i >= 0; --i) {
            freeBuffers.push(i);
        }
    }

    inline vcm::Mat4 * GetModelMatrixBuffer()
    {
        venom_assert(!freeBuffers.empty(), "No more model matrix buffers available");

        const int id = freeBuffers.top();
        freeBuffers.pop();
        return &modelMatrixBuffers[id];
    }

    inline void ReleaseModelMatrixBuffer(const vcm::Mat4 * mat)
    {
        const long id = mat - modelMatrixBuffers.data();
        //DEBUG_PRINT("Releasing model matrix buffer %ld", id);
        freeBuffers.push(static_cast<int>(id));
    }

    inline long GetModelMatrixBufferId(const vcm::Mat4 * mat)
    {
        return mat - modelMatrixBuffers.data();
    }

    inline vcm::Mat4 * GetAllModelMatrixBuffer()
    {
        return modelMatrixBuffers.data();
    }

    std::array<vcm::Mat4, VENOM_MAX_ENTITIES> modelMatrixBuffers;
    std::stack<int> freeBuffers;
};
static UPtr<ExternalModelMatrixManager> s_modelMatrixManager(new ExternalModelMatrixManager());

vcm::Mat4* ShaderResourceTable::GetAllModelMatrixBuffer()
{
    return s_modelMatrixManager->GetAllModelMatrixBuffer();
}

vcm::Mat4* ShaderResourceTable::GetModelMatrixBuffer()
{
    return s_modelMatrixManager->GetModelMatrixBuffer();
}

void ShaderResourceTable::ReleaseModelMatrixBuffer(const vcm::Mat4* mat)
{
    s_modelMatrixManager->ReleaseModelMatrixBuffer(mat);
}

int ShaderResourceTable::GetModelMatrixBufferId(const vcm::Mat4* mat)
{
    return static_cast<int>(s_modelMatrixManager->GetModelMatrixBufferId(mat));
}

#endif

#ifdef VENOM_BINDLESS_TEXTURES

class BindlessTexturesIdManager
{
public:
    BindlessTexturesIdManager()
    {
    }

    inline int GetTextureId()
    {
        venom_assert(!freeBuffers.empty(), "No more texture buffers available or SetMaxTextures has not been called");
        const int id = freeBuffers.top();
        freeBuffers.pop();
        return id;
    }

    inline void ReleaseTexture(const int id)
    {
        freeBuffers.push(static_cast<int>(id));
    }

    inline void SetMaxTextures(uint32_t maxTextures)
    {
        freeBuffers = std::stack<int>();
        for (int i = maxTextures - 1; i >= 0; --i) {
            freeBuffers.push(i);
        }
    }

    std::stack<int> freeBuffers;
};
static UPtr<BindlessTexturesIdManager> s_bindlessTextureManager(new BindlessTexturesIdManager());


int ShaderResourceTable::BindTexture()
{
    return s_bindlessTextureManager->GetTextureId();
}

void ShaderResourceTable::UnbindTexture(int id)
{
    s_bindlessTextureManager->ReleaseTexture(id);
}

int ShaderResourceTable::__maxTextures = 0;
void ShaderResourceTable::SetMaxTextures(uint32_t maxTextures)
{
    s_bindlessTextureManager->SetMaxTextures(maxTextures);
    __maxTextures = maxTextures;
}

#endif
}
}
