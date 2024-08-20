# Bazel_Vulkan_Metal | Bazel_VenomEngine

## Description

`Bazel_Vulkan_Metal` is a personal Computer Graphics project I'm doing for learning `Vulkan`, `Metal`, `Bazel` and many other C++ technical notions that I want to learn like `Renderer Debuggers`, `protobuf`, ...


## Setup

I strongly recommend you to use CLion as your IDE for running this project as Bazel is better made for this IDE, especially for C++ projects as Google is directly maintaining it. 

## Compilation

### Windows & MacOS

```bash
bazel build -c opt //:VenomEngine
bazel run //:VenomEngine
```

### Linux

First, you will need to make sure you know which windowing system you are using (`X11` or `Wayland`).

The default setup for `VenomEngine` is based on `X11`, if you want to change it to `Wayland`, you will need to change some of the Bazel `BUILD` files in the project.

- `/lib/GLFW/BUILD`: comment every instance of `"-D_GLFW_X11"` and uncomment every instance of `"-D_GLFW_WAYLAND"`.
- `/lib/vulkan/BUILD`: comment every instance of `"-D_GLFW_X11"` and uncomment every instance of `"-D_GLFW_WAYLAND"`.

Then, you can compile and run the project with the following commands:


```bash
bazel build -c opt //:VenomEngine
bazel run //:VenomEngine
```

## Debugging

### Useful Links

- [Jetbrains Debugger Renderers](https://www.jetbrains.com/help/clion/qt-tutorial.html#debug-renderers)

### Windows

- Do not forget to setup your default debugger as the one from Visual Studio (`lldb`) because MinGW is using `gdb` and the default settings aren't as helpful as Visual Studio brings a lot of `Debugger Renderers` (`.natvis` files).

### macOS

- 