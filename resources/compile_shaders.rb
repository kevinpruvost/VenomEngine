require 'fileutils'

# Directories
hlsl_dir = './resources/shaders/hlsl'
glsl_dir = './resources/shaders/glsl'
compiled_dir = './resources/shaders/compiled'
dxc_folder_path = './cmake_build/dxc/Release/bin'
dxc_path = "#{dxc_folder_path}/dxc"

# Ensure the compiled directory exists
FileUtils.mkdir_p(compiled_dir)

# Find all HLSL files in the HLSL directory
hlsl_files = Dir.glob("#{hlsl_dir}/*.hlsl")
glsl_files = Dir.glob("#{glsl_dir}/*.glsl")

def output_file_create_name(file, compiled_dir)
    filename_parts = File.basename(file).split('.')
    "#{compiled_dir}/#{filename_parts[0]}.#{filename_parts[1]}.spv"
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

# if arg0 == clean, then remove compiled files, else if arg0 == compile, then compile
if ARGV[0] == 'clean'
    puts 'Cleaning compiled shaders...'
    FileUtils.rm_rf(compiled_dir)
    exit
elsif ARGV[0] == 'compile'
    # Compile each HLSL file to SPIR-V
    hlsl_files.each do |file|
        output_file = output_file_create_name(file, compiled_dir)
        type = shader_type(file)
        cmd = "#{dxc_path} -T #{type} -spirv #{file} -Fo #{output_file}"
        puts "Compiling #{file} to #{output_file} with command[#{cmd}]..."
        system(cmd)
    end
elsif ARGV[0] == 'compile_debug'
    # Compile each HLSL file to SPIR-V
    hlsl_files.each do |file|
        output_file = output_file_create_name(file, compiled_dir)
        type = shader_type(file)
        cmd = "#{dxc_path} -Zi -T #{type} -spirv #{file} -Fo #{output_file}"
        puts "Compiling #{file} to #{output_file}..."
        system(cmd)
    end
elsif ARGV[0] == 'compile_glsl'
    # Compile each GLSL file to SPIR-V
    glsl_files.each do |file|
        output_file = output_file_create_name(file, compiled_dir)
        cmd = "glslangValidator -V #{file} -o #{output_file}"
        puts "Compiling #{file} to #{output_file}..."
        system(cmd)
    end
else
    puts 'Unknown command. Use "clean" to remove compiled shaders or "compile" to compile shaders.'
    exit
end



