cmake_minimum_required(VERSION 3.10)
set(CMAKE_C_COMPILER "/data2/hby/programs/gcc/bin/gcc")
set(CMAKE_CXX_COMPILER "/data2/hby/programs/gcc/bin/g++")

project(webserver)

add_compile_options(-std=c++20 -Wall -Werror)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# include_directories(${PROJECT_SOURCE_DIR}/include)
# include_directories(${PROJECT_SOURCE_DIR}/include/db)
# include_directories(${PROJECT_SOURCE_DIR}/include/model)
# include_directories(${PROJECT_SOURCE_DIR}/include/log)
# 加载子目录
add_subdirectory(src)