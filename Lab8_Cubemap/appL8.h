#ifndef UNTITLED3_APPL8_H
#define UNTITLED3_APPL8_H

#include "dependeces/Common/Initialize.h"
#include "dependeces/Common/controls.hpp"
#include "dependeces/Common/Object.h"


class appL8 {
private:
    GLuint programID;
    GLuint programID2;
    GLuint MatrixID;
    GLuint ViewMatrixID;
    GLuint ModelMatrixID;
    GLuint MatrixID2;
    GLuint ViewMatrixID2;
    GLuint ModelMatrixID2;
    GLuint TextureID;
    GLuint TextureID1;
    GLuint lightID;

    Object cube;
    Object boat;

public:

    appL8(){};
    ~appL8() = default;

    void setUp() {
        window = initGLFW(1024, 768, "Cubemap", nullptr, nullptr);
        if (window != nullptr) initGLEW();

        glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        glfwPollEvents();
        glfwSetCursorPos(window, 1024 / 2, 768 / 2);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        glEnable(GL_DEPTH_TEST);

        //SHADER0
        programID = LoadShaders("shaders/Cubemap.vertexshader", "shaders/Cubemap.fragmentshader");
        TextureID = glGetUniformLocation(programID, "texture0");

        // Get a handle for our "MVP" uniform
        MatrixID = glGetUniformLocation(programID, "MVP");

        cube.initFromFile("resource/cube.obj");
        cube.CubemapTexture("resource/Cubemap_posx.png",
                            "resource/Cubemap_negx.png",
                            "resource/Cubemap_posy.png",
                            "resource/Cubemap_negy.png",
                            "resource/Cubemap_negz.png",
                            "resource/Cubemap_posz.png");

        cube.Cubemap(programID, "texture0", 0);
        cube.scale(glm::vec3(50.0f));


        programID2 = LoadShaders("shaders/Envirmap.vertexshader", "shaders/Envirmap.fragmentshader");
        TextureID1 = glGetUniformLocation(programID2, "texture0");
        // Get a handle for our "MVP" uniform
        MatrixID2 = glGetUniformLocation(programID2, "MVP");
        ViewMatrixID2 = glGetUniformLocation(programID2, "V");
        ModelMatrixID2 = glGetUniformLocation(programID2, "M");
        //avatar.initFromFile("resource/cube.obj");
        boat.initFromFile("resource/Boat.obj");
        boat.scale(glm::vec3(0.02f));
        boat.rotate(glm::vec3(0,1,0),90.0f);
        //boat.scale(glm::vec3(2.0f));
    }

    void update() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(programID);
        computeMatricesFromInputs();
        cube.setProjectionMatrix(getProjectionMatrix());
        cube.setViewMatrix(getViewMatrix());
        cube.drawSkybox(MatrixID,ViewMatrixID,ModelMatrixID);

        glUseProgram(programID2);
        computeMatricesFromInputs();

        boat.setProjectionMatrix(getProjectionMatrix());
        boat.setViewMatrix(getViewMatrix());
        boat.setPositionCamera(programID2,getPosition());
        boat.drawMapModel(MatrixID2, ViewMatrixID2, ModelMatrixID2);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    void clean() {
        glfwTerminate();
    }
};


#endif //UNTITLED3_APPL8_H
