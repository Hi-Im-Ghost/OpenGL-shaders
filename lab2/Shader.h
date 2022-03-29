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
#include <SOIL2.h>
#include <iostream>
#include <fstream>

class Shader {
private:
    GLuint id;

    //Metoda która zwraca kod shadera
    std::string loadShaderFile(char* str);

    //Metoda która ładuje shader
    GLuint loadShader(GLenum type, char *filename);

    //Metoda do łączenia z sobą shaderów
    void linkProgram(GLuint vertexShader, GLuint fragmentShader);

public:
    Shader(char* vertexFile,char*fragmentFile);


    ~Shader();

    //Uniform
    void use();

    void unuse();

    void set1i(GLint value, const GLchar* name);


    void set1f(GLfloat value, const GLchar* name);


    void setVec2f(glm::fvec2 value, const GLchar* name);


    void setVec3f(glm::fvec3 value, const GLchar* name);


    void setVec4f(glm::fvec4 value, const GLchar* name);


    void setMat3fv(glm::mat3 value, const GLchar* name, GLboolean transpose = GL_FALSE);


    void setMat4fv(glm::mat4 value, const GLchar* name, GLboolean transpose = GL_FALSE);

};


