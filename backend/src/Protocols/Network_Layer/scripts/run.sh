#!/bin/bash

project_root="$(cd "$(dirname "$0")/.." && pwd)"

mkdir -p "$project_root/output"

gcc "$project_root/Network_Layer.c" -o "$project_root/output/Network_Layer" -Wall -Wextra

if [ $? -eq 0 ]; then
    echo "✅ Compilation completed. Executable is in outpu/Network_Layer"
else
    echo "❌ Compilation failed"
fi

echo "-------------------------------------------------------------------------"
echo "Network_Layer.c:"
echo

if [ ! -f "$project_root/output/Network_Layer" ]; then 
    echo "❌ Executable not found. Please compile first using ./compile.sh"
    exit 1
fi

"$project_root/output/Network_Layer"
