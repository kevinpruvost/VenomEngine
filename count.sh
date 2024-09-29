(
    find ./lib/common -name "*.cc" -o -name "*.h" | xargs wc -l | awk '!/total$/'
    find ./lib/vulkan -name "*.cc" -o -name "*.h" | xargs wc -l | awk '!/total$/'
    find ./VenomEngine -name "*.cc" -o -name "*.h" | xargs wc -l | awk '!/total$/'
    find ./resources/shaders -name "*.hlsl" -o -name "*.glsl" | xargs wc -l | awk '!/total$/'
) | awk '{total += $1} END {print total}'