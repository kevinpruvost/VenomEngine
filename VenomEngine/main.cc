///
/// Project: Bazel_Vulkan_Metal
/// File: main.cc
/// Date: 8/18/2024
/// Description:
/// Author: Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <stdio.h>
#include <common/Application.h>

int main()
{
    venom::Application app;
    app.LoadApi(venom::Application::ApiType::Vulkan);
    if (const venom::Error err = app.run(); err != venom::Error::Success)
    {
        printf("Failed to run application: %d\n", static_cast<int>(err));
        return 1;
    }
    return 0;
}