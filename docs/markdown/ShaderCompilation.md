# Shader Compilation

The shaders in this project are compiled using the DirectX Shader Compiler. Below are the steps to compile them.

## Guideline

Follow this guideline for shader compilation: [SPIR-V CodeGen](https://github.com/microsoft/DirectXShaderCompiler/wiki/SPIR‐V-CodeGen).

## Command Line Options

Refer to the supported command line options here: [SPIR-V Command Line Options](https://github.com/Microsoft/DirectXShaderCompiler/blob/main/docs/SPIR-V.rst#supported-command-line-options).

## Automated Compilation

To compile all shaders, use:

   ```bash
   make compile_shaders
   ```

For shaders with debugging info:

   ```bash
   make compile_shaders_debug
   ```

For GLSL:

   ```bash
   make compile_shaders_glsl
   ```

## Manual Compilation

You can also compile shaders manually:

   ```bash
   mkdir ./resources/shaders/compiled
   ./cmake_build/Release/bin/dxc -T vs_6_0 -spirv ./resources/shaders/hlsl/vertex_shader.hlsl -Fo ./resources/shaders/compiled/vertex_shader.spv
   ./cmake_build/Release/bin/dxc -T ps_6_0 -spirv ./resources/shaders/hlsl/pixel_shader.hlsl -Fo ./resources/shaders/compiled/fragment_shader.spv
   ```

Refer to [Shader Profiles](https://learn.microsoft.com/en-us/windows/win32/direct3dhlsl/dx-graphics-hlsl-models) for more information.
