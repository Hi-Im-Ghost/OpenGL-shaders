//
// Created by cezar on 25.03.2022.
//

#ifndef UNTITLED3_INITIALIZE_H
#define UNTITLED3_INITIALIZE_H

#define GLFW_INCLUDE_NONE
#include <iostream>
#include <GLFW/glfw3.h>
#include <GL/glew.h>
GLFWwindow * initGLFW(int width,int height,std::string title,GLFWmonitor *monitor, GLFWwindow *share);
bool initGLEW();
#endif //UNTITLED3_INITIALIZE_H
