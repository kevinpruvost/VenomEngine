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

check_ruby:
	@ruby -v > /dev/null 2>&1 || make __install_ruby

__install_ruby:
	@echo "Installing Ruby..."
ifeq ($(OS),Windows_NT)
	# Windows: Using RubyInstaller
	@echo "Installing Ruby on Windows..."
	@powershell -Command "Invoke-WebRequest -Uri 'https://github.com/oneclick/rubyinstaller2/releases/download/RubyInstaller-3.2.5-1/rubyinstaller-devkit-3.2.5-1-x64.exe' -OutFile 'rubyinstaller.exe'"
	@powershell -Command "Start-Process -Wait -FilePath 'rubyinstaller.exe'"
	@powershell -Command "Remove-Item 'rubyinstaller.exe'"
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		# Linux: Using apt-get
		@sudo apt-get update && sudo apt-get install -y ruby-full
	else ifeq ($(UNAME_S),Darwin)
		# macOS: Using Homebrew
		@brew install ruby
	endif
endif

compile_shaders_glsl: check_ruby
	ruby ./resources/compile_shaders.rb compile_glsl

compile_shaders: check_ruby
	ruby ./resources/compile_shaders.rb compile

validate_shaders:
	@echo "Validating all SPIR-V shaders..."
	@echo "If no output is shown, the shaders are valid."
	@for spv in ./assets/shaders/compiled/*.spv; do \
		spirv-val $$spv; \
	done

clean_shaders:
	@ruby ./resources/compile_shaders.rb clean

clean:
	@echo "Cleaning bazel solution..."
	bazel clean
	make clean_shaders

.PHONY: debug release debug_run release_run fast fast_run clean