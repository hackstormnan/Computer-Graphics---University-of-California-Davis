#pragma once
//gl math
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/trigonometric.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//glew
#include <GL/glew.h>

//glfw
#include <GLFW/glfw3.h>


//c++
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream> 

#define ASSERT(X) if(!(X)) __debugbreak();
#define GL_CHECK_ERROR glGetError() == GL_NO_ERROR