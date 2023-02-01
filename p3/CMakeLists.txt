cmake_minimum_required(VERSION 3.10)
include_directories(${PROJECT_SOURCE_DIR}/Chreno 
	${PROJECT_SOURCE_DIR}/Depencencies/GLEW/include 
	${PROJECT_SOURCE_DIR}/Depencencies/GLFW/include 
	${PROJECT_SOURCE_DIR}/Depencencies/glm/include )

aux_source_directory(./Chreno SOURCE)

link_directories(${PROJECT_SOURCE_DIR}/lib)

add_executable(main main.cpp ${SOURCE})

target_link_libraries(main GLEW glfw3  GL pthread dl)

