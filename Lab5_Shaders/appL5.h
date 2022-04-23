#ifndef UNTITLED3_LAB5_H
#define UNTITLED3_LAB5_H

#include "dependeces/Common/Initialize.h"
#include "dependeces/Common/controls.hpp"
#include "dependeces/Common/Object.h"


class appL5 {
private:
    GLuint programID;
    GLuint programID1;
    GLuint programID2;
    GLuint programID3;
    GLuint programID4;
    GLuint MatrixID;
    GLuint ViewMatrixID;
    GLuint ModelMatrixID;
    GLuint TextureID1;
    GLuint TextureID2;
    GLuint lightID;

    Object cube;
    Object cube1;
    Object cube2;
    Object avatar;
    Object avatar1;


public:

    appL5(){};
    ~appL5() = default;

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

        //SHADER0
        programID = LoadShaders("shaders/Texture.vertexshader", "shaders/Texture.fragmentshader");

        TextureID1 = glGetUniformLocation(programID, "texture0");
        TextureID2 = glGetUniformLocation(programID, "texture1");

        // Get a handle for our "MVP" uniform
        MatrixID = glGetUniformLocation(programID, "MVP");
        ViewMatrixID = glGetUniformLocation(programID, "V");
        ModelMatrixID = glGetUniformLocation(programID, "M");
        lightID = glGetUniformLocation(programID, "LightPosition_worldspace");

        //SHADER1
        programID1 = LoadShaders("shaders/Shader1.vertexshader", "shaders/Shader1.fragmentshader");

        TextureID1 = glGetUniformLocation(programID1, "texture0");
        TextureID2 = glGetUniformLocation(programID1, "texture1");

        // Get a handle for our "MVP" uniform
        MatrixID = glGetUniformLocation(programID1, "MVP");
        ViewMatrixID = glGetUniformLocation(programID1, "V");
        ModelMatrixID = glGetUniformLocation(programID1, "M");
        lightID = glGetUniformLocation(programID1, "LightPosition_worldspace");

        //SHADER2
        programID2 = LoadShaders("shaders/Shader2.vertexshader", "shaders/Shader2.fragmentshader");

        TextureID1 = glGetUniformLocation(programID2, "texture0");
        TextureID2 = glGetUniformLocation(programID2, "texture1");

        // Get a handle for our "MVP" uniform
        MatrixID = glGetUniformLocation(programID2, "MVP");
        ViewMatrixID = glGetUniformLocation(programID2, "V");
        ModelMatrixID = glGetUniformLocation(programID2, "M");
        lightID = glGetUniformLocation(programID2, "LightPosition_worldspace");

        //SHADER3
        programID3 = LoadShaders("shaders/Shader3.vertexshader", "shaders/Shader3.fragmentshader");

        TextureID1 = glGetUniformLocation(programID3, "texture0");
        TextureID2 = glGetUniformLocation(programID3, "texture1");

        // Get a handle for our "MVP" uniform
        MatrixID = glGetUniformLocation(programID3, "MVP");
        ViewMatrixID = glGetUniformLocation(programID3, "V");
        ModelMatrixID = glGetUniformLocation(programID3, "M");
        lightID = glGetUniformLocation(programID3, "LightPosition_worldspace");

        //SHADER4
        programID4 = LoadShaders("shaders/Shader4.vertexshader", "shaders/Shader4.fragmentshader");

        TextureID1 = glGetUniformLocation(programID4, "texture0");
        TextureID2 = glGetUniformLocation(programID4, "texture1");

        // Get a handle for our "MVP" uniform
        MatrixID = glGetUniformLocation(programID4, "MVP");
        ViewMatrixID = glGetUniformLocation(programID4, "V");
        ModelMatrixID = glGetUniformLocation(programID4, "M");
        lightID = glGetUniformLocation(programID4, "LightPosition_worldspace");

        cube.initFromFile("resource/cube.obj");
        cube.loadTexture(programID, "resource/grass.png", "texture0",0);
        cube.loadTexture2(programID, "resource/uvmap.png", "texture1",0);

        cube1.initFromFile("resource/cube.obj");
        cube1.loadTexture(programID1, "resource/test.png", "texture0",0);
        cube1.translate(glm::vec3(3,0,0));

        cube2.initFromFile("resource/cube.obj");
        cube2.loadTexture(programID2, "resource/black.png", "texture0",0);
        cube2.translate(glm::vec3(-3,0,0));

        avatar.initFromFile("resource/Avatar.obj");
        avatar.loadTexture(programID3, "resource/Avatar.png", "texture0",0);
        avatar.translate(glm::vec3(6,0,0));
        avatar.scale(glm::vec3(0.03f));

        avatar1.initFromFile("resource/Avatar.obj");
        avatar1.loadTexture(programID, "resource/Avatar.png", "texture0",0);
        avatar1.translate(glm::vec3(9,0,0));
        avatar1.scale(glm::vec3(0.03f));

    }

    void update() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(programID);
        glUniform3f(lightID, 2.0f, 2.0f, 2.0f);

        computeMatricesFromInputs();

        cube.setProjectionMatrix(getProjectionMatrix());
        cube.setViewMatrix(getViewMatrix());
        cube.draw(MatrixID,ViewMatrixID,ModelMatrixID,false,2);

        glUseProgram(programID1);
        glUniform3f(lightID, 2.0f, 2.0f, 2.0f);

        computeMatricesFromInputs();
        cube1.setProjectionMatrix(getProjectionMatrix());
        cube1.setViewMatrix(getViewMatrix());
        cube1.draw(MatrixID, ViewMatrixID, ModelMatrixID,false,1);


        glUseProgram(programID2);
        glUniform3f(lightID, 2.0f, 2.0f, 2.0f);

        computeMatricesFromInputs();
        cube2.setProjectionMatrix(getProjectionMatrix());
        cube2.setViewMatrix(getViewMatrix());
        cube2.draw(MatrixID, ViewMatrixID, ModelMatrixID,false,1);

        glUseProgram(programID3);
        glUniform3f(lightID, 2.0f, 2.0f, 2.0f);

        computeMatricesFromInputs();
        avatar.setProjectionMatrix(getProjectionMatrix());
        avatar.setViewMatrix(getViewMatrix());
        avatar.draw(MatrixID, ViewMatrixID, ModelMatrixID,false,1);

        glUseProgram(programID4);
        glUniform3f(lightID, 2.0f, 2.0f, 2.0f);

        computeMatricesFromInputs();
        avatar1.setProjectionMatrix(getProjectionMatrix());
        avatar1.setViewMatrix(getViewMatrix());
        avatar1.draw(MatrixID, ViewMatrixID, ModelMatrixID,false,1);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    void clean() {
        glfwTerminate();
    }
};



#endif //UNTITLED3_LAB5_H
