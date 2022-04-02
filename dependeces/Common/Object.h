#ifndef RENDERABLE_OBJECT_H
#define RENDERABLE_OBJECT_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <iostream>
#include "stb_image.h"
#include "objloader.hpp"

bool LoadTexture(GLuint shaderID, const std::string& texturePath, GLchar *name,GLint out[]);

class Object {
protected:
    std::vector<glm::vec3> vertices, normals;
    std::vector<glm::vec2> uvs;
    size_t vertexCount;
    GLuint vao;
    GLuint vertexBuff;
    GLuint uvBuff;
    GLuint normalBuff;

    GLuint texture;

    GLuint textureID;
    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;
    glm::mat4 modelMatrix, MVP;
    glm::mat4 reflectionMatrix;

protected:
    void initBuffers();

public:
    Object();
    explicit Object(const std::string& objPath);

    ~Object();

    bool loadTexture( GLuint shaderID, const std::string& texturePath, GLchar *name);

    void setTexture(GLuint textureID, GLuint texture);

    bool initFromArray(const std::vector<glm::vec3>& vertices, std::vector<glm::vec3> normals,
                        std::vector<glm::vec2> uvs, GLuint shederID, const std::string& texturePath, GLchar *name);

    bool initFromArray(std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals,
                        std::vector<glm::vec2> uvs);

    bool initFromFile(const std::string& path, GLuint shaderID, const std::string& texturePath, GLchar *name);
    bool initFromFile(const std::string& path);

    void translate(glm::vec3 vector);

    void rotate(glm::vec3 vector,float angle);

    void scale(glm::vec3 vector);

    void draw(GLuint MatrixID, GLuint ViewMatrixID, GLuint ModelMatrixID, bool reflected = false);

    void setProjectionMatrix(glm::mat4 matrix) { projectionMatrix = matrix; }

    void setViewMatrix(glm::mat4 matrix) { viewMatrix = matrix; }

    void setModelMatrix(glm::mat4 matrix) { modelMatrix = matrix; }

    glm::mat4 getModelMatrix(){return modelMatrix;}

    glm::mat4 getReflectionMatrix() {return reflectionMatrix;}

    void setReflectionVector(glm::vec3 newVector);
};

#endif