#pragma once

#define GLEW_STATIC

//'Include'Windows'functions
#include <Windows.h>

//'Include'input'and'output'operations
#include <iostream>

//'OpenGL'and'GLEW'Header'Files'and'Libraries
#include "Include/GL/glew.h"
#include "Include/GL/wglew.h"
//#include "Include/GLFW/glfw3.h"

// GLM include files
#include "Include/glm/glm.hpp"
//#include "glm/gtc/matrix_projection.hpp"
#include "Include/glm/gtc/matrix_transform.hpp"

//#pragma comment(lib,"Libs/glfw3.lib")
#pragma comment(lib,"Libs/glew32s.lib")
#pragma comment(lib,"opengl32.lib")

#pragma comment (linker,"/nodefaultlib:LIBCMT.lib")