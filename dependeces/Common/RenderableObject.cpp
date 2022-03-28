#include "RenderableObject.h"

void RenderableObject::translate(glm::vec3 vector) {
    modelMatrix = glm::translate(modelMatrix, vector);
}

void RenderableObject::scale(glm::vec3 vector) {
    modelMatrix = glm::scale(modelMatrix, vector);
}

void RenderableObject::setTexture(GLuint textureID, GLuint texture) {
    this->textureID=textureID;
    this->texture=texture;
}
void RenderableObject::rotate(glm::vec3 vector, float angle) {
    modelMatrix = glm::rotate(modelMatrix,glm::radians(angle),vector);
}
bool RenderableObject::initFromArrary(std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals,
                                      std::vector<glm::vec2> uvs, GLuint shederID, std::string texturePath,
                                      GLchar *name) {
    bool check = true;
    this->vertices = vertices;
    this->normals = normals;
    this->uvs = uvs;
    this->vertexCount = vertices.size();
    this->textureID = glGetUniformLocation(shederID, name);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    int width, height, nchan;
    unsigned char *dt = stbi_load(texturePath.c_str(), &width, &height, &nchan, 0);
    if (!dt) {
        puts("Cannot load Texture");
        check = false;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, dt);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(dt);
    initBuffers();

    return check;
}

bool RenderableObject::intFromFile(std::string path, GLuint shaderID, std::string texturePath, GLchar *name) {
    bool check = true;
    loadOBJ(path.c_str(), vertices, uvs, normals);
    this->vertexCount = vertices.size();
    this->textureID = glGetUniformLocation(shaderID, name);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    int width, height, nchan;
    unsigned char *dt = stbi_load(texturePath.c_str(), &width, &height, &nchan, 0);
    if (!dt) {
        puts("Cannot load Texture");
        check = false;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, dt);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(dt);
    initBuffers();

    return check;
}
bool RenderableObject::loadTexture(GLuint shederID, std::string texturePath, GLchar *name) {
    bool check = true;

    this->textureID = glGetUniformLocation(shederID, name);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    int width, height, nchan;
    unsigned char *dt = stbi_load(texturePath.c_str(), &width, &height, &nchan, 0);
    if (!dt) {
        puts("Cannot load Texture");
        check = false;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, dt);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(dt);
    return check;
}
bool RenderableObject::initFromArrary(std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals,
                                      std::vector<glm::vec2> uvs) {
    bool check = true;
    this->vertices = vertices;
    this->normals = normals;
    this->uvs = uvs;
    this->vertexCount = vertices.size();

    initBuffers();
    return check;

}

bool RenderableObject::intFromFile(std::string path) {
    bool check = true;
    loadOBJ(path.c_str(), vertices, uvs, normals);
    this->vertexCount = vertices.size();

    initBuffers();
    return check;
}

RenderableObject::RenderableObject(/* args */) {
    modelMatrix = glm::mat4(1.0);
    this->textureID = 0;
}

RenderableObject::~RenderableObject() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vertexBuff);
    glDeleteBuffers(1, &uvBuff);
    glDeleteBuffers(1, &normalBuff);
}

void RenderableObject::draw(GLuint MatrixID, GLuint ViewMatrixID, GLuint ModelMatrixID) {
    glUniform1i(textureID, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Send our transformation to the currently bound shader,
    // in the "MVP" uniform
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
    glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &modelMatrix[0][0]);
    glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &viewMatrix[0][0]);

    glBindVertexArray(vao);

    MVP = projectionMatrix * viewMatrix * modelMatrix;

    glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &modelMatrix[0][0]);
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}
//

void RenderableObject::initBuffers() {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vertexBuff);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuff);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
            0,        // attribute
            3,        // size
            GL_FLOAT, // type
            GL_FALSE, // normalized?
            0,        // stride
            (void *) 0 // array buffer offset
    );

    glGenBuffers(1, &uvBuff);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuff);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
            1,        // attribute
            2,        // size
            GL_FLOAT, // type
            GL_FALSE, // normalized?
            0,        // stride
            (void *) 0 // array buffer offset
    );

    glGenBuffers(1, &normalBuff);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuff);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(
            2,        // attribute
            3,        // size
            GL_FLOAT, // type
            GL_FALSE, // normalized?
            0,        // stride
            (void *) 0 // array buffer offset
    );

    glBindVertexArray(0);
}
bool LoadTexture(GLuint shederID, std::string texturePath, GLchar *name,GLint out[2]){
    GLuint textureId ,texture;
    textureId = glGetUniformLocation(shederID, name);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    int width, height, nchan;
    unsigned char *dt = stbi_load(texturePath.c_str(), &width, &height, &nchan, 0);
    if (!dt) {
        puts("Cannot load Texture");
        return  false;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, dt);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(dt);
    out[0]=textureId;
    out[1]=texture;
    return true;
}
