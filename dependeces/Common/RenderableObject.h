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

bool LoadTexture(GLuint shederID, std::string texturePath, GLchar *name,GLint out[]);
class RenderableObject {
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

    void initBuffers();

public:
    RenderableObject(/* args */);

    ~RenderableObject();

    bool loadTexture( GLuint shederID, std::string texturePath, GLchar *name);

    void setTexture(GLuint textureID,GLuint texture);

    bool initFromArrary(std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals,
                        std::vector<glm::vec2> uvs, GLuint shederID, std::string texturePath, GLchar *name);

    bool initFromArrary(std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals,
                        std::vector<glm::vec2> uvs);

    bool intFromFile(std::string path, GLuint shaderID, std::string texturePath, GLchar *name);
    bool intFromFile(std::string path);

    void translate(glm::vec3 vector);

    void rotate(glm::vec3 vector,float angle);

    void scale(glm::vec3 vector);

    void draw(GLuint MatrixID, GLuint ViewMatrixID, GLuint ModelMatrixID);

    void setProjectionMatrix(glm::mat4 matrix) { projectionMatrix = matrix; }

    void setViewMatrix(glm::mat4 matrix) { viewMatrix = matrix; }

    void setModelMatrix(glm::mat4 matrix) { modelMatrix = matrix; }

    glm::mat4 getModelMatrix(){return modelMatrix;}



};

#endif;
