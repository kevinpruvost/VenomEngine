#include <common/InputSystem.h>

InputSystem::InputSystem()
    : _callbackKeyDown(nullptr)
    , _callbackWhileKeyDown(nullptr)
    , _callbackKeyUp(nullptr)
    , _callbackKeyRepeat(nullptr)
    , _callbackMouseMove(nullptr)
    , _callbackMouseWheel(nullptr)
    , _callbackMouseDown(nullptr)
    , _callbackMouseWhileDown(nullptr)
    , _callbackMouseUp(nullptr)
{
}

void InputSystem::SetWhileKeyDownCallback(const CallbackContainer<void, Key, KeyModifier>& callback)
{
    _callbackWhileKeyDown = callback;
}

void InputSystem::SetOnKeyDownCallback(const CallbackContainer<void, Key, KeyModifier>& callback)
{
    _callbackKeyDown = callback;
}

void InputSystem::SetOnKeyUpCallback(const CallbackContainer<void, Key, KeyModifier>& callback)
{
    _callbackKeyUp = callback;
}

void InputSystem::SetOnKeyRepeatCallback(const CallbackContainer<void, Key, KeyModifier>& callback)
{
    _callbackKeyRepeat = callback;
}

void InputSystem::SetOnMouseMoveCallback(const CallbackContainer<void, double, double>& callback)
{
    _callbackMouseMove = callback;
}

void InputSystem::SetOnMouseWheelCallback(const CallbackContainer<void, double, double>& callback)
{
    _callbackMouseWheel = callback;
}

void InputSystem::SetOnMouseDownCallback(const CallbackContainer<void, InputSystem::MouseButton, InputSystem::KeyModifier>& callback)
{
    _callbackMouseDown = callback;
}

void InputSystem::SetWhileMouseDownCallback(const CallbackContainer<void, InputSystem::MouseButton, InputSystem::KeyModifier>& callback)
{
    _callbackMouseWhileDown = callback;
}

void InputSystem::SetOnMouseUpCallback(const CallbackContainer<void, InputSystem::MouseButton, InputSystem::KeyModifier>& callback)
{
    _callbackMouseUp = callback;
}