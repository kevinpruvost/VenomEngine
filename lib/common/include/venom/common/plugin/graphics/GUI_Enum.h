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
    GUIWindowFlags_None                   = 0,
    GUIWindowFlags_NoTitleBar             = 1 << 0,   // Disable title-bar
    GUIWindowFlags_NoResize               = 1 << 1,   // Disable user resizing with the lower-right grip
    GUIWindowFlags_NoMove                 = 1 << 2,   // Disable user moving the window
    GUIWindowFlags_NoScrollbar            = 1 << 3,   // Disable scrollbars (window can still scroll with mouse or programmatically)
    GUIWindowFlags_NoScrollWithMouse      = 1 << 4,   // Disable user vertically scrolling with mouse wheel. On child window, mouse wheel will be forwarded to the parent unless NoScrollbar is also set.
    GUIWindowFlags_NoCollapse             = 1 << 5,   // Disable user collapsing window by double-clicking on it. Also referred to as Window Menu Button (e.g. within a docking node).
    GUIWindowFlags_AlwaysAutoResize       = 1 << 6,   // Resize every window to its content every frame
    GUIWindowFlags_NoBackground           = 1 << 7,   // Disable drawing background color (WindowBg, etc.) and outside border. Similar as using SetNextWindowBgAlpha(0.0f).
    GUIWindowFlags_NoSavedSettings        = 1 << 8,   // Never load/save settings in .ini file
    GUIWindowFlags_NoMouseInputs          = 1 << 9,   // Disable catching mouse, hovering test with pass through.
    GUIWindowFlags_MenuBar                = 1 << 10,  // Has a menu-bar
    GUIWindowFlags_HorizontalScrollbar    = 1 << 11,  // Allow horizontal scrollbar to appear (off by default). You may use SetNextWindowContentSize(ImVec2(width,0.0f)); prior to calling Begin() to specify width. Read code in GUI_demo in the "Horizontal Scrolling" section.
    GUIWindowFlags_NoFocusOnAppearing     = 1 << 12,  // Disable taking focus when transitioning from hidden to visible state
    GUIWindowFlags_NoBringToFrontOnFocus  = 1 << 13,  // Disable bringing window to front when taking focus (e.g. clicking on it or programmatically giving it focus)
    GUIWindowFlags_AlwaysVerticalScrollbar= 1 << 14,  // Always show vertical scrollbar (even if ContentSize.y < Size.y)
    GUIWindowFlags_AlwaysHorizontalScrollbar=1<< 15,  // Always show horizontal scrollbar (even if ContentSize.x < Size.x)
    GUIWindowFlags_NoNavInputs            = 1 << 16,  // No keyboard/gamepad navigation within the window
    GUIWindowFlags_NoNavFocus             = 1 << 17,  // No focusing toward this window with keyboard/gamepad navigation (e.g. skipped by CTRL+TAB)
    GUIWindowFlags_UnsavedDocument        = 1 << 18,  // Display a dot next to the title. When used in a tab/docking context, tab is selected when clicking the X + closure is not assumed (will wait for user to stop submitting the tab). Otherwise closure is assumed when pressing the X, so if you keep submitting the tab may reappear at end of tab bar.
    GUIWindowFlags_NoDocking           = 1 << 19,  // Disable docking of this window
    GUIWindowFlags_NoNav                  = GUIWindowFlags_NoNavInputs | GUIWindowFlags_NoNavFocus,
    GUIWindowFlags_NoDecoration           = GUIWindowFlags_NoTitleBar | GUIWindowFlags_NoResize | GUIWindowFlags_NoScrollbar | GUIWindowFlags_NoCollapse,
    GUIWindowFlags_NoInputs               = GUIWindowFlags_NoMouseInputs | GUIWindowFlags_NoNavInputs | GUIWindowFlags_NoNavFocus,

    // [Internal]
    GUIWindowFlags_ChildWindow            = 1 << 24,  // Don't use! For internal use by BeginChild()
    GUIWindowFlags_Tooltip                = 1 << 25,  // Don't use! For internal use by BeginTooltip()
    GUIWindowFlags_Popup                  = 1 << 26,  // Don't use! For internal use by BeginPopup()
    GUIWindowFlags_Modal                  = 1 << 27,  // Don't use! For internal use by BeginPopupModal()
    GUIWindowFlags_ChildMenu              = 1 << 28,  // Don't use! For internal use by BeginMenu()
};
typedef int GUIWindowFlags;

enum GUIChildFlagsBits
{
    GUIChildFlags_None                    = 0,
    GUIChildFlags_Borders                 = 1 << 0,   // Show an outer border and enable WindowPadding. (IMPORTANT: this is always == 1 == true for legacy reason)
    GUIChildFlags_AlwaysUseWindowPadding  = 1 << 1,   // Pad with style.WindowPadding even if no border are drawn (no padding by default for non-bordered child windows because it makes more sense)
    GUIChildFlags_ResizeX                 = 1 << 2,   // Allow resize from right border (layout direction). Enable .ini saving (unless GUIGUIWindowFlags_NoSavedSettings passed to window flags)
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

enum GUIInputTextFlagsBits
{
    // Basic filters (also see GUIInputTextFlags_CallbackCharFilter)
    GUIInputTextFlags_None                = 0,
    GUIInputTextFlags_CharsDecimal        = 1 << 0,   // Allow 0123456789.+-*/
    GUIInputTextFlags_CharsHexadecimal    = 1 << 1,   // Allow 0123456789ABCDEFabcdef
    GUIInputTextFlags_CharsScientific     = 1 << 2,   // Allow 0123456789.+-*/eE (Scientific notation input)
    GUIInputTextFlags_CharsUppercase      = 1 << 3,   // Turn a..z into A..Z
    GUIInputTextFlags_CharsNoBlank        = 1 << 4,   // Filter out spaces, tabs

    // Inputs
    GUIInputTextFlags_AllowTabInput       = 1 << 5,   // Pressing TAB input a '\t' character into the text field
    GUIInputTextFlags_EnterReturnsTrue    = 1 << 6,   // Return 'true' when Enter is pressed (as opposed to every time the value was modified). Consider using IsItemDeactivatedAfterEdit() instead!
    GUIInputTextFlags_EscapeClearsAll     = 1 << 7,   // Escape key clears content if not empty, and deactivate otherwise (contrast to default behavior of Escape to revert)
    GUIInputTextFlags_CtrlEnterForNewLine = 1 << 8,   // In multi-line mode, validate with Enter, add new line with Ctrl+Enter (default is opposite: validate with Ctrl+Enter, add line with Enter).

    // Other options
    GUIInputTextFlags_ReadOnly            = 1 << 9,   // Read-only mode
    GUIInputTextFlags_Password            = 1 << 10,  // Password mode, display all characters as '*', disable copy
    GUIInputTextFlags_AlwaysOverwrite     = 1 << 11,  // Overwrite mode
    GUIInputTextFlags_AutoSelectAll       = 1 << 12,  // Select entire text when first taking mouse focus
    GUIInputTextFlags_ParseEmptyRefVal    = 1 << 13,  // InputFloat(), InputInt(), InputScalar() etc. only: parse empty string as zero value.
    GUIInputTextFlags_DisplayEmptyRefVal  = 1 << 14,  // InputFloat(), InputInt(), InputScalar() etc. only: when value is zero, do not display it. Generally used with GUIInputTextFlags_ParseEmptyRefVal.
    GUIInputTextFlags_NoHorizontalScroll  = 1 << 15,  // Disable following the cursor horizontally
    GUIInputTextFlags_NoUndoRedo          = 1 << 16,  // Disable undo/redo. Note that input text owns the text data while active, if you want to provide your own undo/redo stack you need e.g. to call ClearActiveID().

    // Callback features
    GUIInputTextFlags_CallbackCompletion  = 1 << 17,  // Callback on pressing TAB (for completion handling)
    GUIInputTextFlags_CallbackHistory     = 1 << 18,  // Callback on pressing Up/Down arrows (for history handling)
    GUIInputTextFlags_CallbackAlways      = 1 << 19,  // Callback on each iteration. User code may query cursor position, modify text buffer.
    GUIInputTextFlags_CallbackCharFilter  = 1 << 20,  // Callback on character inputs to replace or discard them. Modify 'EventChar' to replace or discard, or return 1 in callback to discard.
    GUIInputTextFlags_CallbackResize      = 1 << 21,  // Callback on buffer capacity changes request (beyond 'buf_size' parameter value), allowing the string to grow. Notify when the string wants to be resized (for string types which hold a cache of their Size). You will be provided a new BufSize in the callback and NEED to honor it. (see misc/cpp/GUI_stdlib.h for an example of using this)
    GUIInputTextFlags_CallbackEdit        = 1 << 22,  // Callback on any edit (note that InputText() already returns true on edit, the callback is useful mainly to manipulate the underlying buffer while focus is active)
};
typedef int GUIInputTextFlags;

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

enum GUIDockNodeFlagsBits
{
    GUIDockNodeFlags_None                         = 0,
    GUIDockNodeFlags_KeepAliveOnly                = 1 << 0,   //       // Don't display the dockspace node but keep it alive. Windows docked into this dockspace node won't be undocked.
    //GUIDockNodeFlags_NoCentralNode              = 1 << 1,   //       // Disable Central Node (the node which can stay empty)
    GUIDockNodeFlags_NoDockingOverCentralNode     = 1 << 2,   //       // Disable docking over the Central Node, which will be always kept empty.
    GUIDockNodeFlags_PassthruCentralNode          = 1 << 3,   //       // Enable passthru dockspace: 1) DockSpace() will render a GUICol_WindowBg background covering everything excepted the Central Node when empty. Meaning the host window should probably use SetNextWindowBgAlpha(0.0f) prior to Begin() when using this. 2) When Central Node is empty: let inputs pass-through + won't display a DockingEmptyBg background. See demo for details.
    GUIDockNodeFlags_NoDockingSplit               = 1 << 4,   //       // Disable other windows/nodes from splitting this node.
    GUIDockNodeFlags_NoResize                     = 1 << 5,   // Saved // Disable resizing node using the splitter/separators. Useful with programmatically setup dockspaces.
    GUIDockNodeFlags_AutoHideTabBar               = 1 << 6,   //       // Tab bar will automatically hide when there is a single window in the dock node.
    // [Internal]
    GUIDockNodeFlags_DockSpace                = 1 << 10,  // Saved // A dockspace is a node that occupy space within an existing user window. Otherwise the node is floating and create its own window.
    GUIDockNodeFlags_CentralNode              = 1 << 11,  // Saved // The central node has 2 main properties: stay visible when empty, only use "remaining" spaces from its neighbor.
    GUIDockNodeFlags_NoTabBar                 = 1 << 12,  // Saved // Tab bar is completely unavailable. No triangle in the corner to enable it back.
    GUIDockNodeFlags_HiddenTabBar             = 1 << 13,  // Saved // Tab bar is hidden, with a triangle in the corner to show it again (NB: actual tab-bar instance may be destroyed as this is only used for single-window tab bar)
    GUIDockNodeFlags_NoWindowMenuButton       = 1 << 14,  // Saved // Disable window/docking menu (that one that appears instead of the collapse button)
    GUIDockNodeFlags_NoCloseButton            = 1 << 15,  // Saved // Disable close button
    GUIDockNodeFlags_NoResizeX                = 1 << 16,  //       //
    GUIDockNodeFlags_NoResizeY                = 1 << 17,  //       //
    GUIDockNodeFlags_DockedWindowsInFocusRoute= 1 << 18,  //       // Any docked window will be automatically be focus-route chained (window->ParentGUIWindowForFocusRoute set to this) so Shortcut() in this window can run when any docked window is focused.
    // Disable docking/undocking actions in this dockspace or individual node (existing docked nodes will be preserved)
    // Those are not exposed in public because the desirable sharing/inheriting/copy-flag-on-split behaviors are quite difficult to design and understand.
    // The two public flags GUIDockNodeFlags_NoDockingOverCentralNode/GUIDockNodeFlags_NoDockingSplit don't have those issues.
    GUIDockNodeFlags_NoDockingSplitOther      = 1 << 19,  //       // Disable this node from splitting other windows/nodes.
    GUIDockNodeFlags_NoDockingOverMe          = 1 << 20,  //       // Disable other windows/nodes from being docked over this node.
    GUIDockNodeFlags_NoDockingOverOther       = 1 << 21,  //       // Disable this node from being docked over another window or non-empty node.
    GUIDockNodeFlags_NoDockingOverEmpty       = 1 << 22,  //       // Disable this node from being docked over an empty node (e.g. DockSpace with no other windows)
    GUIDockNodeFlags_NoDocking                = GUIDockNodeFlags_NoDockingOverMe | GUIDockNodeFlags_NoDockingOverOther | GUIDockNodeFlags_NoDockingOverEmpty | GUIDockNodeFlags_NoDockingSplit | GUIDockNodeFlags_NoDockingSplitOther,
    // Masks
    GUIDockNodeFlags_SharedFlagsInheritMask_  = ~0,
    GUIDockNodeFlags_NoResizeFlagsMask_       = (int)GUIDockNodeFlags_NoResize | GUIDockNodeFlags_NoResizeX | GUIDockNodeFlags_NoResizeY,

    // When splitting, those local flags are moved to the inheriting child, never duplicated
    GUIDockNodeFlags_LocalFlagsTransferMask_  = (int)GUIDockNodeFlags_NoDockingSplit | GUIDockNodeFlags_NoResizeFlagsMask_ | (int)GUIDockNodeFlags_AutoHideTabBar | GUIDockNodeFlags_CentralNode | GUIDockNodeFlags_NoTabBar | GUIDockNodeFlags_HiddenTabBar | GUIDockNodeFlags_NoWindowMenuButton | GUIDockNodeFlags_NoCloseButton,
    GUIDockNodeFlags_SavedFlagsMask_          = GUIDockNodeFlags_NoResizeFlagsMask_ | GUIDockNodeFlags_DockSpace | GUIDockNodeFlags_CentralNode | GUIDockNodeFlags_NoTabBar | GUIDockNodeFlags_HiddenTabBar | GUIDockNodeFlags_NoWindowMenuButton | GUIDockNodeFlags_NoCloseButton,
};
typedef int GUIDockNodeFlags;

typedef unsigned int GUIId;

// A cardinal direction
enum GUIDir : int
{
    GUIDir_None    = -1,
    GUIDir_Left    = 0,
    GUIDir_Right   = 1,
    GUIDir_Up      = 2,
    GUIDir_Down    = 3,
    GUIDir_COUNT
};

}
}