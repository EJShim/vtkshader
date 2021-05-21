
cmake_minimum_required(VERSION 3.18)
project(vtkShader)

find_package(VTK REQUIRED)

add_executable(vtkshaderexample main.cpp)


target_link_libraries(vtkshaderexample ${VTK_LIBRARIES})

