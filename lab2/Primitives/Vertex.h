#pragma once

// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

struct Vertex
{
    //Pozycja
    glm::vec3 position;
    //Kolor
    glm::vec3 color;
    //Współrzędne tekstury
    glm::vec2 texcoord;
    glm::vec3 normal;
};

