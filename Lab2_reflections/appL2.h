#ifndef UNTITLED3_LAB2_H
#define UNTITLED3_LAB2_H

#include "dependeces/Common/Initialize.h"
#include "dependeces/Common/controls.hpp"
#include "dependeces/Common/Object.h"
GLFWwindow *window;

class appL2 {
private:
    GLuint programID;
    GLuint MatrixID;
    GLuint ViewMatrixID;
    GLuint ModelMatrixID;
    GLuint TextureID;
    GLuint lightID;

    Object suzie;
    Object mirror;
    Object keyhole;
    Object keyWall;

    Object cube;

public:

    appL2(){};
    ~appL2() = default;
    void setUp() {
        window = initGLFW(1024, 768, "Reflections", nullptr, nullptr);
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

        programID = LoadShaders("shaders/StandardShading.vertexshader", "shaders/StandardShading.fragmentshader");

        TextureID = glGetUniformLocation(programID, "myTextureSampler");

        // Get a handle for our "MVP" uniform
        MatrixID = glGetUniformLocation(programID, "MVP");
        ViewMatrixID = glGetUniformLocation(programID, "V");
        ModelMatrixID = glGetUniformLocation(programID, "M");
        lightID = glGetUniformLocation(programID, "LightPosition_worldspace");

        suzie.initFromFile("resource/suzanne.obj");
        suzie.loadTexture(programID, "resource/uvmap.png", "myTextureSampler");

        mirror.initFromFile("resource/floor.obj");
        mirror.loadTexture(programID, "resource/floor.png", "myTextureSampler");
        mirror.translate(glm::vec3(0,-1.5,0));

        keyhole.initFromFile("resource/circle.obj");
        keyhole.translate(glm::vec3(0,0,3));

        keyWall.initFromFile("resource/floor.obj");
        keyWall.loadTexture(programID, "resource/black.png", "myTextureSampler");
        keyWall.rotate(glm::vec3(0,1,1), 180);
        keyWall.translate(glm::vec3(0,3,0));

        cube.initFromFile("resource/cube.obj");
        cube.loadTexture(programID, "resource/uvmap.png", "myTextureSampler");

        //glm::mat4 MatR = glm::scale(cube.getModelMatrix(), glm::vec3(0.0f, -1.0f, 0.0f));
    }

    void update() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glUseProgram(programID);
        glUniform3f(lightID, 2.0f, 2.0f, 2.0f);

        computeMatricesFromInputs();

        /* Stencil / Keyhole */
        glColorMask(0,0,0,0);
        glEnable(GL_STENCIL_TEST);

        // Rysowanie bez żadnych warunków
        glStencilFunc(GL_ALWAYS, 1,1);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

        glDisable(GL_DEPTH_TEST);
        keyhole.setProjectionMatrix(getProjectionMatrix());
        keyhole.setViewMatrix(getViewMatrix());
        keyhole.draw(MatrixID, ViewMatrixID, ModelMatrixID);
        glEnable(GL_DEPTH_TEST);

        // Włączenie rysowania i rysowanie, gdy szablon == 1
        glColorMask(1,1,1,1);
        glStencilFunc(GL_EQUAL, 1, 1);

        // Bufor szablonu tylko do odczytu
        glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
        // Rysowanie obiektów widocznych przez keyhole
        suzie.setProjectionMatrix(getProjectionMatrix());
        suzie.setViewMatrix(getViewMatrix());
        suzie.draw(MatrixID, ViewMatrixID, ModelMatrixID);

        // Move object downwards
        suzie.translate(glm::vec3(0.0f, -3.0f, 0.0f));
        // Scale object by -1 effectively turning it upside down
        suzie.scale(glm::vec3(1, -1, 1));
        // Draw 'reflection'
        suzie.draw(MatrixID, ViewMatrixID, ModelMatrixID);
        // Take the object back where it came from
        suzie.translate(glm::vec3(0.0f, 3.0f, 0.0f));
        // Reset scaling to normal
        suzie.setModelMatrix(glm::mat4(1));

        // Rysowanie, gdy szablon != 1
        glStencilFunc(GL_NOTEQUAL, 1, 1);
        //glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

        cube.setProjectionMatrix(getProjectionMatrix());
        cube.setViewMatrix(getViewMatrix());
        cube.draw(MatrixID, ViewMatrixID, ModelMatrixID);

        /* Cube Reflection */
        glm::mat4 cbmodmat = cube.getModelMatrix();
        cube.translate(glm::vec3(0.0f, -3.0f, 0.0f));
        cube.scale(glm::vec3(1, -1, 1));
        cube.draw(MatrixID, ViewMatrixID, ModelMatrixID);
        cube.translate(glm::vec3(0.0f, 3.0f, 0.0f));
        cube.setModelMatrix(cbmodmat);
        cube.rotate(glm::vec3(0, 1, 0), 0.1);

        glDisable(GL_STENCIL_TEST);
        //glClear(GL_STENCIL_BUFFER_BIT);
        mirror.setProjectionMatrix(getProjectionMatrix());
        mirror.setViewMatrix(getViewMatrix());
        mirror.draw(MatrixID, ViewMatrixID, ModelMatrixID);
        glEnable(GL_STENCIL_TEST);

        glStencilFunc(GL_NOTEQUAL, 1, 1);
        //glDisable(GL_STENCIL_TEST);
        // wall
        keyWall.setProjectionMatrix(getProjectionMatrix());
        keyWall.setViewMatrix(getViewMatrix());
        keyWall.draw(MatrixID, ViewMatrixID, ModelMatrixID);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    void clean() {
        glfwTerminate();
    }
};

#endif //UNTITLED3_LAB2_H
