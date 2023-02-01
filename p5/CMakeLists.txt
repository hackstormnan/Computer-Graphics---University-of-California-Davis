cmake_minimum_required(VERSION 3.10)#PROJECT(main)
include_directories(${PROJECT_SOURCE_DIR}/RayTracer 
	${PROJECT_SOURCE_DIR}/Dependencies/GLFW/include 
	${PROJECT_SOURCE_DIR}/Dependencies/GLEW/include 
	${PROJECT_SOURCE_DIR}/Dependencies/glm/include)

aux_source_directory(./RayTracer SOURCE)

add_definitions(-std=c++11)

link_directories(${PROJECT_SOURCE_DIR}/lib)

add_executable(RayTracer main.cpp ${SOURCE})

target_link_libraries(RayTracer GL  glfw3 GLEW pthread dl)

