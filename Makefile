TARGET = VenomEngine
DOXYGEN = doxygen
DOC_FOLDER = docs
DOXYFILE = Doxyfile
EXTERNAL_LIBS = lib/external
DIRECTXSHADERCOMPILER = $(EXTERNAL_LIBS)/DirectXShaderCompiler
# Detect the platform
ifeq ($(OS),Windows_NT)
    MKDIR_CMD = cmd /c mkdir
else
    MKDIR_CMD = mkdir -p
endif

# all: something

package:
	# if on macOS, cmake build
	if [ "$(shell uname)" = "Darwin" ]; then \
		mkdir -p build_release;\
		cd build_release && cmake .. -DCMAKE_BUILD_TYPE=Release;\
		cmake --build . --config Release -j 8;\
		cpack -G DragNDrop -C Release --config ./CPackConfig.cmake;\
	else
	  echo "Not supported on this platform";\
  	fi


# Generates and open doc for visualization
docs:
	cd $(DOC_FOLDER) && $(DOXYGEN) $(DOXYFILE)

dxc:
	-$(MKDIR_CMD) cmake_build/dxc
	# You can ignore the generator flag if you want to use the default one
	cd cmake_build/dxc && \
	cmake ../$(DIRECTXSHADERCOMPILER) -C ../$(DIRECTXSHADERCOMPILER)/cmake/caches/PredefinedParams.cmake -DCMAKE_BUILD_TYPE=Release -DDXC_USE_LIT=On -DLLVM_ENABLE_ASSERTIONS=On -DLLVM_LIT_ARGS="-v" && \
	cmake --build . --target dxc --config Release

check_ruby:
	@echo "Checking Ruby..."
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

compile_shaders_msl: check_ruby
	ruby ./resources/compile_shaders.rb compile_msl

compile_shaders_debug: check_ruby
	ruby ./resources/compile_shaders.rb compile_debug

validate_shaders:
	@echo "Validating all SPIR-V shaders..."
	@echo "If no output is shown, the shaders are valid."
	@for spv in ./resources/shaders/compiled/*.spv; do \
		spirv-val $$spv; \
	done

clean_shaders:
	@ruby ./resources/compile_shaders.rb clean

clean:
	@echo "Cleaning solution..."
	make clean_shaders

.PHONY: clean docs