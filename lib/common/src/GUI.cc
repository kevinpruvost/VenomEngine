///
/// Project: VenomEngineWorkspace
/// @file GUI.cc
/// @date Nov, 07 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/common/plugin/graphics/GUI.h>

#include "venom/common/plugin/graphics/GraphicsApplication.h"

namespace venom
{
namespace common
{
GUI * GUI::s_gui = nullptr;
GUIDrawCallback GUI::s_guiDrawCallback = nullptr;

GUI::GUI()
    : _app(nullptr)
{
    venom_assert(s_gui == nullptr, "GUI::GUI() : s_gui is not nullptr");
    s_gui = this;
}

GUI::~GUI()
{
    s_gui = nullptr;
}

void GUI::SetGraphicsApplication(GraphicsApplication* app)
{
    _app = app;
}

vc::Error GUI::__PreUpdate()
{
    return _PreUpdate();
}
}
}
