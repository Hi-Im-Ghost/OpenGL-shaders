#ifndef UNTITLED3_APPL6_H
#define UNTITLED3_APPL6_H
#include "dependeces/Common/Initialize.h"
#include "dependeces/Common/controls.hpp"
#include "dependeces/Common/Object.h"

class appL6 {
private:
    GLuint programID;
    GLuint MatrixID;
    GLuint ViewMatrixID;
    GLuint ModelMatrixID;
    GLuint TextureID;
    GLuint lightID;

    Object cube;

public:

    appL6(){};
    ~appL6() = default;
    void setUp() {
        window = initGLFW(1024, 768, "Light", nullptr, nullptr);
        if (window != nullptr) initGLEW();

        glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
        glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_DISABLED);

        glfwPollEvents();
        glfwSetCursorPos(window, 1024 / 2, 768 / 2);
        glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

        glEnable(GL_DEPTH_TEST);
        // Accept fragment if it's closer to the camera than the former one
        glDepthFunc(GL_LESS);

        glEnable(GL_BLEND);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        programID = LoadShaders("shaders/Light.vertexshader", "shaders/Light.fragmentshader");

        TextureID = glGetUniformLocation(programID, "texture0");

        // Get a handle for our "MVP" uniform
        MatrixID = glGetUniformLocation(programID, "MVP");
        ViewMatrixID = glGetUniformLocation(programID, "V");
        ModelMatrixID = glGetUniformLocation(programID, "M");
        lightID = glGetUniformLocation(programID, "LightPosition_worldspace");

        cube.isTwoTex = glGetUniformLocation(programID, "isTwoTex");
        cube.initFromFile("resource/cube.obj");
        cube.loadTexture(programID, "resource/uvmap.png", "texture0",0);

    }

    void update() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(programID);
        //Pozycja światła
        //glUniform3f(lightID, getPosition().x,getPosition().y,getPosition().z);
        glUniform3f(lightID, 0.0f, 2.0f, -2.0f);
        computeMatricesFromInputs();

        cube.setProjectionMatrix(getProjectionMatrix());
        cube.setViewMatrix(getViewMatrix());
        cube.draw(MatrixID,ViewMatrixID,ModelMatrixID,false,1);
        cube.rotate(glm::vec3(1, 1, 0), 0.1);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    void clean() {
        glfwTerminate();
    }
};

#endif //UNTITLED3_APPL6_H
