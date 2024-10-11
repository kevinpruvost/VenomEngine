///
/// Project: VenomEngine
/// @file VenomEngine.h
/// @date Aug, 26 2024
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#pragma once

#include <venom/common/plugin/PluginManager.h>
#include <venom/common/ECS.h>
#include <venom/common/VenomSettings.h>

namespace venom
{
namespace common
{
/**
 * @brief Main class of the engine
 * This class will be the main entry point of the engine.
 * It will contain all the main components of the engine, as singleton are harder to manage,
 * especially for unit tests and memory management.
 */
class VENOM_COMMON_API VenomEngine
{
private:
    VenomEngine();
public:
    ~VenomEngine();

    /**
     * @brief Gets instance of VenomEngine
     * @return VenomEngine *
     */
    static VenomEngine * GetInstance();

    /**
     * @brief Runs the engine
     * @param argc
     * @param argv
     * @return error if any, refer to Error enum for more information
     */
    static Error RunEngine(int argc, char ** argv);

public:
    std::unique_ptr<PluginManager> pluginManager;

private:
    std::unique_ptr<DLL_Cache> __dllCache;
    std::unique_ptr<ECS> __ecs;
    std::unique_ptr<VenomSettings> __settings;
};
}
}
