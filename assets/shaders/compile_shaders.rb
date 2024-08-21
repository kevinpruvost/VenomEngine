require 'fileutils'

# Directories
hlsl_dir = './assets/shaders/hlsl'
compiled_dir = './assets/shaders/compiled'
dxc_path = './cmake_build/Release/bin/dxc'

# Ensure the compiled directory exists
FileUtils.mkdir_p(compiled_dir)

# Find all HLSL files in the HLSL directory
hlsl_files = Dir.glob("#{hlsl_dir}/*.hlsl")

# Function to determine shader type based on filename
def shader_type(file)
    case File.basename(file)
        when /vs_/
        when /vertex_/
            'vs_6_0'
        when /ps_/
        when /pixel_/
            'ps_6_0'
        when /cs_/
        when /compute_/
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
        output_file = "#{compiled_dir}/#{File.basename(file, '.hlsl')}.spv"
        type = shader_type(file)
        cmd = "#{dxc_path} -T #{type} -spirv #{file} -Fo #{output_file}"
        puts "Compiling #{file} to #{output_file}..."
        system(cmd)
    end
else
    puts 'Unknown command. Use "clean" to remove compiled shaders or "compile" to compile shaders.'
    exit
end



