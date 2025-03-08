///
/// Project: VenomEngineWorkspace
/// @file ContextPlugin.cc
/// @date Mar, 09 2025
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <ContextPlugin.h>
#include <Context_GLFW.h>

namespace venom
{
namespace context
{
namespace glfw
{
GLFWContextPlugin::GLFWContextPlugin()
{
}

GLFWContextPlugin::~GLFWContextPlugin()
{

}

vc::Context* GLFWContextPlugin::CreateContext()
{
    return new ContextGLFW();
}
}
}
}

extern "C" EXPORT vc::ContextPlugin* createContextPlugin() {
    return new venom::context::glfw::GLFWContextPlugin();
}