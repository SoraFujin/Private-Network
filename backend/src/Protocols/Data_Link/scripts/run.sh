#!/bin/bash

project_root="$(cd "$(dirname "$0")/.." && pwd)"

mkdir -p "$project_root/output"

gcc "$project_root/Data_Link.c" -o "$project_root/output/Data_Link" -Wall -Wextra

if [ $? -eq 0 ]; then
    echo "✅ Compilation completed. Executable is in outpu/Data_Link"
else
    echo "❌ Compilation failed"
fi

echo "-------------------------------------------------------------------------"
echo "Data Link: "
echo

if [ ! -f "$project_root/output/Data_Link" ]; then 
    echo "❌ Executable not found. Please compile first using ./compile.sh"
    exit 1
fi

"$project_root/output/Data_Link"
