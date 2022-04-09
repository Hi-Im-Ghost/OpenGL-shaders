#ifndef UNTITLED3_INITIALIZE_H
#define UNTITLED3_INITIALIZE_H

#define GLFW_INCLUDE_NONE
#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>
using namespace glm;
GLFWwindow * initGLFW(int width,int height,std::string title,GLFWmonitor *monitor, GLFWwindow *share);
bool initGLEW();
#endif //UNTITLED3_INITIALIZE_H
