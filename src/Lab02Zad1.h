#ifndef UNTITLED3_LAB02ZAD1_H
#define UNTITLED3_LAB02ZAD1_H

#include "dependeces/Common/Initialize.h"
#include "dependeces/Common/controls.hpp"
#include "dependeces/Common/RenderableObject.h"
GLFWwindow *window;

class Lab02Zad1 {
private:
    GLuint programID;
    GLuint MatrixID;
    GLuint ViewMatrixID;
    GLuint ModelMatrixID;
    GLuint TextureID;
    GLuint lightID;
    RenderableObject cube;



public:

    Lab02Zad1(){};
    ~Lab02Zad1(){};
    void setUp() {
        window = initGLFW(1024, 768, "OpenglSample", NULL, NULL);
        if (window != NULL) initGLEW();

        glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
        glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_DISABLED);

        glfwPollEvents();
        glfwSetCursorPos(window, 1024 / 2, 768 / 2);
        glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

        glEnable(GL_DEPTH_TEST);
        // Accept fragment if it closer to the camera than the former one
        glDepthFunc(GL_LESS);

        glEnable(GL_BLEND);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        programID = LoadShaders("shaders/StandardShading.vertexshader", "shaders/StandardShading.fragmentshader");

        TextureID = glGetUniformLocation(programID, "myTextureSampler");

        // Get a handle for our "MVP" uniform
        MatrixID = glGetUniformLocation(programID, "MVP");
        ViewMatrixID = glGetUniformLocation(programID, "V");
        ModelMatrixID = glGetUniformLocation(programID, "M");
        lightID = glGetUniformLocation(programID, "LightPosition_worldspace");

        cube.intFromFile("resource/cube.obj");
        cube.loadTexture(programID,"resource/uvmap.png","myTextureSampler");
    }

    void update() {
        glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
        glUseProgram(programID);
        glUniform3f(lightID,2.0f,2.0f,2.0f);

        computeMatricesFromInputs();
        cube.setProjectionMatrix(getProjectionMatrix());
        cube.setViewMatrix(getViewMatrix());
        cube.draw(MatrixID,ViewMatrixID,ModelMatrixID);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    void clean() {
        glfwTerminate();
    }
};

#endif //UNTITLED3_LAB02ZAD1_H
