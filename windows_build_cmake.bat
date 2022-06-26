cmake -G"Visual Studio 17 2022" -A x64 -S . -B build/msvc_build_x64
cmake -G"Visual Studio 17 2022" -T"ClangCL" -A x64 -S . -B build/clang_build_x64
cmake --build build/msvc_build_x64
cmake --build build/clang_build_x64
pause