require 'fileutils'

# Directories
$hlsl_dir = './resources/shaders/hlsl'
$glsl_dir = './resources/shaders/glsl'
$metal_dir = './resources/shaders/msl'
compiled_debug_dir = './resources/shaders/Debug/compiled'
compiled_release_dir = './resources/shaders/Release/compiled'
dxc_folder_path = './cmake_build/dxc/Release/bin'
dxc_path = "#{dxc_folder_path}/dxc"
glslangValidator_path = '/usr/local/bin/glslangValidator'
glslc_path = '/usr/local/bin/glslc'
macos_flag = RUBY_PLATFORM =~ /darwin/ ? 'MACOS' : ''
xcrun = '/usr/bin/xcrun'

# Ensure the compiled directory exists
FileUtils.mkdir_p(compiled_debug_dir)

# Find all HLSL files in the HLSL directory
hlsl_files = Dir.glob("#{$hlsl_dir}/**/*.hlsl")
glsl_files = Dir.glob("#{$glsl_dir}/**/*.glsl")
metal_files = Dir.glob("#{$metal_dir}/**/*.metal")

# exclude files in 'old/'
hlsl_files = hlsl_files.reject { |file| file.include?('old/') }
glsl_files = glsl_files.reject { |file| file.include?('old/') }
metal_files = metal_files.reject { |file| file.include?('old/') }

def output_file_create_name(file, compiled_debug_dir, extension)
    filename_parts = File.basename(file).split('.')
    # try to keep subdirectory if any, e.g. "./resources/shaders/hlsl/pbr_mesh/gbuffer.ps.hlsl" keeps "pbr_mesh"
    # if hlsl, then remove hlsl_dir out of the path
    subdir = if file.include?('.hlsl')
                file.sub($hlsl_dir, '')
             elsif file.include?('.glsl')
                file.sub($glsl_dir, '')
             elsif file.include?('.metal')
                file.sub($metal_dir, '')
             else
                ''
             end
    # if contains multiple /
    if subdir.count('/') > 1
        subdir = subdir.split('/')[1] || ''
        # Create directory if does not exist
        FileUtils.mkdir_p("#{compiled_debug_dir}/#{subdir}")
        if file.include?('.metal')
            "#{compiled_debug_dir}/#{subdir}/#{filename_parts[0]}#{extension}"
        else
            "#{compiled_debug_dir}/#{subdir}/#{filename_parts[0]}.#{filename_parts[1]}#{extension}"
        end
    else
        if file.include?('.metal')
            "#{compiled_debug_dir}/#{filename_parts[0]}#{extension}"
        else
            "#{compiled_debug_dir}/#{filename_parts[0]}.#{filename_parts[1]}#{extension}"
        end
    end
end

# Check if dxc path folder exists
if !File.exist?(dxc_folder_path)
    # Also check at ./cmake_build/dxc/bin because of MacOS
    dxc_folder_path = './cmake_build/dxc/bin'
    dxc_path = "#{dxc_folder_path}/dxc"
    if !File.exist?(dxc_folder_path)
        puts "DXC path does not exist at #{dxc_folder_path}. Please ensure DXC is built (by using 'make dxc')."
        exit
    end
end

# Function to determine shader type based on filename
def shader_type(file)
    # as some files will have 2 '.' in their name, we need to split the name and remove the last element
    filename_parts = File.basename(file).split('.')
    stage = filename_parts[filename_parts.length - 2]
    case stage
        when /vs/, /vertex/, /vert/
            'vs_6_0'
        when /ps/, /pixel/, /frag/, /fs/
            'ps_6_0'
        when /cs/, /compute/, /comp/
            'cs_6_0'
        else
            raise "Unknown shader type for #{file}"
        end
end

def shader_type_glsl(file)
    # as some files will have 2 '.' in their name, we need to split the name and remove the last element
    filename_parts = File.basename(file).split('.')
    stage = filename_parts[filename_parts.length - 2]
    case stage
        when /vert/
            'vert'
        when /frag/
            'frag'
        when /comp/
            'comp'
        else
            raise "Unknown shader type for #{file}"
        end
end

# if arg0 == clean, then remove compiled files, else if arg0 == compile, then compile
if ARGV[0] == 'clean'
    puts 'Cleaning compiled shaders...'
    FileUtils.rm_rf(compiled_debug_dir)
    exit
# Disabling DXC, working with glsl directly as some features are just disabled when compiling to spirv from hlsl (like inverse or partial derivatives)
# elsif ARGV[0] == 'compile'
#     # Compile each HLSL file to SPIR-V
#     hlsl_files.each do |file|
#         output_file = output_file_create_name(file, compiled_debug_dir)
#         type = shader_type(file)
#         cmd = "#{dxc_path} -T #{type} -spirv #{file} -Fo #{output_file} -D#{macos_flag}"
#         puts "Compiling #{file} to #{output_file} with command[#{cmd}]..."
#         system(cmd)
#     end
elsif ARGV[0] == 'compile_debug'
    # Compile each HLSL file to SPIR-V
    hlsl_files.each do |file|
        output_file = output_file_create_name(file, compiled_debug_dir, '.spv')
        type = shader_type(file)
        cmd = "#{dxc_path} -T #{type} -spirv #{file} -Fo #{output_file} -O1 -Zi #{macos_flag} -Fh #{output_file}.pdb"
        puts "Compiling #{file} to #{output_file}..."
        system(cmd)
    end
elsif ARGV[0] == 'compile_msl'
    # Compile each MSL file to .metallib
    metal_files.each do |file|
        output_file = output_file_create_name(file, $metal_dir, '.metallib')
        output_file_ios = output_file_create_name(file, $metal_dir, '_ios.metallib')
        cmd = "#{xcrun} -sdk macosx metal -o #{output_file} #{file}"
        cmd1 = "#{xcrun} -sdk iphoneos metal -o #{output_file_ios} #{file}"
        puts "Compiling #{file} to #{output_file}... [#{cmd}]"
        system(cmd)
        puts "Compiling #{file} to #{output_file_ios}... [#{cmd1}]"
        system(cmd1)
    end
elsif ARGV[0] == 'compile_glsl' or ARGV[0] == 'compile'
    # Compile each GLSL file to SPIR-V
    glsl_files.each do |file|
        output_file = output_file_create_name(file, compiled_debug_dir, '.spv')
        cmd = "#{glslangValidator_path} -V #{file} -o #{output_file} -U#{macos_flag}"
        puts "Compiling #{file} to #{output_file}... [#{cmd}]"
        system(cmd)
    end
    # Compile in release mode
    glsl_files.each do |file|
        output_file = output_file_create_name(file, compiled_release_dir, '.spv')
        shader_stage = shader_type_glsl(file)
        cmd = "#{glslc_path} -fshader-stage=#{shader_stage} -O #{file} -o #{output_file} -D#{macos_flag}"
        puts "Compiling in release mode: #{file} to #{output_file}... [#{cmd}]"
        system(cmd)
    end
else
    puts 'Unknown command. Use "clean" to remove compiled shaders or "compile" to compile shaders.'
    exit
end



