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

namespace venom
{
namespace common
{
class GraphicsApplication;

typedef void(*GUIDrawCallback)();
class VENOM_COMMON_API GUI : public GraphicsPluginObject
{
public:
    GUI();
    ~GUI() override;

    void SetGraphicsApplication(GraphicsApplication * app);

    virtual vc::Error Initialize() = 0;
    virtual vc::Error Reset() = 0;

    inline void DrawCallback() { if (s_guiDrawCallback) s_guiDrawCallback(); }
    static inline void SetGUIDrawCallback(GUIDrawCallback guiDrawCallback) { s_guiDrawCallback = guiDrawCallback; }

    inline void Render() { DrawCallback(); s_gui->_Render(); }

    static inline GUI * Get() { return s_gui; }

    static inline void NewFrame() { s_gui->_NewFrame(); }
    static inline void Begin(const char * name, bool * p_open = nullptr, GUIWindowFlags flags = 0) { s_gui->_Begin(name, p_open, flags); }
    static inline void End() { s_gui->_End(); }
    static inline void Text(const char* fmt, ...) { va_list args; va_start(args, fmt); s_gui->_Text(fmt, args); va_end(args); }
    static inline void TextColored(const vcm::Vec4 & col, const char* fmt, ...) { va_list args; va_start(args, fmt); s_gui->_TextColored(col, fmt, args); va_end(args); }
    static inline void LabelText(const char* label, const char* fmt, ...) { va_list args; va_start(args, fmt); s_gui->_LabelText(label, fmt, args); va_end(args); }

    static inline void SliderFloat(const char* label, float* v, float v_min, float v_max, const char* format = "%.3f", float power = 1.0f) { s_gui->_SliderFloat(label, v, v_min, v_max, format, power); }
    static inline void SliderFloat3(const char* label, float v[3], float v_min, float v_max, const char* format = "%.3f", float power = 1.0f) { s_gui->_SliderFloat3(label, v, v_min, v_max, format, power); }

    static inline void ColorEdit3(const char* label, float col[3], GUIColorEditFlags flags = 0) { s_gui->_ColorEdit3(label, col, flags); }

    static inline void Button(const char* label, const vcm::Vec2 & size = vcm::Vec2(0, 0)) { s_gui->_Button(label, size); }
    static inline bool Checkbox(const char* label, bool* v) { return s_gui->_Checkbox(label, v); }
    static inline void ProgressBar(float fraction, const vcm::Vec2 & size_arg = vcm::Vec2(-1, 0), const char* overlay = nullptr) { s_gui->_ProgressBar(fraction, size_arg, overlay); }

    static inline void SameLine(float offset_from_start_x = 0.0f, float spacing = -1.0f) { s_gui->_SameLine(offset_from_start_x, spacing); }

protected:
    virtual void _NewFrame() = 0;
    virtual void _Begin(const char * name, bool * p_open, GUIWindowFlags flags) = 0;
    virtual void _End() = 0;
    virtual void _Text(const char* fmt, ...) = 0;
    virtual void _TextColored(const vcm::Vec4 & col, const char* fmt, ...) = 0;
    virtual void _LabelText(const char* label, const char* fmt, ...) = 0;

    virtual void _SliderFloat(const char* label, float* v, float v_min, float v_max, const char* format, float power) = 0;
    virtual void _SliderFloat3(const char* label, float v[3], float v_min, float v_max, const char* format, float power) = 0;

    virtual void _ColorEdit3(const char* label, float col[3], GUIColorEditFlags flags) = 0;

    virtual void _Button(const char* label, const vcm::Vec2 & size) = 0;
    virtual bool _Checkbox(const char* label, bool* v) = 0;
    virtual void _ProgressBar(float fraction, const vcm::Vec2 & size_arg, const char* overlay) = 0;

    virtual void _SameLine(float offset_from_start_x, float spacing) = 0;

    virtual void _Render() = 0;

protected:
    GraphicsApplication * _app;

private:
    static GUI * s_gui;

private:
    static GUIDrawCallback s_guiDrawCallback;
};
}
}