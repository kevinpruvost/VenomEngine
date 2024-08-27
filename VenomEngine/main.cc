///
/// Project: VenomEngine
/// @file main.cc
/// @date Aug, 18 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <stdio.h>
#include <venom/common/plugin/graphics/GraphicsApplication.h>
#include <venom/common/VenomEngine.h>
#include <venom/common/Log.h>
#include <venom/common/Resources.h>

#include <venom/common/plugin/graphics/Mesh.h>

int main(int argc, char** argv)
{
    vc::Resources::InitializeFilesystem(argv);
    vc::VenomEngine * engine = vc::VenomEngine::GetInstance();
    vc::GraphicsApplication * app = vc::GraphicsApplication::Create();

    if (const vc::Error err = app->Run(); err != vc::Error::Success)
    {
        printf("Failed to run application: %d\n", static_cast<int>(err));
        return 1;
    }
    engine->pluginManager.UnloadPlugins();
    return 0;
}
