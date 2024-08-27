# Project Compilation

The project can be compiled using Bazel. Follow the instructions for your platform.

## Windows & macOS

Use the following commands:

   ```bash
   bazel build -c opt //:VenomEngine
   bazel run //:VenomEngine
   ```

## Linux

Ensure you know which windowing system you are using (`X11` or `Wayland`). The default setup is based on `X11`.

### Switching to Wayland

Modify the following Bazel `BUILD` files:

- `/lib/GLFW/BUILD`: Comment every instance of `"-D_GLFW_X11"` and uncomment `"-D_GLFW_WAYLAND"`.
- `/lib/vulkan/BUILD`: Comment every instance of `"-D_GLFW_X11"` and uncomment `"-D_GLFW_WAYLAND"`.

### Compilation and Execution

   ```bash
   bazel build -c opt //:VenomEngine
   bazel run //:VenomEngine
   ```
