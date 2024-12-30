///
/// Project: VenomEngineWorkspace
/// @file GUI.h
/// @date Nov, 07 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once
#include <venom/common/plugin/graphics/GraphicsPluginObject.h>

#include <venom/common/math/Matrix.h>
#include <venom/common/plugin/graphics/GUI_Enum.h>

#include <venom/common/plugin/graphics/RenderTarget.h>
#include <IconsMaterialSymbols.h>

namespace venom
{
namespace common
{
class GraphicsApplication;
class VenomEngine;

class Model;
class Texture;

typedef void(*GUIDrawCallback)();
typedef void * GUIViewport;
class VENOM_COMMON_API GUI : public GraphicsPluginObject
{
public:
    GUI();
    ~GUI() override;

    void SetGraphicsApplication(GraphicsApplication * app);

    virtual vc::Error Initialize() = 0;
    vc::Error Reset();

    inline void DrawCallback() { if (s_guiDrawCallback) s_guiDrawCallback(); }
    static inline void SetGUIDrawCallback(GUIDrawCallback guiDrawCallback) { s_guiDrawCallback = guiDrawCallback; }

    void Render();

    static inline GUI * Get() { return s_gui; }

    static void AddFont(const char * fontPath, float fontSize, const uint16_t * glyphRanges);
    static void AddFont(const char * fontPath, float fontSize);
    static void ClearFonts();

    static inline void SetNextWindowPos(const vcm::Vec2 & pos, GUICond cond = GUICondBits::GUICond_None, const vcm::Vec2 & pivot = vcm::Vec2(0, 0)) { s_gui->_SetNextWindowPos(pos, cond, pivot); }
    static inline void SetNextWindowSize(const vcm::Vec2 & size, GUICond cond = GUICondBits::GUICond_None) { s_gui->_SetNextWindowSize(size, cond); }
    static inline void SetNextWindowViewport(GUIViewport viewport) { s_gui->_SetNextWindowViewport(viewport); }

    static inline vcm::Vec2 GetContentRegionAvail() { return s_gui->_GetContentRegionAvail(); }

    static inline GUIViewport GetMainViewport() { return s_gui->_GetMainViewport(); }
    static inline vcm::Vec2 GetWindowSize() { return s_gui->_GetWindowSize(); }
    static inline vcm::Vec2 GetWindowPos() { return s_gui->_GetWindowPos(); }
    static inline void NewFrame() { s_gui->_NewFrame(); }
    static inline void Begin(const char * name, bool * p_open = nullptr, GUIWindowFlags flags = 0) { s_gui->_Begin(name, p_open, flags); }
    static inline void End() { s_gui->_End(); }
    static inline void Text(const char* fmt, ...) { va_list args; va_start(args, fmt); s_gui->_Text(fmt, args); va_end(args); }
    static inline void TextColored(const vcm::Vec4 & col, const char* fmt, ...) { va_list args; va_start(args, fmt); s_gui->_TextColored(col, fmt, args); va_end(args); }
    static inline void LabelText(const char* label, const char* fmt, ...) { va_list args; va_start(args, fmt); s_gui->_LabelText(label, fmt, args); va_end(args); }

    static inline void Image(const vc::Texture * texture, const vcm::Vec2 & size, bool centering = true) { s_gui->_Image(texture, size, centering); }
    static inline void Image(const vc::RenderTarget * renderTarget, const vcm::Vec2 & size, bool centering = true) { s_gui->_Image(renderTarget->GetTexture(), size, centering); }

    static inline bool InputText(const char* label, char* buf, size_t buf_size, GUIInputTextFlags flags = 0) { return s_gui->_InputText(label, buf, buf_size, flags); }

    static inline bool TreeNode(const char* label) { return s_gui->_TreeNode(label); }
    static inline void TreePop() { s_gui->_TreePop(); }
    static inline void TreePush(const char* str_id) { s_gui->_TreePush(str_id); }

    static inline void SeparatorText(const char* text) { s_gui->_SeparatorText(text); }
    static inline void Separator() { s_gui->_Separator(); }
    static inline void Spacing() { s_gui->_Spacing(); }

    static inline bool SliderFloat(const char* label, float* v, float v_min, float v_max, const char* format = "%.3f") { return s_gui->_SliderFloat(label, v, v_min, v_max, format); }
    static inline bool SliderFloat3(const char* label, float v[3], float v_min, float v_max, const char* format = "%.3f") { return s_gui->_SliderFloat3(label, v, v_min, v_max, format); }

    static inline bool InputFloat(const char* label, float* v, float step = 0.0f, float step_fast = 0.0f, const char* format = "%.3f", GUIColorEditFlags flags = 0) { return s_gui->_InputFloat(label, v, step, step_fast, format, flags); }
    static inline bool InputFloat3(const char* label, float v[3], const char* format = "%.3f", GUIColorEditFlags flags = 0) { return s_gui->_InputFloat3(label, v, format, flags); }

    static inline void ColorEdit3(const char* label, float col[3], GUIColorEditFlags flags = 0) { s_gui->_ColorEdit3(label, col, flags); }

    static inline bool CollapsingHeader(const char* label, GUITreeNodeFlags flags = 0) { return s_gui->_CollapsingHeader(label, flags); }

    static inline bool Button(const char* label, const vcm::Vec2 & size = vcm::Vec2(0, 0)) { return s_gui->_Button(label, size); }
    static inline bool Checkbox(const char* label, bool* v) { return s_gui->_Checkbox(label, v); }
    static inline void ProgressBar(float fraction, const vcm::Vec2 & size_arg = vcm::Vec2(-1, 0), const char* overlay = nullptr) { s_gui->_ProgressBar(fraction, size_arg, overlay); }

    static inline bool Selectable(const char* label, bool selected, GUISelectableFlags flags = 0, const vcm::Vec2 & size = vcm::Vec2(0, 0)) { return s_gui->_Selectable(label, selected, flags, size); }

    static inline bool BeginCombo(const char* label, const char* preview_value, GUIComboFlags flags = 0) { return s_gui->_BeginCombo(label, preview_value, flags); }
    static inline void EndCombo() { s_gui->_EndCombo(); }

    static inline bool BeginMenu(const char* label, bool enabled = true) { return s_gui->_BeginMenu(label, enabled); }
    static inline void EndMenu() { s_gui->_EndMenu(); }

    static inline bool BeginMainMenuBar() { return s_gui->_BeginMainMenuBar(); }
    static inline void EndMainMenuBar() { s_gui->_EndMainMenuBar(); }

    static inline bool BeginMenuBar() { return s_gui->_BeginMenuBar(); }
    static inline void EndMenuBar() { s_gui->_EndMenuBar(); }

    static inline bool BeginChild(const char* str_id, const vcm::Vec2 & size, GUIChildFlags childFlags, GUIWindowFlags extraFlags = 0) { return s_gui->_BeginChild(str_id, size, childFlags, extraFlags); }
    static inline void EndChild() { s_gui->_EndChild(); }

    static inline bool MenuItem(const char* str, const char* text = nullptr) { return s_gui->_MenuItem(str, text); }

    static inline void SetNextItemWidth(float item_width) { s_gui->_SetNextItemWidth(item_width); }
    static inline void SetItemDefaultFocus() { s_gui->_SetItemDefaultFocus(); }

    static inline void SameLine(float offset_from_start_x = 0.0f, float spacing = -1.0f) { s_gui->_SameLine(offset_from_start_x, spacing); }

    static inline void PushItemWidth(float item_width) { s_gui->_PushItemWidth(item_width); }
    static inline void PopItemWidth() { s_gui->_PopItemWidth(); }

    static inline void PushButtonTextAlign(const vcm::Vec2 & padding) { s_gui->_PushButtonTextAlign(padding); }
    static inline void PushWindowPadding(const vcm::Vec2 & padding) { s_gui->_PushWindowPadding(padding); }
    static inline void PopStyleVar() { s_gui->_PopStyleVar(); }

    static inline GUIId DockSpace(GUIId id, const vcm::Vec2 & size, GUIDockNodeFlags flags) { return s_gui->_DockSpace(id, size, flags); }
    static inline GUIId DockSpace(const char * id, const vcm::Vec2 & size, GUIDockNodeFlags flags) { return s_gui->_DockSpace(id, size, flags); }
    static inline GUIId DockSpaceOverViewport() { return s_gui->_DockSpaceOverViewport(); }
    static inline GUIId DockSpaceAddNode(GUIId id, GUIDockNodeFlags flags) { return s_gui->_DockSpaceAddNode(id, flags); }
    static inline void DockSpaceRemoveNode(GUIId id) { s_gui->_DockSpaceRemoveNode(id); }
    static inline void DockSpaceSetNodeSize(GUIId id, const vcm::Vec2 & size) { s_gui->_DockSpaceSetNodeSize(id, size); }
    static inline GUIId DockSpaceSplitNode(GUIId id, GUIDir split_dir, float size_ratio, GUIId * out_id_at_dir, GUIId * out_id_at_opposite_dir) { return s_gui->_DockSpaceSplitNode(id, split_dir, size_ratio, out_id_at_dir, out_id_at_opposite_dir); }
    static inline void DockWindow(const char * str_id, GUIId id) { s_gui->_DockWindow(str_id, id); }
    static inline void DockFinish(GUIId id) { s_gui->_DockFinish(id); }

    static bool isFirstFrame();

    static inline GUIId GetID(const char * str_id) { return s_gui->_GetID(str_id); }

    static inline void Test() { s_gui->_Test(); }

    static void GraphicsSettingsCollaspingHeader();

    static void EntitiesListCollapsingHeader();

    static bool EditableTexture(vc::Texture * texture, vc::String & path);
    static bool EditableModel(vc::Model * model, vc::String & path);
private:
    static void _EntityPropertiesWindow();

protected:
    virtual vc::Error _Reset() = 0;

    virtual void _AddFont(const char * fontPath, float fontSize, const uint16_t * glyphRanges) = 0;
    virtual void _AddFont(const char * fontPath, float fontSize) = 0;

    virtual void _SetNextWindowPos(const vcm::Vec2 & pos, GUICond cond, const vcm::Vec2 & pivot) = 0;
    virtual void _SetNextWindowSize(const vcm::Vec2 & size, GUICond cond) = 0;
    virtual void _SetNextWindowViewport(GUIViewport viewport) = 0;

    virtual vcm::Vec2 _GetContentRegionAvail() = 0;

    virtual vcm::Vec2 _GetWindowSize() = 0;
    virtual vcm::Vec2 _GetWindowPos() = 0;
    virtual GUIViewport _GetMainViewport() = 0;

    virtual void _NewFrame() = 0;
    virtual void _Begin(const char * name, bool * p_open, GUIWindowFlags flags) = 0;
    virtual void _End() = 0;
    virtual void _Text(const char* fmt, ...) = 0;
    virtual void _TextColored(const vcm::Vec4 & col, const char* fmt, ...) = 0;
    virtual void _LabelText(const char* label, const char* fmt, ...) = 0;

    virtual void _Image(const vc::Texture * texture, const vcm::Vec2 & size, bool centering) = 0;

    virtual bool _InputText(const char* label, char* buf, size_t buf_size, GUIInputTextFlags flags) = 0;

    virtual bool _TreeNode(const char* label) = 0;
    virtual void _TreePop() = 0;
    virtual void _TreePush(const char* str_id) = 0;

    virtual void _SeparatorText(const char* text) = 0;
    virtual void _Separator() = 0;
    virtual void _Spacing() = 0;

    virtual bool _SliderFloat(const char* label, float* v, float v_min, float v_max, const char* format) = 0;
    virtual bool _SliderFloat3(const char* label, float v[3], float v_min, float v_max, const char* format) = 0;

    virtual bool _InputFloat(const char* label, float* v, float step, float step_fast, const char* format, GUIColorEditFlags flags) = 0;
    virtual bool _InputFloat3(const char* label, float v[3], const char* format, GUIColorEditFlags flags) = 0;

    virtual void _ColorEdit3(const char* label, float col[3], GUIColorEditFlags flags) = 0;

    virtual bool _CollapsingHeader(const char* label, GUITreeNodeFlags flags) = 0;

    virtual bool _Button(const char* label, const vcm::Vec2 & size) = 0;
    virtual bool _Checkbox(const char* label, bool* v) = 0;
    virtual void _ProgressBar(float fraction, const vcm::Vec2 & size_arg, const char* overlay) = 0;

    virtual bool _Selectable(const char* label, bool selected, GUISelectableFlags flags, const vcm::Vec2 & size) = 0;

    virtual bool _BeginCombo(const char* label, const char* preview_value, GUIComboFlags flags) = 0;
    virtual void _EndCombo() = 0;

    virtual bool _BeginMenu(const char* label, bool enabled) = 0;
    virtual void _EndMenu() = 0;

    virtual bool _BeginMainMenuBar() = 0;
    virtual void _EndMainMenuBar() = 0;

    virtual bool _BeginMenuBar() = 0;
    virtual void _EndMenuBar() = 0;

    virtual bool _BeginChild(const char* str_id, const vcm::Vec2 & size, GUIChildFlags childFlags, GUIWindowFlags extra_flags) = 0;
    virtual void _EndChild() = 0;

    virtual bool _MenuItem(const char* str, const char* text) = 0;

    virtual void _SetNextItemWidth(float item_width) = 0;
    virtual void _SetItemDefaultFocus() = 0;

    virtual void _SameLine(float offset_from_start_x, float spacing) = 0;

    virtual void _PushItemWidth(float item_width) = 0;
    virtual void _PopItemWidth() = 0;

    virtual void _PushButtonTextAlign(const vcm::Vec2 & padding) = 0;
    virtual void _PushWindowPadding(const vcm::Vec2 & padding) = 0;
    virtual void _PopStyleVar() = 0;

    virtual GUIId _DockSpace(GUIId id, const vcm::Vec2 & size, GUIDockNodeFlags flags) = 0;
    virtual GUIId _DockSpace(const char * id, const vcm::Vec2 & size, GUIDockNodeFlags flags) = 0;
    virtual GUIId _DockSpaceOverViewport() = 0;
    virtual GUIId _DockSpaceAddNode(GUIId id, GUIDockNodeFlags flags) = 0;
    virtual void _DockSpaceRemoveNode(GUIId id) = 0;
    virtual void _DockSpaceSetNodeSize(GUIId id, const vcm::Vec2 & size) = 0;
    virtual GUIId _DockSpaceSplitNode(GUIId id, GUIDir split_dir, float size_ratio, GUIId * out_id_at_dir, GUIId * out_id_at_opposite_dir) = 0;
    virtual void _DockWindow(const char * str_id, GUIId id) = 0;
    virtual void _DockFinish(GUIId id) = 0;

    virtual GUIId _GetID(const char * str_id) = 0;

    virtual void _Render() = 0;
    virtual vc::Error _PreUpdate() = 0;

    virtual void _Test() = 0;

protected:
    GraphicsApplication * _app;
    struct FontData
    {
        const std::string path;
        float size;
        const uint16_t * glyphRanges;
    };
    vc::Vector<FontData> _fonts;

private:
    /**
     * @brief Mainly serves for the GUI to update its internal state
     * automatically from VenomEngine and not the Graphics API
     */
    vc::Error __PreUpdate();

private:
    static GUI * s_gui;
    bool __firstFrame;

private:
    static GUIDrawCallback s_guiDrawCallback;

    friend class VenomEngine;
};
}
}
