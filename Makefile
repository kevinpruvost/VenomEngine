TARGET = VenomEngine

all: fast_run

debug:
	bazel build //:$(TARGET) --compilation_mode=dbg

release:
	bazel build //:$(TARGET) --compilation_mode=opt

debug_run: debug
	bazel run //:$(TARGET) --compilation_mode=dbg

release_run: release
	bazel run //:$(TARGET) --compilation_mode=opt

fast:
	bazel build //:$(TARGET) --compilation_mode=fastbuild

fast_run: fast
	bazel run //:$(TARGET) --compilation_mode=fastbuild

dxc:
	-mkdir -p cmake_build
	# You can ignore the generator flag if you want to use the default one
	cd cmake_build && \
	cmake ../lib/DirectXShaderCompiler -C ../lib/DirectXShaderCompiler/cmake/caches/PredefinedParams.cmake -DCMAKE_BUILD_TYPE=Release -DDXC_USE_LIT=On -DLLVM_ENABLE_ASSERTIONS=On -DLLVM_LIT_ARGS="-v" && \
	cmake --build . --target dxc --config Release

compile_shader:
	-mkdir ./assets/shaders/compiled
	./cmake_build/Release/bin/dxc -T vs_6_0 -spirv ./assets/shaders/hlsl/vertex_shader.hlsl   -Fo ./assets/shaders/compiled/vertex_shader.spv
	./cmake_build/Release/bin/dxc -T ps_6_0 -spirv ./assets/shaders/hlsl/pixel_shader.hlsl -Fo ./assets/shaders/compiled/fragment_shader.spv


clean:
	bazel clean

.PHONY: debug release debug_run release_run fast fast_run clean