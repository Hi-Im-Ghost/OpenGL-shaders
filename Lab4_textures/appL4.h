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
    GLuint tex;

    Object suzie;
    Object lion;
    Object avatar;
    Object cube;
    Object frame;

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
        int windowWidth = 1024;
        int windowHeight = 768;
        glfwGetFramebufferSize(window, &windowWidth, &windowHeight);


        glEnable(GL_DEPTH_TEST);
        // Accept fragment if it's closer to the camera than the former one
        glDepthFunc(GL_LESS);

        glEnable(GL_BLEND);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

        programID = LoadShaders("shaders/Texture.vertexshader", "shaders/Texture.fragmentshader");

        TextureID1 = glGetUniformLocation(programID, "texture0");
        TextureID2 = glGetUniformLocation(programID, "texture1");
        cube.isTwoTex = glGetUniformLocation(programID, "isTwoTex");

        // Get a handle for our "MVP" uniform
        MatrixID = glGetUniformLocation(programID, "MVP");
        ViewMatrixID = glGetUniformLocation(programID, "V");
        ModelMatrixID = glGetUniformLocation(programID, "M");
        lightID = glGetUniformLocation(programID, "LightPosition_worldspace");

        frame.initFromFile("resource/cube.obj");
        frame.loadTexture(programID, "resource/floor.png", "texture0",4);
        frame.translate(glm::vec3(-6,0,0));

        cube.initFromFile("resource/cube.obj");
        cube.loadTexture(programID, "resource/grass.png", "texture0",0);
        cube.loadTexture2(programID, "resource/uvmap.png", "texture1",0);

        suzie.initFromFile("resource/suzanne.obj");
        suzie.loadTexture(programID, "resource/uvmap.png", "texture0",3);
        //suzie.loadTexture2(programID, "resource/black.png", "texture1",1);
        suzie.translate(glm::vec3(3,0,0));

        lion.initFromFile("resource/Lion.obj");
        lion.loadTexture(programID, "resource/Lion.png", "texture0",2);
        lion.translate(glm::vec3(-3,-1,0));
        lion.scale(glm::vec3(0.02f));

        avatar.initFromFile("resource/Avatar.obj");
        avatar.loadTexture(programID, "resource/Avatar.png", "texture0",1);
        avatar.translate(glm::vec3(6,0,0));
        avatar.scale(glm::vec3(0.03f));


    }

    void update() {

        glBindFramebuffer(GL_FRAMEBUFFER, frame.FramebufferName);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(programID);
        glViewport(0, 0, 256, 256);

        glUniform3f(lightID, 2.0f, 2.0f, 2.0f);
        computeMatricesFromInputs();

        cube.setProjectionMatrix(getProjectionMatrix());
        cube.setViewMatrix(getViewMatrix());
        cube.draw(MatrixID,ViewMatrixID,ModelMatrixID,false,2);

        suzie.setProjectionMatrix(getProjectionMatrix());
        suzie.setViewMatrix(getViewMatrix());
        suzie.draw(MatrixID, ViewMatrixID, ModelMatrixID,false,1);

        avatar.setProjectionMatrix(getProjectionMatrix());
        avatar.setViewMatrix(getViewMatrix());
        avatar.draw(MatrixID, ViewMatrixID, ModelMatrixID,false,1);

        lion.setProjectionMatrix(getProjectionMatrix());
        lion.setViewMatrix(getViewMatrix());
        lion.draw(MatrixID, ViewMatrixID, ModelMatrixID,false,1);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(programID);
        glViewport(0, 0, 1024, 768);

        glUniform3f(lightID, 2.0f, 2.0f, 2.0f);
        computeMatricesFromInputs();

        frame.setProjectionMatrix(getProjectionMatrix());
        frame.setViewMatrix(getViewMatrix());
        frame.draw(MatrixID,ViewMatrixID,ModelMatrixID,false,1);

        cube.setProjectionMatrix(getProjectionMatrix());
        cube.setViewMatrix(getViewMatrix());
        cube.draw(MatrixID,ViewMatrixID,ModelMatrixID,false,2);

        suzie.setProjectionMatrix(getProjectionMatrix());
        suzie.setViewMatrix(getViewMatrix());
        suzie.draw(MatrixID, ViewMatrixID, ModelMatrixID,false,1);

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
