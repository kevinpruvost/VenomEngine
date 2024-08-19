# Bazel_Vulkan_Metal

## Description

`Bazel_Vulkan_Metal` is a personal Computer Graphics project I'm doing for learning Vulkan, Metal, Bazel and many other C++ technical notions that I want to learn like `Renderer Debuggers`, `protobuf`, ...


## Setup

I strongly recommend you to use CLion as your IDE for running this project as Bazel is better made for this IDE, especially for C++ projects as Google is directly maintaining it. 

## Debugging

### Useful Links

- [Jetbrains Debugger Renderers](https://www.jetbrains.com/help/clion/qt-tutorial.html#debug-renderers)

### Windows

- Do not forget to setup your default debugger as the one from Visual Studio (`lldb`) because MinGW is using `gdb` and the default settings aren't as helpful as Visual Studio brings a lot of `Debugger Renderers` (`.natvis` files).

### macOS

- 