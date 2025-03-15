///
/// Project: VenomEngine
/// @file Context.cc
/// @date Aug, 18 2024
/// @brief
/// @author Pruvost Kevin | pruvostkevin (pruvostkevin0@gmail.com)
///
#include <venom/common/Context.h>
#include <venom/common/Log.h>

#include <venom/common/plugin/graphics/GraphicsSettings.h>

#include <venom/common/Config.h>

#include <venom/common/plugin/graphics/GraphicsApplication.h>

// GLFW Context
#if !defined(VENOM_DISABLE_GLFW)
#include <venom/common/context/ContextGLFW.h>
#endif

// Apple Context
#if defined(VENOM_PLATFORM_APPLE)
#include <venom/common/context/ContextApple.h>
#endif

namespace venom::common
{
Context * Context::s_context = nullptr;

ScreenVideoMode::ScreenVideoMode(int width, int height)
    : __width(width)
    , __height(height)
{
}

ScreenVideoMode::~ScreenVideoMode()
{
}

void ScreenVideoMode::SortRefreshRates()
{
    std::sort(__refreshRates.begin(), __refreshRates.end(), std::greater<int>());
}

bool ScreenVideoMode::operator<(const ScreenVideoMode& mode) const
{
    return __width < mode.__width && __height < mode.__height;
}

bool ScreenVideoMode::operator>(const ScreenVideoMode& mode) const
{
    return __width > mode.__width;
}

bool ScreenVideoMode::operator==(const ScreenVideoMode& s) const
{
    return __width == s.__width && __height == s.__height;
}

void ScreenVideoMode::__AddRefreshRate(int rate)
{
    __refreshRates.emplace_back(rate);
}

Screen::Screen()
{
}

Screen::~Screen()
{
}

void Screen::AddVideoMode(int width, int height, int refreshRate)
{
    ScreenVideoMode mode(width, height);
    auto ite = std::find(__modes.begin(), __modes.end(), mode);
    if (ite == __modes.end()) {
        __modes.emplace_back(mode);
        ite = __modes.end() - 1;
    }
    ite->__AddRefreshRate(refreshRate);
}

void Screen::SortVideoModes()
{
    std::sort(__modes.begin(), __modes.end(), [](const auto & mode1, const auto & mode2) {
        if (mode1.__width == mode2.__width) return mode1.__height > mode2.__height;
        return mode1.__width > mode2.__width;
    });
    for (auto & mode : __modes) {
        mode.SortRefreshRates();
    }
}

Context::Context()
    : __keyboardModifierState(0x0000)
    , __mousePos{0.0, 0.0}
    , __mouseLastPos{0.0, 0.0}
    , __shouldClose(false)
    , _currentScreenIndex(0)
    , _currentVideoModeIndex(0)
    , _fullscreen(false)
    , _scale(1.0f)
#if defined(VENOM_PLATFORM_MOBILE)
    , __virtualJoystickPressed(false)
    , __virtualCameraPressed(false)
    , __virtualJoystickReleased(false)
    , __virtualCameraReleased(false)
    , __triggerPreviousAction(false)
    , __triggerNextAction(false)
    , __virtualJoystickMovement{0.0, 0.0}
    , __virtualCameraRotation{0.0, 0.0}
#endif
{
    venom_assert(s_context == nullptr, "Context::Context() : Context already exists");
    s_context = this;
    for (int i = 0; i < __keyboardState.size(); ++i) __keyboardState[i] = InputState::Unknown;
    for (int i = 0; i < __mouseState.size(); ++i) __mouseState[i] = InputState::Unknown;
}

Context::~Context()
{
    s_context = nullptr;
}

Context* Context::CreateContext()
{
    ContextType contextType = vc::Config::GetContextType();
    Context * context = nullptr;
    switch (contextType)
    {
#if defined(VENOM_PLATFORM_DESKTOP) && !defined(VENOM_PLATFORM_IOS)
        case ContextType::GLFW:
            context = CreateContextGLFW();
            break;
#endif
#if defined(VENOM_PLATFORM_APPLE)
        case ContextType::Apple:
            context = CreateContextApple();
            break;
#endif
        default:
            vc::Log::Error("Context::CreateContext() : Invalid context type");
            break;
    }
    return context;
}

vc::Error Context::Init()
{
    vc::Error err = _InitContext();
    // Set Initial pos
    _GetCursorPos(__mousePos);
    memcpy(__mouseLastPos, __mousePos, sizeof(__mousePos));

    return err;
}

vc::Error Context::ChangeRefreshRate(int rate)
{
    // Check if valid
    if (rate < 0) return vc::Error::InvalidArgument;
    for (auto refreshRate : _screens[_currentScreenIndex].GetVideoMode(_currentVideoModeIndex).GetRefreshRates())
    {
        if (rate == refreshRate)
        {
            _currentRefreshRate = rate;
            _width = _screens[_currentScreenIndex].GetVideoMode(_currentVideoModeIndex).GetWidth();
            _height = _screens[_currentScreenIndex].GetVideoMode(_currentVideoModeIndex).GetHeight();
            return _UpdateRefreshRate();
        }
    }
    return vc::Error::InvalidArgument;
}

vc::Error Context::ChangeVideoMode(int index)
{
    // Check if valid
    if (index < 0 || index >= _screens[_currentScreenIndex].GetVideoModeCount()) return vc::Error::InvalidArgument;
    _currentVideoModeIndex = index;
    _width = _screens[_currentScreenIndex].GetVideoMode(_currentVideoModeIndex).GetWidth();
    _height = _screens[_currentScreenIndex].GetVideoMode(_currentVideoModeIndex).GetHeight();
    return _UpdateVideoMode();
}

vc::Error Context::ChangeScreen(int index)
{
    // Check if valid
    if (index < 0 || index >= _screens.size()) return vc::Error::InvalidArgument;
    _currentScreenIndex = index;
    _width = _screens[_currentScreenIndex].GetVideoMode(_currentVideoModeIndex).GetWidth();
    _height = _screens[_currentScreenIndex].GetVideoMode(_currentVideoModeIndex).GetHeight();
    return _UpdateScreen();
}

void Context::PollEvents()
{
    // Check for released keys in the previous frame
    while (!__keyReleasedStack.empty()) {
        __keyboardState[__keyReleasedStack.top()] = InputState::Unknown;
        __keyReleasedStack.pop();
    }
    while (!__mouseReleasedStack.empty()) {
        __mouseState[__mouseReleasedStack.top()] = InputState::Unknown;
        __mouseReleasedStack.pop();
    }
#if defined(VENOM_PLATFORM_MOBILE)
    if (__triggerPreviousAction) {
        __triggerPreviousAction = false;
    }
    if (__triggerNextAction) {
        __triggerNextAction = false;
    }
    if (__virtualJoystickReleased) {
        __virtualJoystickReleased = false;
    }
    if (__virtualCameraReleased) {
        __virtualCameraReleased = false;
    }
#endif

    _PollEvents();

    __shouldClose = _ShouldClose();

    // Mouse movement
    __mouseLastPos[0] = __mousePos[0];
    __mouseLastPos[1] = __mousePos[1];
    //memcpy(__mouseLastPos, __mousePos, sizeof(__mousePos));
    _GetCursorPos(__mousePos);
}

vc::Error Context::Run(int argc, const char * argv[])
{
    venom_assert(_runLoopFunction, "Context::Run() : No run loop function set");
    venom_assert(_preRunLoopFunction, "Context::Run() : No post run loop function set");
    if (_preRunLoopFunction() != vc::Error::Success) return vc::Error::Failure;
    while (!ShouldClose()) {
        _runLoopFunction();
        PollEvents();
    }
    vc::GraphicsApplication::Get()->PreClose();
    return vc::Error::Success;
}
}
