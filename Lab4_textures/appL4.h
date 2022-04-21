#ifndef UNTITLED3_LAB4_H
#define UNTITLED3_LAB4_H

#include "dependeces/Common/Initialize.h"
#include "dependeces/Common/controls.hpp"
#include "dependeces/Common/Object.h"


class appL4 {
private:
    GLuint programID;
    GLuint MatrixID;
    GLuint ViewMatrixID;
    GLuint ModelMatrixID;
    GLuint TextureID1;
    GLuint TextureID2;
    GLuint lightID;

    Object suzie;
    Object lion;
    Object avatar;
    Object cube;

public:

    appL4(){};
    ~appL4() = default;

    void setUp() {
        window = initGLFW(1024, 768, "Textures", nullptr, nullptr);
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
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

        programID = LoadShaders("shaders/Texture.vertexshader", "shaders/Texture.fragmentshader");

        TextureID1 = glGetUniformLocation(programID, "texture0");
        TextureID2 = glGetUniformLocation(programID, "texture1");

        // Get a handle for our "MVP" uniform
        MatrixID = glGetUniformLocation(programID, "MVP");
        ViewMatrixID = glGetUniformLocation(programID, "V");
        ModelMatrixID = glGetUniformLocation(programID, "M");
        lightID = glGetUniformLocation(programID, "LightPosition_worldspace");


        cube.initFromFile("resource/cube.obj");
        cube.loadTexture(programID, "resource/grass.png", "texture0");
        cube.loadTexture2(programID, "resource/uvmap.png", "texture1");

        suzie.initFromFile("resource/suzanne.obj");
        suzie.loadTexture(programID, "resource/fog.png", "texture0");
        suzie.loadTexture2(programID, "resource/black.png", "texture1");
        suzie.translate(glm::vec3(3,0,0));

        lion.initFromFile("resource/Lion.obj");
        lion.loadTexture(programID, "resource/Lion.png", "texture0");
        lion.translate(glm::vec3(-3,-1,0));
        lion.scale(glm::vec3(0.02f));

        avatar.initFromFile("resource/Avatar.obj");
        avatar.loadTexture(programID, "resource/Avatar.png", "texture0");
        avatar.translate(glm::vec3(6,0,0));
        avatar.scale(glm::vec3(0.03f));

    }

    void update() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(programID);
        glUniform3f(lightID, 2.0f, 2.0f, 2.0f);

        computeMatricesFromInputs();

        cube.setProjectionMatrix(getProjectionMatrix());
        cube.setViewMatrix(getViewMatrix());
        cube.draw(MatrixID,ViewMatrixID,ModelMatrixID,false,2);

        suzie.setProjectionMatrix(getProjectionMatrix());
        suzie.setViewMatrix(getViewMatrix());
        suzie.draw(MatrixID, ViewMatrixID, ModelMatrixID,false,2);

        avatar.setProjectionMatrix(getProjectionMatrix());
        avatar.setViewMatrix(getViewMatrix());
        avatar.draw(MatrixID, ViewMatrixID, ModelMatrixID,false,1);

        lion.setProjectionMatrix(getProjectionMatrix());
        lion.setViewMatrix(getViewMatrix());
        lion.draw(MatrixID, ViewMatrixID, ModelMatrixID,false,1);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    void clean() {
        glfwTerminate();
    }
};



#endif //UNTITLED3_LAB4_H
