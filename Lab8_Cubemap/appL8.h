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
    GLuint TextureID;
    GLuint lightID;

    Object cube;
    Object avatar;

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
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

        glEnable(GL_DEPTH_TEST);
        // Accept fragment if it's closer to the camera than the former one
        glDepthFunc(GL_LESS);


        glEnable(GL_BLEND);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        //SHADER0
        //programID = LoadShaders("shaders/Cubemap.vertexshader", "shaders/Cubemap.fragmentshader");
        programID = LoadShaders("shaders/Envirmap.vertexshader", "shaders/Envirmap.fragmentshader");
        TextureID = glGetUniformLocation(programID, "texture0");

        // Get a handle for our "MVP" uniform
        MatrixID = glGetUniformLocation(programID, "MVP");
        ViewMatrixID = glGetUniformLocation(programID, "V");
        ModelMatrixID = glGetUniformLocation(programID, "M");
        lightID = glGetUniformLocation(programID, "LightPosition_worldspace");


        cube.initFromFile("resource/cube.obj");
        cube.CubemapTexture("resource/Cubemap_posx.png",
                            "resource/Cubemap_negx.png",
                            "resource/Cubemap_posy.png",
                            "resource/Cubemap_negy.png",
                            "resource/Cubemap_negz.png",
                            "resource/Cubemap_posz.png");

        cube.Cubemap(programID, "texture0", 0);
        cube.scale(glm::vec3(50.0f));


        avatar.initFromFile("resource/Avatar.obj");
        avatar.scale(glm::vec3(0.2f));
    }

    void update() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(programID);
        glUniform3f(lightID, 2.0f, 2.0f, 2.0f);
        computeMatricesFromInputs();

        glDepthMask(GL_FALSE);
        cube.setProjectionMatrix(getProjectionMatrix());
        cube.setViewMatrix(getViewMatrix());
        cube.draw(MatrixID,ViewMatrixID,ModelMatrixID,false,0);
        glDepthMask(GL_TRUE);

        avatar.setProjectionMatrix(getProjectionMatrix());
        avatar.setViewMatrix(getViewMatrix());
        avatar.draw(MatrixID, ViewMatrixID, ModelMatrixID,false,0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    void clean() {
        glfwTerminate();
    }
};


#endif //UNTITLED3_APPL8_H
