///
/// Project: VenomEngineWorkspace
/// @file ContextAppleInfo.mm
/// @date Mar, 12 2025
/// @brief 
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/common/context/apple/ContextAppleInfo.h>
#include <venom/common/Log.h>

// Define and initialize the global variables to nil
id<MTLDevice> sharedMetalDevice = nil;
CAMetalLayer *sharedMetalLayer = nil;

void setGlobaMetallDevice(id<MTLDevice> device)
{
    sharedMetalDevice = device;
}

void setGlobalMetalLayer(CAMetalLayer *layer)
{
    sharedMetalLayer = layer;
}

id<MTLDevice> getGlobalMetalDevice(void)
{
    return sharedMetalDevice;
}

CAMetalLayer *getGlobalMetalLayer(void)
{
    return sharedMetalLayer;
}
