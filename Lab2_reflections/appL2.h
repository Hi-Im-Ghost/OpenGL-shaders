#ifndef UNTITLED3_LAB2_H
#define UNTITLED3_LAB2_H

#include "dependeces/Common/Initialize.h"
#include "dependeces/Common/controls.hpp"
#include "dependeces/Common/Object.h"
GLFWwindow *window;

class appL2 {
private:
    //Kolor do odbicia lustra
    GLint uniColor;
    GLuint programID;
    GLuint MatrixID;
    GLuint ViewMatrixID;
    GLuint ModelMatrixID;
    GLuint TextureID;
    GLuint lightID;

    Object suzie;
    Object mirror;
    Object mirror1;
    Object keyhole;
    Object keyWall;

    Object cube;

public:

    appL2(){};
    ~appL2() = default;
    void CubeReflection();
    void CubeReflection1();
    void Keyhole();
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

        mirror1.initFromFile("resource/floor.obj");
        mirror1.loadTexture(programID, "resource/floor1.png", "myTextureSampler");
        mirror1.translate(glm::vec3(0,-1.5,0));

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
        uniColor = glGetUniformLocation(programID,"overrideColor");
    }

    void update() {
        // Włączenie rysowania i rysowanie, gdy szablon == 1

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(programID);
        glUniform3f(lightID, 2.0f, 2.0f, 2.0f);

        computeMatricesFromInputs();

        //Funkcja do odbicia  cuba
        //CubeReflection();

        //Funkcja do odbicia lustrzanego cuba
        CubeReflection1();

        //Funkcja do robienia dziurki od klucza (na razie dziala tylko z zwyklym odbiciem cuba)
        //Keyhole();


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    void clean() {
        glfwTerminate();
    }
};

void appL2::CubeReflection1() {

    //Ustawienie cuba i narysowanie
    cube.setProjectionMatrix(getProjectionMatrix());
    cube.setViewMatrix(getViewMatrix());
    cube.draw(MatrixID, ViewMatrixID, ModelMatrixID);
    //Uruchomienie stencil buffora
    glEnable(GL_STENCIL_TEST);

    //Konfiguracja stencil buffora
    glStencilFunc(GL_ALWAYS, 1, 0xFF); //zawsze pisz do maski
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glStencilMask(0xFF); //zapis do odpowiedniej maski
    glDepthMask(GL_FALSE); //Nie pisanie w glebokosci
    glClear(GL_STENCIL_BUFFER_BIT); //czyść buffor
    //Rysowanie podlogi
    mirror1.setProjectionMatrix(getProjectionMatrix());
    mirror1.setViewMatrix(getViewMatrix());
    mirror1.draw(MatrixID, ViewMatrixID, ModelMatrixID);
    //Rysowanie odbicia kostki
    glStencilFunc(GL_EQUAL, 1, 0xFF); //Dla wartosci 1
    glStencilMask(0x00); //Nie pisz
    glDepthMask(GL_TRUE); //Zapis do glebokosci
    /* Rysowanie cuba odwroconego */
    glm::mat4 cbmodmat = cube.getModelMatrix();
    cube.translate(glm::vec3(0.0f, -3.0f, 0.0f));
    cube.scale(glm::vec3(1, -1, 1));
    cube.draw(MatrixID, ViewMatrixID, ModelMatrixID);
    cube.translate(glm::vec3(0.0f, 3.0f, 0.0f));
    glUniform3f(uniColor, 0.3f, 0.3f, 0.3f);
    cube.setModelMatrix(cbmodmat);
    glUniform3f(uniColor, 1.0f, 1.0f, 1.0f);

    glDisable(GL_STENCIL_TEST);
    //Obracanie cuba

    cube.rotate(glm::vec3(0, 1, 0), 0.1);
}

void appL2::CubeReflection() {

    //Ustawienie cuba i narysowanie
    cube.setProjectionMatrix(getProjectionMatrix());
    cube.setViewMatrix(getViewMatrix());
    cube.draw(MatrixID, ViewMatrixID, ModelMatrixID);

    /* Rysowanie cuba odwroconego */
    glm::mat4 cbmodmat = cube.getModelMatrix();
    cube.translate(glm::vec3(0.0f, -3.0f, 0.0f));
    cube.scale(glm::vec3(1, -1, 1));
    cube.draw(MatrixID, ViewMatrixID, ModelMatrixID);
    cube.translate(glm::vec3(0.0f, 3.0f, 0.0f));
    cube.setModelMatrix(cbmodmat);

    //Rysowanie podlogi
    glDisable(GL_STENCIL_TEST);
    mirror.setProjectionMatrix(getProjectionMatrix());
    mirror.setViewMatrix(getViewMatrix());
    mirror.draw(MatrixID, ViewMatrixID, ModelMatrixID);
    glEnable(GL_STENCIL_TEST);

    glStencilFunc(GL_NOTEQUAL, 1, 1);


    //Obracanie cuba
    cube.rotate(glm::vec3(0, 1, 0), 0.1);
}

void appL2::Keyhole() {
    glClear(GL_STENCIL_BUFFER_BIT);
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

    glStencilFunc(GL_NOTEQUAL, 1, 1);
    // wall
    keyWall.setProjectionMatrix(getProjectionMatrix());
    keyWall.setViewMatrix(getViewMatrix());
    keyWall.draw(MatrixID, ViewMatrixID, ModelMatrixID);

}

#endif //UNTITLED3_LAB2_H
