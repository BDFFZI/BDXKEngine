cmake_minimum_required(VERSION 3.0)
string(REGEX MATCH "[^/]*$" Project_Name "${CMAKE_CURRENT_BINARY_DIR}")
project(${Project_Name})
# 头文件
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")
# 资源文件
file(COPY "Assets" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/Assets")
# 源文件
file(GLOB_RECURSE BDXKEngine "BDXKEngine/*.cpp" "BDXKEngine/*.h")
file(GLOB_RECURSE BDXKEditor "BDXKEditor/*.cpp" "BDXKEditor/*.h")
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/Scripts/Main.cpp" [[
#include "BDXKEngine/BDXKEngine.h"
int main(){BDXKEngine::Run();}
]])
# Visual Studio 额外处理
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${BDXKEngine})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${BDXKEditor})
# 编译
add_compile_options("/utf-8" "/MTd" "/std:c++20")
add_executable(${CMAKE_PROJECT_NAME} 
"${CMAKE_CURRENT_BINARY_DIR}/Scripts/Main.cpp"
 ${BDXKEngine} 
 ${BDXKEditor}
)
