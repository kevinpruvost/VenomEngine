///
/// Project: VenomEngineWorkspace
/// @file GUI.cc
/// @date Nov, 07 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <imgui_internal.h>
#include <imgui_impl_metal.h>
#include <venom/metal/plugin/graphics/GUI.h>

#include <venom/common/plugin/graphics/Camera.h>

#include <venom/metal/MetalApplication.h>
#include <venom/metal/plugin/graphics/RenderTarget.h>

#include <venom/metal/Layer.h>

#include <ImGuizmo.h>

#include <venom/metal/Device.h>

#import <CoreGraphics/CoreGraphics.h>

#include <venom/common/Config.h>

#include <venom/common/context/ContextApple.h>

#if defined(VENOM_PLATFORM_MACOS)

#define GLFW_INCLUDE_NONE
#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#endif

namespace venom
{
namespace metal
{
MetalGUI::MetalGUI()
{
}

MetalGUI::~MetalGUI()
{
    ImGui_ImplMetal_Shutdown();
}

void MetalGUI::__SetStyle()
{
    auto _style = &ImGui::GetStyle();

    _style->WindowBorderSize = 0.0f;
    _style->ChildBorderSize = 1.0f;
    _style->WindowPadding = ImVec2(3, 3);

    _style->WindowMinSize = ImVec2(160, 20);
    _style->FramePadding = ImVec2(4, 2);
    _style->ItemSpacing = ImVec2(6, 2);
    _style->ItemInnerSpacing = ImVec2(6, 4);
    _style->Alpha = 1.0f;
    _style->WindowRounding = 0.0f;
    _style->FrameRounding = 2.0f;
    _style->IndentSpacing = 6.0f;
    _style->ItemInnerSpacing = ImVec2(2, 4);
    _style->ColumnsMinSpacing = 50.0f;
    _style->GrabMinSize = 14.0f;
    _style->GrabRounding = 16.0f;
    _style->ScrollbarSize = 12.0f;
    _style->ScrollbarRounding = 16.0f;
    _style->Colors[ImGuiCol_Text] = ImVec4(0.86f, 0.93f, 0.89f, 0.78f); // Light text
    _style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.55f, 0.58f, 0.28f); // Muted text
    _style->Colors[ImGuiCol_WindowBg] = ImVec4(0.025f, 0.025f, 0.025f, 1.00f); // Very dark background
    _style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.50f);
    _style->Colors[ImGuiCol_FrameBg] = ImVec4(0.12f, 0.12f, 0.12f, 1.00f); // Darker gray
    _style->Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f); // Dark gray
    _style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.10f, 0.10f, 0.10f, 0.75f);
    _style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.90f); // Near black
    _style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    _style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    _style->Colors[ImGuiCol_Separator] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    _style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.25f, 0.25f, 0.25f, 0.04f);
    _style->Colors[ImGuiCol_PlotLines] = ImVec4(0.50f, 0.55f, 0.58f, 0.63f);
    _style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.50f, 0.55f, 0.58f, 0.63f);
    _style->Colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.95f); // Near black

    _style->Colors[ImGuiCol_Border] = ImVec4(0.0f, 0.0f, 0.0f, 0.50f); // Subtle border

    // Colored ones
    _style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.12f, 0.4f, 0.18f, 0.78f);
    _style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.12f, 0.4f, 0.18f, 1.00f);
    _style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.12f, 0.4f, 0.18f, 1.00f);
    _style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.12f, 0.4f, 0.18f, 0.78f);
    _style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.12f, 0.4f, 0.18f, 1.00f);
    _style->Colors[ImGuiCol_CheckMark] = ImVec4(0.14f, 0.45f, 0.17f, 1.00f);
    _style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.09f, 0.53f, 0.30f, 0.14f);
    _style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.12f, 0.4f, 0.18f, 1.00f);
    _style->Colors[ImGuiCol_Button] = ImVec4(0.09f, 0.53f, 0.30f, 0.14f);
    _style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.12f, 0.4f, 0.18f, 0.86f);
    _style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.12f, 0.4f, 0.18f, 1.00f);
    _style->Colors[ImGuiCol_Header] = ImVec4(0.12f, 0.4f, 0.18f, 0.76f);
    _style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.12f, 0.4f, 0.18f, 0.86f);
    _style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.12f, 0.4f, 0.18f, 1.00f);
    _style->Colors[ImGuiCol_Separator] = ImVec4(0.12f, 0.4f, 0.18f, 0.78f);
    _style->Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.12f, 0.4f, 0.18f, 0.78f);
    _style->Colors[ImGuiCol_SeparatorActive] = ImVec4(0.12f, 0.4f, 0.18f, 1.00f);
    _style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.12f, 0.4f, 0.18f, 0.78f);
    _style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.12f, 0.4f, 0.18f, 1.00f);
    _style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.12f, 0.4f, 0.18f, 1.00f);
    _style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.12f, 0.4f, 0.18f, 1.00f);
    _style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.12f, 0.4f, 0.18f, 0.43f);

    _style->Colors[ImGuiCol_Tab] = ImVec4(0.0f, 0.3f, 0.0f, 1.0f);           // Dark green for an inactive tab
    _style->Colors[ImGuiCol_TabHovered] = ImVec4(0.0f, 0.5f, 0.0f, 1.0f);    // Brighter green when the tab is hovered
    _style->Colors[ImGuiCol_TabActive] = ImVec4(0.0f, 0.4f, 0.0f, 1.0f);     // Brighter green for the active tab
    _style->Colors[ImGuiCol_TabUnfocused] = ImVec4(0.0f, 0.3f, 0.0f, 0.8f);  // Dark green for an unfocused inactive tab
    _style->Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.0f, 0.4f, 0.0f, 0.9f); // Green for an active tab in an unfocused window
    _style->Colors[ImGuiCol_TabSelectedOverline] = ImVec4(0.0f, 0.4f, 0.0f, 0.9f); // Green for an active tab in an unfocused window
}

vc::Error MetalGUI::_Initialize()
{
    const MetalApplication * const app = static_cast<const MetalApplication * const>(_app);

    // Setup Platform/Renderer backends
    if (_firstInit) {
        ImGui::CreateContext();
        ImGui::StyleColorsLight();
        //renderPassDescriptor = [MTLRenderPassDescriptor new];
    }

    // Setup Platform/Renderer backends

#if !defined(VENOM_PLATFORM_IOS)
    ImGui_ImplGlfw_InitForOther((GLFWwindow*)vc::Context::Get()->GetWindow(), true);
#endif
    if (!ImGui_ImplMetal_Init(GetMetalDevice()))
        return vc::Error::Failure;
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2(static_cast<float>(vc::Context::GetWindowWidth()), static_cast<float>(vc::Context::GetWindowHeight()));
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.Fonts->AddFontDefault();
    
#if !defined(VENOM_PLATFORM_IOS)
    // TODO: Abstract if context is managed by glfw or not
    NSWindow *nswin;
    if (vc::Config::GetContextType() == vc::Context::ContextType::GLFW)
        nswin = glfwGetCocoaWindow((GLFWwindow*)vc::Context::Get()->GetWindow());
    else if (vc::Config::GetContextType() == vc::Context::ContextType::Apple)
        nswin = venom::context::apple::ContextApple::GetAppleContext()->GetAppleWindow();
    nswin.contentView.layer = getGlobalMetalLayer();
    nswin.contentView.wantsLayer = YES;
#endif

    __SetStyle();
    return vc::Error::Success;
}

void MetalGUI::_EntityGuizmo(vc::Transform3D* transform3D, const vcm::Vec2 & renderingSize)
{
    static ImGuizmo::OPERATION operation = ImGuizmo::TRANSLATE;
    if (ImGui::IsKeyPressed(ImGuiKey_1)) // Press 'T' for translate
        operation = ImGuizmo::TRANSLATE;
    if (ImGui::IsKeyPressed(ImGuiKey_2)) // Press 'R' for rotate
        operation = ImGuizmo::ROTATE;
    if (ImGui::IsKeyPressed(ImGuiKey_3)) // Press 'S' for scale
        operation = ImGuizmo::SCALE;

    static ImGuizmo::MODE mode = ImGuizmo::LOCAL;
    if (ImGui::IsKeyPressed(ImGuiKey_4)) // Press 'L' to toggle local/world mode
        mode = (mode == ImGuizmo::LOCAL) ? ImGuizmo::WORLD : ImGuizmo::LOCAL;

    vc::Camera * camera = vc::Camera::GetMainCamera();

    vcm::Mat4 & vcmViewMatrix = camera->GetViewMatrixMut();
    float * viewMatrix = vcm::ValuePtr(vcmViewMatrix);
    const float * projectionMatrix = vcm::ValuePtr(camera->GetProjectionMatrix());

    ImGuizmo::SetDrawlist();
    //ImGuizmo::SetRect(0, 0, vc::Context::GetWindowWidth(), vc::Context::GetWindowHeight());
    ImVec2 pos = ImGui::GetWindowPos();
    ImGuizmo::SetRect(pos.x, pos.y + __imageVerticalOffset, renderingSize.x, renderingSize.y);

    if (transform3D)
    {
        float * modelMatrix = vcm::ValuePtr(transform3D->GetModelMatrixMut());

        // static float cubeMatrix[16] = {
        //     1.0f, 0.0f, 0.0f, 0.0f,
        //     0.0f, 1.0f, 0.0f, 0.0f,
        //     0.0f, 0.0f, 1.0f, 0.0f,
        //     0.0f, 0.0f, 0.0f, 1.0f
        // };

        // ImGuizmo::DrawGrid(viewMatrix, projectionMatrix, modelMatrix, 10.0f);
        // ImGuizmo::DrawCubes(viewMatrix, projectionMatrix, cubeMatrix, 1);

        ImGuizmo::PushID(0);
        if (ImGuizmo::Manipulate(viewMatrix, projectionMatrix, operation, mode, modelMatrix)) {
            float matrixTranslation[3], matrixRotation[3], matrixScale[3];
            // Matrix rotation is in degrees
            ImGuizmo::DecomposeMatrixToComponents(modelMatrix, matrixTranslation, matrixRotation, matrixScale);
            transform3D->SetRawPosition(vcm::Vec3(matrixTranslation[0], matrixTranslation[1], matrixTranslation[2]));
            transform3D->SetRawRotation(vcm::Vec3(matrixRotation[0], matrixRotation[1], matrixRotation[2]));
            transform3D->SetRawScale(vcm::Vec3(matrixScale[0], matrixScale[1], matrixScale[2]));
        }
        ImGuizmo::PopID();
    }
    // constexpr int cubeSize = 100;
    // ImGuizmo::ViewManipulate(viewMatrix, 1.0f, {pos.x + renderingSize.x - cubeSize, pos.y + __imageVerticalOffset}, {cubeSize, cubeSize}, 0x10101010);
    // // TODO: Take care of the view matrix change
    // if (ImGuizmo::IsUsingViewManipulate()) {
    //     float yaw = std::atan2f(vcmViewMatrix[1][2], vcmViewMatrix[2][2]);
    //     float pitch = std::atan2f(-vcmViewMatrix[0][2], std::sqrtf(vcmViewMatrix[1][2] * vcmViewMatrix[1][2] + vcmViewMatrix[2][2] * vcmViewMatrix[2][2]));
    //     float roll = std::atan2f(vcmViewMatrix[0][1], vcmViewMatrix[0][0]);
    //     camera->SetRawRotation(vcm::Vec3(pitch, yaw, roll));
    // }
}

vc::Error MetalGUI::_Reset()
{
    const MetalApplication * const app = static_cast<const MetalApplication * const>(_app);

    vc::Texture::UnloadAllGuiTextures();
    ImGui_ImplMetal_Shutdown();
#if !defined(VENOM_PLATFORM_IOS)
    ImGui_ImplGlfw_Shutdown();
#endif

    return _Initialize();
}

void MetalGUI::_ClearFonts()
{
    ImGui::GetIO().Fonts->Clear();
}

void MetalGUI::_AddFont(const char* fontPath, float fontSize, const uint16_t* glyphRanges)
{
    ImGuiIO& io = ImGui::GetIO();
    ImFontConfig iconsConfig;
    iconsConfig.MergeMode = true;
    iconsConfig.GlyphOffset.y = 2.0f;
    //iconsConfig.PixelSnapH = true;
    iconsConfig.GlyphMinAdvanceX = fontSize;
    iconsConfig.OversampleH = 3;
    iconsConfig.OversampleV = 3;
    io.Fonts->AddFontFromFileTTF(fontPath, fontSize, &iconsConfig, glyphRanges);
}

void MetalGUI::_AddFont(const char* fontPath, float fontSize)
{
    ImGuiIO& io = ImGui::GetIO();
    ImFontConfig iconsConfig;
    iconsConfig.OversampleH = 3;
    iconsConfig.OversampleV = 3;
    ImFont * font = io.Fonts->AddFontFromFileTTF(fontPath, fontSize, &iconsConfig);
    if (font == nullptr) {
        vc::Log::Error("Failed to load font: %s", fontPath);
    } else {
        io.FontDefault = font;
    }
}

void MetalGUI::_SetNextWindowPos(const vcm::Vec2& pos, vc::GUICond cond, const vcm::Vec2& pivot)
{
    ImGui::SetNextWindowPos(ImVec2(pos.x, pos.y), static_cast<ImGuiCond>(cond), ImVec2(pivot.x, pivot.y));
}

void MetalGUI::_SetNextWindowSize(const vcm::Vec2& size, vc::GUICond cond)
{
    ImGui::SetNextWindowSize(ImVec2(size.x, size.y), static_cast<ImGuiCond>(cond));
}

void MetalGUI::_SetNextWindowViewport(vc::GUIViewport viewport)
{
    ImGui::SetNextWindowViewport(reinterpret_cast<ImGuiViewport*>(viewport)->ID);
}

vcm::Vec2 MetalGUI::_GetContentRegionAvail()
{
    const auto& size = ImGui::GetContentRegionAvail();
    return {size.x, size.y};
}

vcm::Vec2 MetalGUI::_GetWindowSize()
{
    const auto& size = ImGui::GetWindowSize();
    return {size.x, size.y};
}

vcm::Vec2 MetalGUI::_GetWindowPos()
{
    const auto& pos = ImGui::GetWindowPos();
    return {pos.x, pos.y};
}

vc::GUIViewport MetalGUI::_GetMainViewport()
{
    return ImGui::GetMainViewport();
}

void MetalGUI::_NewFrame()
{
//    constexpr float clear_color[4] = {0.45f, 0.55f, 0.60f, 1.00f};
//    id<MTLCommandBuffer> commandBuffer = [commandQueue commandBuffer];
//    renderPassDescriptor.colorAttachments[0].clearColor = MTLClearColorMake(clear_color[0] * clear_color[3], clear_color[1] * clear_color[3], clear_color[2] * clear_color[3], clear_color[3]);
//    renderPassDescriptor.colorAttachments[0].texture = drawable.texture;
//    renderPassDescriptor.colorAttachments[0].loadAction = MTLLoadActionClear;
//    renderPassDescriptor.colorAttachments[0].storeAction = MTLStoreActionStore;
//    id <MTLRenderCommandEncoder> renderEncoder = [commandBuffer renderCommandEncoderWithDescriptor:renderPassDescriptor];
//    [renderEncoder pushDebugGroup:@"ImGui demo"];
    
    //ImGui_ImplMetal_NewFrame(renderPassDescriptor);
    //ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGuizmo::SetOrthographic(false);
    ImGuizmo::BeginFrame();
}

void MetalGUI::_Begin(const char* name, bool* p_open, vc::GUIWindowFlags flags)
{
    ImGui::Begin(name, p_open, static_cast<ImGuiWindowFlags>(flags));
}

void MetalGUI::_End()
{
    ImGui::End();
}

void MetalGUI::_Text(const char* fmt, va_list args)
{
    ImGui::TextV(fmt, args);
}

void MetalGUI::_TextColored(const vcm::Vec4& col, const char* fmt, va_list args)
{
    ImGui::TextColoredV(ImVec4(col.x, col.y, col.z, col.w), fmt, args);
}

void MetalGUI::_LabelText(const char* label, const char* fmt, va_list args)
{
    ImGui::LabelTextV(label, fmt, args);
}

void MetalGUI::_Image(const vc::Texture* texture, const vcm::Vec2 & size, bool centering)
{
    void * textureId;
    if (texture->GetGUITextureID(&textureId) != vc::Error::Success) {
        vc::Log::Error("Failed to get GUI texture ID");
        return;
    }

    if (centering) {
        ImVec2 availableSpace = ImGui::GetContentRegionAvail();

        float verticalOffset = (availableSpace.y - size.y) * 0.5f;

        // Draw a black background if a vertical offset is applied
        if (verticalOffset > 0.0f) {
            // Get the current cursor position in screen space
            ImVec2 cursorScreenPos = ImGui::GetCursorScreenPos();

            ImVec2 bgMin = cursorScreenPos;
            ImVec2 bgMax = ImVec2(cursorScreenPos.x + size.x, cursorScreenPos.y + availableSpace.y);

            // Access the window's draw list and draw the background rectangle
            ImGui::GetWindowDrawList()->AddRectFilled(
                bgMin,
                bgMax,
                IM_COL32(4, 4, 4, 255) // Black color
            );

            // Adjust cursor position for vertical centering
            __imageVerticalOffset = ImGui::GetCursorPosY() + verticalOffset;
            ImGui::SetCursorPosY(__imageVerticalOffset);
        }
    }

    // Remove padding from the window
    ImGui::Image(reinterpret_cast<ImTextureID>(textureId), ImVec2(size.x, size.y));
}

bool MetalGUI::_InputText(const char* label, char* buf, size_t buf_size, vc::GUIInputTextFlags flags)
{
    return ImGui::InputText(label, buf, buf_size, static_cast<ImGuiInputTextFlags>(flags));
}

bool MetalGUI::_TreeNode(const char* label)
{
    return ImGui::TreeNode(label);
}

void MetalGUI::_TreePop()
{
    ImGui::TreePop();
}

void MetalGUI::_TreePush(const char* str_id)
{
    ImGui::TreePush(str_id);
}

void MetalGUI::_SeparatorText(const char* text)
{
    ImGui::SeparatorText(text);
}

void MetalGUI::_Separator()
{
    ImGui::Separator();
}

void MetalGUI::_Spacing()
{
    ImGui::Spacing();
}

void MetalGUI::_Dummy(const vcm::Vec2& size)
{
    ImGui::Dummy(ImVec2(size.x, size.y));
}

bool MetalGUI::_SliderFloat(const char* label, float* v, float v_min, float v_max, const char* format)
{
    return ImGui::SliderFloat(label, v, v_min, v_max, format);
}

bool MetalGUI::_SliderFloat3(const char* label, float v[3], float v_min, float v_max, const char* format)
{
    return ImGui::SliderFloat3(label, v, v_min, v_max, format, ImGuiSliderFlags_None);
}

bool MetalGUI::_InputFloat(const char* label, float* v, float step, float step_fast, const char* format,
    vc::GUIColorEditFlags flags)
{
    return ImGui::InputFloat(label, v, step, step_fast, format, static_cast<ImGuiColorEditFlags>(flags));
}

bool MetalGUI::_InputFloat3(const char* label, float v[3], const char* format, vc::GUIColorEditFlags flags)
{
    return ImGui::InputFloat3(label, v, format, static_cast<ImGuiColorEditFlags>(flags));
}

void MetalGUI::_ColorEdit3(const char* label, float col[3], vc::GUIColorEditFlags flags)
{
    ImGui::ColorEdit3(label, col, static_cast<ImGuiColorEditFlags>(flags));
}

bool MetalGUI::_CollapsingHeader(const char* label, vc::GUITreeNodeFlags flags)
{
    return ImGui::CollapsingHeader(label, static_cast<ImGuiTreeNodeFlags>(flags));
}

bool MetalGUI::_Button(const char* label, const vcm::Vec2& size)
{
    return ImGui::Button(label, ImVec2(size.x, size.y));
}

bool MetalGUI::_Checkbox(const char* label, bool* v)
{
    return ImGui::Checkbox(label, v);
}

void MetalGUI::_ProgressBar(float fraction, const vcm::Vec2& size_arg, const char* overlay)
{
    ImGui::ProgressBar(fraction, ImVec2(size_arg.x, size_arg.y), overlay);
}

bool MetalGUI::_Selectable(const char* label, bool selected, vc::GUISelectableFlags flags, const vcm::Vec2& size)
{
    return ImGui::Selectable(label, selected, static_cast<ImGuiSelectableFlags>(flags), ImVec2(size.x, size.y));
}

bool MetalGUI::_BeginCombo(const char* label, const char* preview_value, vc::GUIComboFlags flags)
{
    return ImGui::BeginCombo(label, preview_value, static_cast<ImGuiComboFlags>(flags));
}

void MetalGUI::_EndCombo()
{
    ImGui::EndCombo();
}

bool MetalGUI::_BeginMenu(const char* label, bool enabled)
{
    return ImGui::BeginMenu(label, enabled);
}

void MetalGUI::_EndMenu()
{
    ImGui::EndMenu();
}

bool MetalGUI::_BeginMainMenuBar()
{
    return ImGui::BeginMainMenuBar();
}

void MetalGUI::_EndMainMenuBar()
{
    ImGui::EndMainMenuBar();
}

bool MetalGUI::_BeginMenuBar()
{
    return ImGui::BeginMenuBar();
}

void MetalGUI::_EndMenuBar()
{
    ImGui::EndMenuBar();
}

bool MetalGUI::_BeginChild(const char* str_id, const vcm::Vec2& size, vc::GUIChildFlags childFlags, vc::GUIWindowFlags extra_flags)
{
    return ImGui::BeginChild(str_id, ImVec2(size.x, size.y), static_cast<ImGuiChildFlags>(childFlags),
        static_cast<ImGuiWindowFlags>(extra_flags));
}

void MetalGUI::_EndChild()
{
    ImGui::EndChild();
}

bool MetalGUI::_MenuItem(const char* str, const char* text)
{
    return ImGui::MenuItem(str, text);
}

void MetalGUI::_SetNextItemWidth(float item_width)
{
    ImGui::SetNextItemWidth(item_width);
}

void MetalGUI::_SetItemDefaultFocus()
{
    ImGui::SetItemDefaultFocus();
}

void MetalGUI::_SameLine(float offset_from_start_x, float spacing)
{
    ImGui::SameLine(offset_from_start_x, spacing);
}

void MetalGUI::_PushItemWidth(float item_width)
{
    ImGui::PushItemWidth(item_width);
}

void MetalGUI::_PopItemWidth()
{
    ImGui::PopItemWidth();
}

void MetalGUI::_PushButtonTextAlign(const vcm::Vec2& padding)
{
    ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(padding.x, padding.y));
}

void MetalGUI::_PushWindowPadding(const vcm::Vec2& padding)
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(padding.x, padding.y));
}

void MetalGUI::_PopStyleVar()
{
    ImGui::PopStyleVar();
}

vc::GUIId MetalGUI::_DockSpace(vc::GUIId id, const vcm::Vec2& size, vc::GUIDockNodeFlags flags)
{
    return ImGui::DockSpace(id, ImVec2(size.x, size.y), static_cast<ImGuiDockNodeFlags>(flags));
}

vc::GUIId MetalGUI::_DockSpace(const char* id, const vcm::Vec2& size, vc::GUIDockNodeFlags flags)
{
    vc::GUIId guiId = ImGui::GetID(id);
    return ImGui::DockSpace(guiId, ImVec2(size.x, size.y), static_cast<ImGuiDockNodeFlags>(flags));
}

vc::GUIId MetalGUI::_DockSpaceOverViewport()
{
    return ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());
}

vc::GUIId MetalGUI::_DockSpaceAddNode(vc::GUIId id, vc::GUIDockNodeFlags flags)
{
    return ImGui::DockBuilderAddNode(id, static_cast<ImGuiDockNodeFlags>(flags));
}

void MetalGUI::_DockSpaceRemoveNode(vc::GUIId id)
{
    ImGui::DockBuilderRemoveNode(id);
}

void MetalGUI::_DockSpaceSetNodeSize(vc::GUIId id, const vcm::Vec2& size)
{
    ImGui::DockBuilderSetNodeSize(id, ImVec2(size.x, size.y));
}

vc::GUIId MetalGUI::_DockSpaceSplitNode(vc::GUIId id, vc::GUIDir split_dir, float size_ratio, vc::GUIId* out_id_at_dir,
    vc::GUIId* out_id_at_opposite_dir)
{
    return ImGui::DockBuilderSplitNode(id, static_cast<ImGuiDir>(split_dir), size_ratio, out_id_at_dir, out_id_at_opposite_dir);
}

void MetalGUI::_DockWindow(const char* str_id, vc::GUIId id)
{
    ImGui::DockBuilderDockWindow(str_id, id);
}

void MetalGUI::_DockFinish(vc::GUIId id)
{
    ImGui::DockBuilderFinish(id);
}

void MetalGUI::_OpenPopup(const char* str_id, vc::GUIPopupFlags flags)
{
    ImGui::OpenPopup(str_id, static_cast<ImGuiPopupFlags>(flags));
}

bool MetalGUI::_BeginPopup(const char* str_id, vc::GUIWindowFlags flags)
{
    return ImGui::BeginPopup(str_id, static_cast<ImGuiWindowFlags>(flags));
}

bool MetalGUI::_BeginPopupModal(const char* name, bool* p_open, vc::GUIWindowFlags flags)
{
    return ImGui::BeginPopupModal(name, p_open, static_cast<ImGuiWindowFlags>(flags));
}

bool MetalGUI::_BeginPopupContextItem(const char* str_id, common::GUIPopupFlags flags)
{
    return ImGui::BeginPopupContextItem(str_id, static_cast<ImGuiPopupFlags>(flags));
}

void MetalGUI::_EndPopup()
{
    ImGui::EndPopup();
}

void MetalGUI::_CloseCurrentPopup()
{
    ImGui::CloseCurrentPopup();
}

vc::GUIId MetalGUI::_GetID(const char* str_id)
{
    return ImGui::GetID(str_id);
}

vc::Error MetalGUI::_PreUpdate()
{
    return vc::Error::Success;
}

void MetalGUI::_Render()
{
    const MetalApplication * const app = static_cast<const MetalApplication * const>(_app);

    ImGui::Render();
    ImDrawData* draw_data = ImGui::GetDrawData();
    //ImGui_ImplMetal_RenderDrawData(draw_data, app->GetCurrentGraphicsCommandBuffer()->GetVkCommandBuffer());
}

void MetalGUI::_Test()
{
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_NoCloseButton;
    static bool dockspaceOpen = true;

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoMove;

    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("MainWindow", &dockspaceOpen, window_flags);
    ImGui::PopStyleVar();

    ImGuiID mainDockSpaceId = ImGui::GetID("MainDockSpace");
    ImGui::DockSpace(mainDockSpaceId, ImVec2(0.0f, 0.0f), dockspace_flags);

    ImGui::Begin("AAA", nullptr, ImGuiWindowFlags_NoCollapse);
    {

    }
    ImGui::End();

    ImGui::Begin("BBB", nullptr, ImGuiWindowFlags_NoCollapse);
    {

    }
    ImGui::End();

    static bool sFirstFrame = true;
    if (sFirstFrame)
    {
        sFirstFrame = false;

        const ImVec2 dockspace_size = ImGui::GetContentRegionAvail();
        ImGui::DockBuilderRemoveNode(mainDockSpaceId);
        ImGui::DockBuilderAddNode(mainDockSpaceId, ImGuiDockNodeFlags_DockSpace);
        ImGui::DockBuilderSetNodeSize(mainDockSpaceId, dockspace_size);

        ImGuiID dock_id_left;
        ImGuiID dock_id_right = ImGui::DockBuilderSplitNode(mainDockSpaceId, ImGuiDir_Right, 0.5f, NULL, &dock_id_left);

        ImGui::DockBuilderDockWindow("AAA", dock_id_left);
        ImGui::DockBuilderDockWindow("BBB", dock_id_right);

        ImGui::DockBuilderFinish(mainDockSpaceId);
    }

    ImGui::End();
}
}
}
