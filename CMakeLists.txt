cmake_minimum_required(VERSION 3.0.0)
project(nvm VERSION 0.1.0)

add_executable(nvm "src/main.c" "src/common.h" "src/ops.h" "src/tot_file.h" "src/tot_file.c" "src/compiler.h" "src/compiler.c" "src/nvm.h" "src/nvm.c" "src/mem.h"  )