#include "Primitives.h"

void Primitives::set(const Vertex *vertices, const unsigned int nrOfVertices, const GLuint *indices,
                     const unsigned int nrOfIndices) {
    for (size_t i = 0; i < nrOfVertices; i++)
    {
        this->vertices.push_back(vertices[i]);
    }
    for (size_t i = 0; i < nrOfIndices; i++)
    {
        this->indices.push_back(indices[i]);
    }

}


Quad::Quad(): Primitives() {
    //Tablica wierzchołków
    Vertex vertices[] =
            {
                    //Position								        //Color							        //Texcoords					    //Normals
                    glm::vec3(-0.5f, 0.5f, 0.f),			glm::vec3(1.f, 0.f, 0.f),		glm::vec2(0.f, 1.f),		glm::vec3(0.f, 0.f, 1.f),
                    glm::vec3(-0.5f, -0.5f, 0.f),			glm::vec3(0.f, 1.f, 0.f),		glm::vec2(0.f, 0.f),		glm::vec3(0.f, 0.f, 1.f),
                    glm::vec3(0.5f, -0.5f, 0.f),			glm::vec3(0.f, 0.f, 1.f),		glm::vec2(1.f, 0.f),		glm::vec3(0.f, 0.f, 1.f),
                    glm::vec3(0.5f, 0.5f, 0.f),			glm::vec3(1.f, 1.f, 0.f),		glm::vec2(1.f, 1.f),	    glm::vec3(0.f, 0.f, 1.f)
            };
    //Obliczanie ilości wierzchołków
    unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);
    //Indeksy do określania przy rysowaniu które wierzchołki użyć by nie było duplikatów
    GLuint indices[] =
            {
                    0, 1, 2,
                    0, 2, 3
            };
    //Obliczanie ilości indeksów
    unsigned nrOfIndices = sizeof(indices) / sizeof(GLuint);

    //Utworzenie kwadrata
    this->set(vertices, nrOfVertices, indices, nrOfIndices);
}


Triangle::Triangle() : Primitives() {
    //Tablica wierzchołków
    Vertex vertices[] =
            {
                    //Position								        //Color							        //Texcoords					    //Normals
                    glm::vec3(-0.5f, 0.5f, 0.f),			glm::vec3(1.f, 0.f, 0.f),		glm::vec2(0.f, 1.f),		glm::vec3(0.f, 0.f, 1.f),
                    glm::vec3(-0.5f, -0.5f, 0.f),			glm::vec3(0.f, 1.f, 0.f),		glm::vec2(0.f, 0.f),		glm::vec3(0.f, 0.f, 1.f),
                    glm::vec3(0.5f, -0.5f, 0.f),			glm::vec3(0.f, 0.f, 1.f),		glm::vec2(1.f, 0.f),		glm::vec3(0.f, 0.f, 1.f),
                    glm::vec3(0.5f, 0.5f, 0.f),			glm::vec3(1.f, 1.f, 0.f),		glm::vec2(1.f, 1.f),		glm::vec3(0.f, 0.f, 1.f)
            };
    //Obliczanie ilości wierzchołków
    unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);

    //Indeksy do określania przy rysowaniu które wierzchołki użyć by nie było duplikatów
    GLuint indices[] =
            {
                    0, 1, 2	//Trójkąt 1
            };
    //Obliczanie ilości indeksów
    unsigned nrOfIndices = sizeof(indices) / sizeof(GLuint);

    //Utworzenie trójkąta
    this->set(vertices, nrOfVertices, indices, nrOfIndices);
}

Pyramid::Pyramid() : Primitives() {
    Vertex vertices[] =
            {
                    //Position								//Color							//Texcoords					//Normals
                    //Triangle front
                    glm::vec3(0.f, 0.5f, 0.f),				glm::vec3(1.f, 0.f, 0.f),		glm::vec2(0.5f, 1.f),		glm::vec3(0.f, 0.f, 1.f),
                    glm::vec3(-0.5f, -0.5f, 0.5f),			glm::vec3(0.f, 1.f, 0.f),		glm::vec2(0.f, 0.f),		glm::vec3(0.f, 0.f, 1.f),
                    glm::vec3(0.5f, -0.5f, 0.5f),			glm::vec3(0.f, 0.f, 1.f),		glm::vec2(1.f, 0.f),		glm::vec3(0.f, 0.f, 1.f),

                    //Triangle left
                    glm::vec3(0.f, 0.5f, 0.f),				glm::vec3(1.f, 1.f, 0.f),		glm::vec2(0.5f, 1.f),		glm::vec3(-1.f, 0.f, 0.f),
                    glm::vec3(-0.5f, -0.5f, -0.5f),			glm::vec3(0.f, 0.f, 1.f),		glm::vec2(0.f, 0.f),		glm::vec3(-1.f, 0.f, 0.f),
                    glm::vec3(-0.5f, -0.5f, 0.5f),			glm::vec3(0.f, 0.f, 1.f),		glm::vec2(1.f, 0.f),		glm::vec3(-1.f, 0.f, 0.f),

                    //Triangle back
                    glm::vec3(0.f, 0.5f, 0.f),				glm::vec3(1.f, 1.f, 0.f),		glm::vec2(0.5f, 1.f),		glm::vec3(0.f, 0.f, -1.f),
                    glm::vec3(0.5f, -0.5f, -0.5f),			glm::vec3(0.f, 0.f, 1.f),		glm::vec2(0.f, 0.f),		glm::vec3(0.f, 0.f, -1.f),
                    glm::vec3(-0.5f, -0.5f, -0.5f),			glm::vec3(0.f, 0.f, 1.f),		glm::vec2(1.f, 0.f),		glm::vec3(0.f, 0.f, -1.f),

                    //Triangles right
                    glm::vec3(0.f, 0.5f, 0.f),				glm::vec3(1.f, 1.f, 0.f),		glm::vec2(0.5f, 1.f),		glm::vec3(1.f, 0.f, 0.f),
                    glm::vec3(0.5f, -0.5f, 0.5f),			glm::vec3(0.f, 0.f, 1.f),		glm::vec2(0.f, 0.f),		glm::vec3(1.f, 0.f, 0.f),
                    glm::vec3(0.5f, -0.5f, -0.5f),			glm::vec3(0.f, 0.f, 1.f),		glm::vec2(1.f, 0.f),		glm::vec3(1.f, 0.f, 0.f),
            };
    unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);

    this->set(vertices, nrOfVertices, nullptr, 0);

}
