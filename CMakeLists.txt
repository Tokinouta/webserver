cmake_minimum_required(VERSION 3.10)
set(CMAKE_C_COMPILER "/data2/hby/programs/llvm/bin/clang")
set(CMAKE_CXX_COMPILER "/data2/hby/programs/llvm/bin/clang++")
set(CMAKE_EXPORT_COMPILE_COMMANDS on)

project(webserver)

add_compile_options(-std=c++20 -stdlib=libc++ -Wall -fexperimental-library)
add_link_options(-stdlib=libc++ -fexperimental-library)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(${LLVM_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/include)
# include_directories(${PROJECT_SOURCE_DIR}/include/db)
# include_directories(${PROJECT_SOURCE_DIR}/include/model)
# include_directories(${PROJECT_SOURCE_DIR}/include/log)

enable_testing()

# 加载子目录
add_subdirectory(src)
add_subdirectory(tests)
