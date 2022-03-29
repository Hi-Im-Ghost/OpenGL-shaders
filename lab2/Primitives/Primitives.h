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
#include <vector>
#include "Vertex.h"

class Primitives {
private:
    //Wierzchołki
    std::vector<Vertex> vertices;
    //Indeksy
    std::vector<GLuint> indices;

public:

    Primitives() {}
    virtual ~Primitives() {}

    //Metoda do tworzenia prymitywów
    void set(const Vertex* vertices, const unsigned nrOfVertices, const GLuint* indices, const unsigned nrOfIndices);

    //Metoda do pobierania wierzchołków
    inline Vertex* getVertices();
    //Metoda do pobierania indeksów
    inline GLuint* getIndices();
    //Metoda do pobierania ilości wierzchołków
    inline const unsigned getNrOfVertices();
    //Metoda do pobierania ilości indeksów
    inline const unsigned getNrOfIndices();
};

class Triangle : public Primitives
{
public:
    Triangle();
};

class Quad : public Primitives
{
public:
    Quad();
};