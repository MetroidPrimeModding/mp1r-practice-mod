#!/bin/zsh -xe

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
cd "${SCRIPT_DIR}/.."
xxd -i romfs/ShaderData/imgui.bin > src/files/imgui_shader.h
clang++ extern/imgui/misc/fonts/binary_to_compressed_c.cpp -o "${SCRIPT_DIR}/binary_to_compressed_c"
"${SCRIPT_DIR}/binary_to_compressed_c" scripts/fonts-shrunk/JetBrainsMonoNL-Regular.ttf JetBrainsMonoNL > src/files/JetBrainsMonoNL-Regular.h