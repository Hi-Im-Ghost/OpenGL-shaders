//
// Created by Ghost on 29.03.2022.
//

#include "Mesh.h"

void Mesh::initVAO(Primitives *primitives) {

//    //Ustawienie zmiennycch
//    this->nrOfVertices = primitives->getNrOfVertices();
//    this->nrOfIndices = primitives->getNrOfIndices();
//
//    //Utworzenie VAO
//    glCreateVertexArrays(1, &this->VAO);
//    //Aktywacja do przechowywania w danym buforze wszystkich rzeczy
//    glBindVertexArray(this->VAO);
//
//    //Tworzenie miejsca dla buforów i dodawanie danych
//    glGenBuffers(1, &this->VBO);
//    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
//    //Dane wysyłane do GPU
//    glBufferData(GL_ARRAY_BUFFER, this->nrOfVertices * sizeof(Vertex), primitives->getVertices(), GL_STATIC_DRAW); //STATIC jesli nie zmieniamy obiektow ktore rysujemy
//
//    //EBO
//    //Do indeksowania
//    glGenBuffers(1, &this->EBO);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->nrOfIndices * sizeof(GLuint), primitives->getIndices(), GL_STATIC_DRAW);
//
//    //POSITION
//    //Jak będziemy używać danych wejściowych
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
//    //Włączenie tablicy wierzchołków
//    glEnableVertexAttribArray(0);
//    //COLOR
//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
//    glEnableVertexAttribArray(1);
//    //TEXCOORD
//    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));
//    glEnableVertexAttribArray(2);
//    //NORMAL
//    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
//    glEnableVertexAttribArray(3);
//
//    //Odłączenie tablicy wierzechołków
//    glBindVertexArray(0);
}

void Mesh::initVAO(Vertex *vertexArray, const unsigned int &nrOfVertices, GLuint *indexArray, const unsigned int &nrOfIndices) {
    //Ustawienie zmiennycch
    this->nrOfVertices = nrOfVertices;
    this->nrOfIndices = nrOfIndices;

    //Utworzenie VAO
    glCreateVertexArrays(1, &this->VAO);
    //Aktywacja do przechowywania w danym buforze wszystkich rzeczy
    glBindVertexArray(this->VAO);

    //Tworzenie miejsca dla buforów i dodawanie danych
    glGenBuffers(1, &this->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    //Dane wysyłane do GPU
    glBufferData(GL_ARRAY_BUFFER, this->nrOfVertices * sizeof(Vertex), vertexArray, GL_STATIC_DRAW);

    //EBO
    //Do indeksowania
    glGenBuffers(1, &this->EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->nrOfIndices * sizeof(GLuint), indexArray, GL_STATIC_DRAW);

    //POSITION
    //Jak będziemy używać danych wejściowych
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);
    //COLOR
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
    glEnableVertexAttribArray(1);
    //TEXCOORD
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));
    glEnableVertexAttribArray(2);
    //NORMAL
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(3);

    //Odłączenie tablicy wierzechołków
    glBindVertexArray(0);
}

void Mesh::updateUni(Shader *shader) {
    shader->setMat4fv(this->ModelMatrix, "ModelMatrix");

}

void Mesh::updateMatrix() {
    this->ModelMatrix = glm::mat4(1.f);
    this->ModelMatrix = glm::translate(this->ModelMatrix, this->position);
    this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.x), glm::vec3(1.f, 0.f, 0.f));
    this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.y), glm::vec3(0.f, 1.f, 0.f));
    this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.z), glm::vec3(0.f, 0.f, 1.f));
    this->ModelMatrix = glm::scale(this->ModelMatrix, this->scale);
}

Mesh::Mesh(Vertex *vertexArray, const unsigned int &nrOfVertices, GLuint *indexArray, const unsigned int &nrOfIndices,
           glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) {
    this->position = position;
    this->rotation = rotation;
    this->scale = scale;

    this->initVAO(vertexArray, nrOfVertices, indexArray, nrOfIndices);
    this->updateMatrix();

}

Mesh::Mesh(Primitives *primitive, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) {
    this->position = position;
    this->rotation = rotation;
    this->scale = scale;

    this->initVAO(primitive);
    this->updateMatrix();

}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &this->VAO);
    glDeleteBuffers(1, &this->VBO);
    glDeleteBuffers(1, &this->EBO);
}

void Mesh::setPosition(const glm::vec3 position) {
    this->position = position;
}

void Mesh::setRotation(const glm::vec3 rotation) {
    this->rotation = rotation;
}

void Mesh::setScale(const glm::vec3 setScale) {
    this->scale = scale;
}

void Mesh::move(const glm::vec3 position) {
    this->position += position;
}

void Mesh::rotate(const glm::vec3 rotation) {
    this->rotation += rotation;
}

void Mesh::scaleUp(const glm::vec3 scale) {
    this->scale += scale;
}

void Mesh::update() {

}

void Mesh::render(Shader *shader) {
    //Aktualizacja matrixa i shaderów
    this->updateMatrix();
    this->updateUni(shader);

    shader->use();

    //Aktywacja do przechowywania w danym buforze wszystkich rzeczy
    glBindVertexArray(this->VAO);

    //Rysowania elementów
    glDrawElements(GL_TRIANGLES, this->nrOfIndices, GL_UNSIGNED_INT, 0);
}
