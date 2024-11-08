///
/// Project: VenomEngineWorkspace
/// @file GUI_Enum.h
/// @date Nov, 07 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

namespace venom
{
namespace common
{
/**
 * Window flags based on GUIWindowFlagsBits_
 */
enum GUIWindowFlagsBits
{
    WindowFlags_None                   = 0,
    WindowFlags_NoTitleBar             = 1 << 0,   // Disable title-bar
    WindowFlags_NoResize               = 1 << 1,   // Disable user resizing with the lower-right grip
    WindowFlags_NoMove                 = 1 << 2,   // Disable user moving the window
    WindowFlags_NoScrollbar            = 1 << 3,   // Disable scrollbars (window can still scroll with mouse or programmatically)
    WindowFlags_NoScrollWithMouse      = 1 << 4,   // Disable user vertically scrolling with mouse wheel. On child window, mouse wheel will be forwarded to the parent unless NoScrollbar is also set.
    WindowFlags_NoCollapse             = 1 << 5,   // Disable user collapsing window by double-clicking on it. Also referred to as Window Menu Button (e.g. within a docking node).
    WindowFlags_AlwaysAutoResize       = 1 << 6,   // Resize every window to its content every frame
    WindowFlags_NoBackground           = 1 << 7,   // Disable drawing background color (WindowBg, etc.) and outside border. Similar as using SetNextWindowBgAlpha(0.0f).
    WindowFlags_NoSavedSettings        = 1 << 8,   // Never load/save settings in .ini file
    WindowFlags_NoMouseInputs          = 1 << 9,   // Disable catching mouse, hovering test with pass through.
    WindowFlags_MenuBar                = 1 << 10,  // Has a menu-bar
    WindowFlags_HorizontalScrollbar    = 1 << 11,  // Allow horizontal scrollbar to appear (off by default). You may use SetNextWindowContentSize(ImVec2(width,0.0f)); prior to calling Begin() to specify width. Read code in GUI_demo in the "Horizontal Scrolling" section.
    WindowFlags_NoFocusOnAppearing     = 1 << 12,  // Disable taking focus when transitioning from hidden to visible state
    WindowFlags_NoBringToFrontOnFocus  = 1 << 13,  // Disable bringing window to front when taking focus (e.g. clicking on it or programmatically giving it focus)
    WindowFlags_AlwaysVerticalScrollbar= 1 << 14,  // Always show vertical scrollbar (even if ContentSize.y < Size.y)
    WindowFlags_AlwaysHorizontalScrollbar=1<< 15,  // Always show horizontal scrollbar (even if ContentSize.x < Size.x)
    WindowFlags_NoNavInputs            = 1 << 16,  // No keyboard/gamepad navigation within the window
    WindowFlags_NoNavFocus             = 1 << 17,  // No focusing toward this window with keyboard/gamepad navigation (e.g. skipped by CTRL+TAB)
    WindowFlags_UnsavedDocument        = 1 << 18,  // Display a dot next to the title. When used in a tab/docking context, tab is selected when clicking the X + closure is not assumed (will wait for user to stop submitting the tab). Otherwise closure is assumed when pressing the X, so if you keep submitting the tab may reappear at end of tab bar.
    WindowFlags_NoNav                  = WindowFlags_NoNavInputs | WindowFlags_NoNavFocus,
    WindowFlags_NoDecoration           = WindowFlags_NoTitleBar | WindowFlags_NoResize | WindowFlags_NoScrollbar | WindowFlags_NoCollapse,
    WindowFlags_NoInputs               = WindowFlags_NoMouseInputs | WindowFlags_NoNavInputs | WindowFlags_NoNavFocus,

    // [Internal]
    WindowFlags_ChildWindow            = 1 << 24,  // Don't use! For internal use by BeginChild()
    WindowFlags_Tooltip                = 1 << 25,  // Don't use! For internal use by BeginTooltip()
    WindowFlags_Popup                  = 1 << 26,  // Don't use! For internal use by BeginPopup()
    WindowFlags_Modal                  = 1 << 27,  // Don't use! For internal use by BeginPopupModal()
    WindowFlags_ChildMenu              = 1 << 28,  // Don't use! For internal use by BeginMenu()
};
typedef int GUIWindowFlags;

enum GUIColorEditFlagsBits
{
    GUIColorEditFlags_None            = 0,
    GUIColorEditFlags_NoAlpha         = 1 << 1,   //              // ColorEdit, ColorPicker, ColorButton: ignore Alpha component (will only read 3 components from the input pointer).
    GUIColorEditFlags_NoPicker        = 1 << 2,   //              // ColorEdit: disable picker when clicking on color square.
    GUIColorEditFlags_NoOptions       = 1 << 3,   //              // ColorEdit: disable toggling options menu when right-clicking on inputs/small preview.
    GUIColorEditFlags_NoSmallPreview  = 1 << 4,   //              // ColorEdit, ColorPicker: disable color square preview next to the inputs. (e.g. to show only the inputs)
    GUIColorEditFlags_NoInputs        = 1 << 5,   //              // ColorEdit, ColorPicker: disable inputs sliders/text widgets (e.g. to show only the small preview color square).
    GUIColorEditFlags_NoTooltip       = 1 << 6,   //              // ColorEdit, ColorPicker, ColorButton: disable tooltip when hovering the preview.
    GUIColorEditFlags_NoLabel         = 1 << 7,   //              // ColorEdit, ColorPicker: disable display of inline text label (the label is still forwarded to the tooltip and picker).
    GUIColorEditFlags_NoSidePreview   = 1 << 8,   //              // ColorPicker: disable bigger color preview on right side of the picker, use small color square preview instead.
    GUIColorEditFlags_NoDragDrop      = 1 << 9,   //              // ColorEdit: disable drag and drop target. ColorButton: disable drag and drop source.
    GUIColorEditFlags_NoBorder        = 1 << 10,  //              // ColorButton: disable border (which is enforced by default)

    // User Options (right-click on widget to change some of them).
    GUIColorEditFlags_AlphaBar        = 1 << 16,  //              // ColorEdit, ColorPicker: show vertical alpha bar/gradient in picker.
    GUIColorEditFlags_AlphaPreview    = 1 << 17,  //              // ColorEdit, ColorPicker, ColorButton: display preview as a transparent color over a checkerboard, instead of opaque.
    GUIColorEditFlags_AlphaPreviewHalf= 1 << 18,  //              // ColorEdit, ColorPicker, ColorButton: display half opaque / half checkerboard, instead of opaque.
    GUIColorEditFlags_HDR             = 1 << 19,  //              // (WIP) ColorEdit: Currently only disable 0.0f..1.0f limits in RGBA edition (note: you probably want to use GUIColorEditFlags_Float flag as well).
    GUIColorEditFlags_DisplayRGB      = 1 << 20,  // [Display]    // ColorEdit: override _display_ type among RGB/HSV/Hex. ColorPicker: select any combination using one or more of RGB/HSV/Hex.
    GUIColorEditFlags_DisplayHSV      = 1 << 21,  // [Display]    // "
    GUIColorEditFlags_DisplayHex      = 1 << 22,  // [Display]    // "
    GUIColorEditFlags_Uint8           = 1 << 23,  // [DataType]   // ColorEdit, ColorPicker, ColorButton: _display_ values formatted as 0..255.
    GUIColorEditFlags_Float           = 1 << 24,  // [DataType]   // ColorEdit, ColorPicker, ColorButton: _display_ values formatted as 0.0f..1.0f floats instead of 0..255 integers. No round-trip of value via integers.
    GUIColorEditFlags_PickerHueBar    = 1 << 25,  // [Picker]     // ColorPicker: bar for Hue, rectangle for Sat/Value.
    GUIColorEditFlags_PickerHueWheel  = 1 << 26,  // [Picker]     // ColorPicker: wheel for Hue, triangle for Sat/Value.
    GUIColorEditFlags_InputRGB        = 1 << 27,  // [Input]      // ColorEdit, ColorPicker: input and output data in RGB format.
    GUIColorEditFlags_InputHSV        = 1 << 28,  // [Input]      // ColorEdit, ColorPicker: input and output data in HSV format.

    // Defaults Options. You can set application defaults using SetColorEditOptions(). The intent is that you probably don't want to
    // override them in most of your calls. Let the user choose via the option menu and/or call SetColorEditOptions() once during startup.
    GUIColorEditFlags_DefaultOptions_ = GUIColorEditFlags_Uint8 | GUIColorEditFlags_DisplayRGB | GUIColorEditFlags_InputRGB | GUIColorEditFlags_PickerHueBar,

    // [Internal] Masks
    GUIColorEditFlags_DisplayMask_    = GUIColorEditFlags_DisplayRGB | GUIColorEditFlags_DisplayHSV | GUIColorEditFlags_DisplayHex,
    GUIColorEditFlags_DataTypeMask_   = GUIColorEditFlags_Uint8 | GUIColorEditFlags_Float,
    GUIColorEditFlags_PickerMask_     = GUIColorEditFlags_PickerHueWheel | GUIColorEditFlags_PickerHueBar,
    GUIColorEditFlags_InputMask_      = GUIColorEditFlags_InputRGB | GUIColorEditFlags_InputHSV,

    // Obsolete names
    //GUIColorEditFlags_RGB = GUIColorEditFlags_DisplayRGB, GUIColorEditFlags_HSV = GUIColorEditFlags_DisplayHSV, GUIColorEditFlags_HEX = GUIColorEditFlags_DisplayHex  // [renamed in 1.69]
};
typedef int GUIColorEditFlags;
}
}