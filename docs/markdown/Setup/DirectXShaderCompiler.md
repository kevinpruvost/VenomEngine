@page dxc_setup DirectX Shader Compiler Setup

@brief Instructions for setting up DirectX Shader Compiler to compile HLSL shaders to SPIR-V.

@section prerequisites Prerequisites
Refer to the official [DXC Prerequisites](https://github.com/microsoft/DirectXShaderCompiler/blob/main/docs/BuildingAndTestingDXC.rst#prerequisites).

@section automated_setup Automated Setup
Use the following Makefile command:
@code{.sh}
make dxc
@endcode

@section manual_setup Manual Setup
Alternatively, you can manually build DXC:
@code{.sh}
mkdir cmake_build
cd cmake_build
cmake ../lib/DirectXShaderCompiler -G <Generator> -C ../lib/DirectXShaderCompiler/cmake/caches/PredefinedParams.cmake -DCMAKE_BUILD_TYPE=Release -DDXC_USE_LIT=On -DLLVM_ENABLE_ASSERTIONS=On -DLLVM_LIT_ARGS="-v"
cmake --build . --target dxc --config Release
@endcode