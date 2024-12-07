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
 * Window flags based on GUIWindowFlagsBits_ (based on GUI 1.91.5)
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

enum GUIChildFlagsBits
{
    GUIChildFlags_None                    = 0,
    GUIChildFlags_Borders                 = 1 << 0,   // Show an outer border and enable WindowPadding. (IMPORTANT: this is always == 1 == true for legacy reason)
    GUIChildFlags_AlwaysUseWindowPadding  = 1 << 1,   // Pad with style.WindowPadding even if no border are drawn (no padding by default for non-bordered child windows because it makes more sense)
    GUIChildFlags_ResizeX                 = 1 << 2,   // Allow resize from right border (layout direction). Enable .ini saving (unless GUIWindowFlags_NoSavedSettings passed to window flags)
    GUIChildFlags_ResizeY                 = 1 << 3,   // Allow resize from bottom border (layout direction). "
    GUIChildFlags_AutoResizeX             = 1 << 4,   // Enable auto-resizing width. Read "IMPORTANT: Size measurement" details above.
    GUIChildFlags_AutoResizeY             = 1 << 5,   // Enable auto-resizing height. Read "IMPORTANT: Size measurement" details above.
    GUIChildFlags_AlwaysAutoResize        = 1 << 6,   // Combined with AutoResizeX/AutoResizeY. Always measure size even when child is hidden, always return true, always disable clipping optimization! NOT RECOMMENDED.
    GUIChildFlags_FrameStyle              = 1 << 7,   // Style the child window like a framed item: use FrameBg, FrameRounding, FrameBorderSize, FramePadding instead of ChildBg, ChildRounding, ChildBorderSize, WindowPadding.
    GUIChildFlags_NavFlattened            = 1 << 8,   // [BETA] Share focus scope, allow keyboard/gamepad navigation to cross over parent border to this child or between sibling child windows.
};
typedef int GUIChildFlags;

/**
 * Color edit flags based on GUIColorEditFlagsBits_ (based on GUI 1.91.5)
 */
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

enum GuiComboFlagsBits
{
    GUIComboFlags_None                    = 0,
    GUIComboFlags_PopupAlignLeft          = 1 << 0,   // Align the popup toward the left by default
    GUIComboFlags_HeightSmall             = 1 << 1,   // Max ~4 items visible. Tip: If you want your combo popup to be a specific size you can use SetNextWindowSizeConstraints() prior to calling BeginCombo()
    GUIComboFlags_HeightRegular           = 1 << 2,   // Max ~8 items visible (default)
    GUIComboFlags_HeightLarge             = 1 << 3,   // Max ~20 items visible
    GUIComboFlags_HeightLargest           = 1 << 4,   // As many fitting items as possible
    GUIComboFlags_NoArrowButton           = 1 << 5,   // Display on the preview box without the square arrow button
    GUIComboFlags_NoPreview               = 1 << 6,   // Display only a square arrow button
    GUIComboFlags_WidthFitPreview         = 1 << 7,   // Width dynamically calculated from preview contents
    GUIComboFlags_HeightMask_             = GUIComboFlags_HeightSmall | GUIComboFlags_HeightRegular | GUIComboFlags_HeightLarge | GUIComboFlags_HeightLargest,
};
typedef int GUIComboFlags;

enum GUISelectableFlagsBits
{
    GUISelectableFlags_None               = 0,
    GUISelectableFlags_NoAutoClosePopups  = 1 << 0,   // Clicking this doesn't close parent popup window (overrides GUIItemFlags_AutoClosePopups)
    GUISelectableFlags_SpanAllColumns     = 1 << 1,   // Frame will span all columns of its container table (text will still fit in current column)
    GUISelectableFlags_AllowDoubleClick   = 1 << 2,   // Generate press events on double clicks too
    GUISelectableFlags_Disabled           = 1 << 3,   // Cannot be selected, display grayed out text
    GUISelectableFlags_AllowOverlap       = 1 << 4,   // (WIP) Hit testing to allow subsequent widgets to overlap this one
    GUISelectableFlags_Highlight          = 1 << 5,   // Make the item be displayed as if it is hovered
};
typedef int GUISelectableFlags;

enum GUICondBits
{
    GUICond_None          = 0,        // No condition (always set the variable), same as _Always
    GUICond_Always        = 1 << 0,   // No condition (always set the variable), same as _None
    GUICond_Once          = 1 << 1,   // Set the variable once per runtime session (only the first call will succeed)
    GUICond_FirstUseEver  = 1 << 2,   // Set the variable if the object/window has no persistently saved data (no entry in .ini file)
    GUICond_Appearing     = 1 << 3,   // Set the variable if the object/window is appearing after being hidden/inactive (or the first time)
};
typedef int GUICond;

enum GUITreeNodeFlagsBits
{
    GUITreeNodeFlags_None                 = 0,
    GUITreeNodeFlags_Selected             = 1 << 0,   // Draw as selected
    GUITreeNodeFlags_Framed               = 1 << 1,   // Draw frame with background (e.g. for CollapsingHeader)
    GUITreeNodeFlags_AllowOverlap         = 1 << 2,   // Hit testing to allow subsequent widgets to overlap this one
    GUITreeNodeFlags_NoTreePushOnOpen     = 1 << 3,   // Don't do a TreePush() when open (e.g. for CollapsingHeader) = no extra indent nor pushing on ID stack
    GUITreeNodeFlags_NoAutoOpenOnLog      = 1 << 4,   // Don't automatically and temporarily open node when Logging is active (by default logging will automatically open tree nodes)
    GUITreeNodeFlags_DefaultOpen          = 1 << 5,   // Default node to be open
    GUITreeNodeFlags_OpenOnDoubleClick    = 1 << 6,   // Open on double-click instead of simple click (default for multi-select unless any _OpenOnXXX behavior is set explicitly). Both behaviors may be combined.
    GUITreeNodeFlags_OpenOnArrow          = 1 << 7,   // Open when clicking on the arrow part (default for multi-select unless any _OpenOnXXX behavior is set explicitly). Both behaviors may be combined.
    GUITreeNodeFlags_Leaf                 = 1 << 8,   // No collapsing, no arrow (use as a convenience for leaf nodes).
    GUITreeNodeFlags_Bullet               = 1 << 9,   // Display a bullet instead of arrow. IMPORTANT: node can still be marked open/close if you don't set the _Leaf flag!
    GUITreeNodeFlags_FramePadding         = 1 << 10,  // Use FramePadding (even for an unframed text node) to vertically align text baseline to regular widget height. Equivalent to calling AlignTextToFramePadding() before the node.
    GUITreeNodeFlags_SpanAvailWidth       = 1 << 11,  // Extend hit box to the right-most edge, even if not framed. This is not the default in order to allow adding other items on the same line without using AllowOverlap mode.
    GUITreeNodeFlags_SpanFullWidth        = 1 << 12,  // Extend hit box to the left-most and right-most edges (cover the indent area).
    GUITreeNodeFlags_SpanTextWidth        = 1 << 13,  // Narrow hit box + narrow hovering highlight, will only cover the label text.
    GUITreeNodeFlags_SpanAllColumns       = 1 << 14,  // Frame will span all columns of its container table (text will still fit in current column)
    GUITreeNodeFlags_NavLeftJumpsBackHere = 1 << 15,  // (WIP) Nav: left direction may move to this TreeNode() from any of its child (items submitted between TreeNode and TreePop)
    GUITreeNodeFlags_CollapsingHeader     = GUITreeNodeFlags_Framed | GUITreeNodeFlags_NoTreePushOnOpen | GUITreeNodeFlags_NoAutoOpenOnLog,
};
typedef int GUITreeNodeFlags;

}
}