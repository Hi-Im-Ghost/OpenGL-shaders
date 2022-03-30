#pragma once
// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/controls.hpp>
#include <common/stb_image.h>
#include <iostream>
#include <fstream>
#include "Shader.h"
#include "Texture.h"
#include "Primitives/Primitives.h"
#include "Primitives/Vertex.h"

class Mesh {
private:
    //Liczba wierzchołków
    unsigned nrOfVertices;
    //Indeksy do określania przy rysowaniu które wierzchołki użyć by nie było duplikatów
    unsigned nrOfIndices;
    //Buffory
    //Pojemnik
    GLuint VAO;
    //Miejsce dla bufforów i danych
    GLuint VBO;
    //Do indeksowania
    GLuint EBO;

    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    glm::mat4 ModelMatrix;
    //Metoda do tworzenia bufforów i konfiguracja dla danego primitywu
    void initVAO(Primitives *primitives);
    //Metoda do tworzenia bufforów i konfiguracja dla dowolnej tablicy wierzchołków
    void initVAO(Vertex* vertexArray, const unsigned& nrOfVertices, GLuint* indexArray, const unsigned& nrOfIndices);

    void updateUni(Shader* shader);

    void updateMatrix();


public:
    //Konstruktor dla tablicy wierzechołków
    Mesh(Vertex* vertexArray, const unsigned& nrOfVertices, GLuint* indexArray, const unsigned& nrOfIndices, glm::vec3 position = glm::vec3(0.f), glm::vec3 rotation = glm::vec3(0.f), glm::vec3 scale = glm::vec3(1.f));
    //Konstruktor dla prymitywów
    Mesh(Primitives* primitives, glm::vec3 position = glm::vec3(0.f), glm::vec3 rotation = glm::vec3(0.f), glm::vec3 scale = glm::vec3(1.f));

    //Destruktor
    ~Mesh();


    //SETERY
    void setPosition(const glm::vec3 position);


    void setRotation(const glm::vec3 rotation);


    void setScale(const glm::vec3 setScale);

    //Metoda do poruszania
    void move(const glm::vec3 position);

    //Metoda do obracania
    void rotate(const glm::vec3 rotation);

    //Metoda do skalowania
    void scaleUp(const glm::vec3 scale);

    //Metoda do aktualizacji
    void update();

    //Metoda do rysowania
    void render(Shader* shader);

};