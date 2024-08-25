///
/// Project: VenomEngine
/// @file main.cc
/// @date Aug, 18 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <stdio.h>
#include <common/Application.h>
#include <common/Log.h>
#include <common/Resources.h>

int main(int argc, char** argv)
{
    vc::Resources::InitializeFilesystem(argv);

    vc::Application app;
    app.LoadApi(vc::Application::ApiType::Vulkan);
    vc::Log::LogToFile("bonjour");
    if (const vc::Error err = app.Run(); err != vc::Error::Success)
    {
        printf("Failed to run application: %d\n", static_cast<int>(err));
        return 1;
    }
    return 0;
}
