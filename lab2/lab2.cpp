// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

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
#include "Shader.h"


struct Vertex
{
    glm::vec3 position;
    glm::vec3 color;
    //Współrzędne tekstury
    glm::vec2 texcoord;
    glm::vec3 normal;
};

Vertex vertices[]=
{
        //Position                           //Color                           //Textcoords                 //NORMAL
        glm::vec3(-0.5f,0.5f,0.f),  glm::vec3(1.f,0.f,0.f),   glm::vec2(0.f,1.f),  glm::vec3(0.f,0.f,-1.f),
        glm::vec3(-0.5f,-0.5f,0.f), glm::vec3(0.f,1.f,0.f),   glm::vec2(0.f,0.f),  glm::vec3(0.f,0.f,-1.f),
        glm::vec3(0.5f,-0.5f,0.f),  glm::vec3(0.f,0.f,1.f),   glm::vec2(1.f,0.f),  glm::vec3(0.f,0.f,-1.f),
        glm::vec3(0.5f,0.5f,0.f),   glm::vec3(1.f,1.f,0.f),   glm::vec2(1.f,1.f),  glm::vec3(0.f,0.f,-1.f)

};
unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);

//Indeksy do określania przy rysowaniu które wierzchołki użyć by nie było duplikatów
GLuint  indices[] =
{
    0, 1, 2, //1 trójkąt
    0, 2, 3 //2 trójkąt
};
unsigned nrOfIndices = sizeof(indices)/sizeof(GLuint);

//Funkcja do poruszania
void updateInput(GLFWwindow* window,glm::vec3 &position, glm::vec3& rotation, glm::vec3 &scale)
{
    //Jeśli klikne klawisz W to przesune.....
    if(glfwGetKey(window,GLFW_KEY_W) == GLFW_PRESS){
        position.z-=0.01f;
    }
    if(glfwGetKey(window,GLFW_KEY_S) == GLFW_PRESS){
        position.z+=0.01f;
    }
    if(glfwGetKey(window,GLFW_KEY_A) == GLFW_PRESS){
        position.x-=0.01f;
    }
    if(glfwGetKey(window,GLFW_KEY_D) == GLFW_PRESS){
        position.x+=0.01f;
    }
    if(glfwGetKey(window,GLFW_KEY_Q) == GLFW_PRESS){
        rotation.y-=1.f;
    }
    if(glfwGetKey(window,GLFW_KEY_E) == GLFW_PRESS){
        rotation.y+=1.f;
    }
    if(glfwGetKey(window,GLFW_KEY_Z) == GLFW_PRESS){
        scale+=0.1f;
    }
    if(glfwGetKey(window,GLFW_KEY_X) == GLFW_PRESS){
        scale-=0.1f;
    }
}

//Zmiana rozmiaru okna
void resize(GLFWwindow *,int W, int H)
{
    glViewport(0,0,W,H);
}
//Funkcja służąca do obsługi klawiatury
void updateInput(GLFWwindow *window)
{
    //Jeśli naciśniemy przycisk ESC w danym oknie to zamkniemy okno
    if(glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window,GL_TRUE);
    }
}

int main( void )
{
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}
    //Incjalizacja zmiennych dla okna
    const int WINDOW_WIDTH = 640;
    const int WINDOW_HEIGT = 480;
    int framebufferwidth = 0;
    int framebufferheight = 0;

    //Ustawienie wersji OPENGL
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    //Zmienianie rozmiaru okna
    glfwWindowHint(GLFW_RESIZABLE,GL_TRUE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE); //MAC OS

    //Create window
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH,WINDOW_HEIGT,"Lab2",NULL,NULL);

    //Wybranie okna na którym chcemy działać
    glfwGetFramebufferSize(window,&framebufferwidth,&framebufferheight);
    //Zmiana rozmiaru okna bufora
    glfwSetFramebufferSizeCallback(window,resize);
//Używane gdy nie zmieniamy rozmiaru
//    //Wybranie na jakiej części okna rysujemy
//    glViewport(0,0,framebufferwidth,framebufferheight);
    //Ustawienie aktualnego okna
    glfwMakeContextCurrent(window);

    //INIT GLEW
    //Włączenie nowoczesnych bajer dla OpenGL
    glewExperimental = GL_TRUE;
    //Sprawdzenie błedów z glew
    if(glewInit()!= GLEW_OK){
        std::cout << "ERROR::LAB2.CPP::GLEW_INIT"<<"\n";
        glfwTerminate();
    }

    //OPENGL OPTIONS
    //Umożliwia użycie współrzędnej Z
    glEnable(GL_DEPTH_TEST);
    //Usuwanie z rysowania tego co jest za czyms
    glEnable(GL_CULL_FACE);
    //Ustawienie kierunków rysowania
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    //Włączenie blendingu czyli mieszania kolorów
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    //Pozowala robić z prymitywów obiekty, które zostaną wypełnione kolorem
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);


    //INIT SHADER
    Shader core("TransformVertexShader.vertexshader","TextureFragmentShader.fragmentshader");

    //BUFORY VAO,VBO,EBO
    GLuint VAO;
    glCreateVertexArrays(1,&VAO);
    //Aktywacja do przechowywania w danym buforze wszystkich rzeczy
    glBindVertexArray(VAO);

    //Tworzenie miejsca dla buforów i dodawanie danych
    GLuint VBO;
    glGenBuffers(1,&VBO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    //Dane wysyłane do GPU
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW); //STATIC jesli nie zmieniamy obiektow ktore rysujemy
    //Do indeksowania
    GLuint EBO;
    glGenBuffers(1,&EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);

    //POSITION
    //Jak będziemy używać danych wejściowych
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(GLvoid*)offsetof(Vertex,position));
    //Włączenie tablicy wierzchołków
    glEnableVertexAttribArray(0);

    //COLOR
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(GLvoid*)offsetof(Vertex,color));
    glEnableVertexAttribArray(1);

    //TEXCOORD
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,sizeof(Vertex),(GLvoid*)offsetof(Vertex,texcoord));
    glEnableVertexAttribArray(2);

    //NORMAL
    glVertexAttribPointer(3,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(GLvoid*)offsetof(Vertex,normal));
    glEnableVertexAttribArray(3);

    //Odłączenie tablicy wierzechołków
    glBindVertexArray(0);

    //TEXTURE 0
    //Id textury
    GLuint texture0;
    //Generowanie tekstury
    glGenTextures(1,&texture0);
    //Wybranie tekstury
    glBindTexture(GL_TEXTURE_2D,texture0);

    //Powtarzanie tekstury
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    //Antyaliasing i inne takie
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

    int image_width, image_height, channels ;
    unsigned char* image = stbi_load("../Images/wood.png",&image_width,&image_height,&channels,STBI_rgb_alpha);

    if(image)
    {
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,image_width,image_height,0,GL_RGBA,GL_UNSIGNED_BYTE,image);
        //Generowanie kilku obrazów o różnych rozmiarach w zaleznosci od odleglosci
        glGenerateMipmap(GL_TEXTURE_2D);
    }else{
        std::cout << "ERROR::TEXUTRE LOADING" << "\n";
    }

    glActiveTexture(0);
    //Wyłączenie tekstury
    glBindTexture(GL_TEXTURE_2D,0);
    stbi_image_free(image);

    //TEXTURE 1
    //Id textury
    GLuint texture1;
    //Generowanie tekstury
    glGenTextures(1,&texture1);
    //Wybranie tekstury
    glBindTexture(GL_TEXTURE_2D,texture1);

    //Powtarzanie tekstury
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    //Antyaliasing i inne takie
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

    int image_width1, image_height1, channels1 ;
    unsigned char* image1 = stbi_load("../Images/awesomeface.png",&image_width1,&image_height1,&channels1,STBI_rgb_alpha);

    if(image1)
    {
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,image_width1,image_height1,0,GL_RGBA,GL_UNSIGNED_BYTE,image1);
        //Generowanie kilku obrazów o różnych rozmiarach w zaleznosci od odleglosci
        glGenerateMipmap(GL_TEXTURE_2D);
    }else{
        std::cout << "ERROR::TEXUTRE LOADING" << "\n";
    }

    glActiveTexture(0);
    //Wyłączenie tekstury
    glBindTexture(GL_TEXTURE_2D,0);
    stbi_image_free(image1);

    //MATRIX
    glm::vec3 position(0.f);
    glm::vec3 rotation(0.f);
    glm::vec3 scale(1.f);
    //Inicjalizowanie macierzy
    glm::mat4 ModelMatrix(1.f);
    //Operacje
    //Translacja
    ModelMatrix = glm::translate(ModelMatrix,position);
    //Obrót wybranej osi
    ModelMatrix = glm::rotate(ModelMatrix,glm::radians(rotation.x),glm::vec3(1.f,0.f,0.f));
    ModelMatrix = glm::rotate(ModelMatrix,glm::radians(rotation.y),glm::vec3(0.f,1.f,0.f));
    ModelMatrix = glm::rotate(ModelMatrix,glm::radians(rotation.z),glm::vec3(0.f,0.f,1.f));
    //Skalowanie
    ModelMatrix = glm::scale(ModelMatrix,glm::vec3(scale));

    //CAMERA
    glm::vec3 camPos(0.f,0.f,1.f);
    //Wektor skierowany w góre
    glm::vec3 worldUP = glm::vec3(0.f,1.f,0.f);
    //Wektor skierowany w przód
    glm::vec3 worldFront = glm::vec3(0.f,0.f,-1.f);
    glm::mat4 ViewMatrix(1.f);
    ViewMatrix = glm::lookAt(camPos,camPos+worldFront,worldUP);

    float fov = 90.f;
    float near = 0.1f;
    float far = 1000.f;
    glm::mat4 ProjectMatrix(1.f);

    //Zachowanie rozmiarów przy zmianie rozmiaru okna
    ProjectMatrix = glm::perspective(glm::radians(fov),static_cast<float>(framebufferwidth/framebufferheight),near,far);

    //LIGHTS
    glm::vec3 lightPos0(0.f,0.f,2.f);

    //UNIFORMS
    core.setMat4fv(ModelMatrix,"ModelMatrix");
    core.setMat4fv(ViewMatrix,"ViewMatrix");
    core.setMat4fv(ProjectMatrix,"ProjectMatrix");

    core.setVec3f(lightPos0,"lightPos0");
    core.setVec3f(camPos,"cameraPos");

    //MAIN LOOP
    while(!glfwWindowShouldClose(window))
    {
        //UPDATE INPUT
        //Pozwolenie na interakcji kursorowi
        glfwPollEvents();
        //Poruszanie
        updateInput(window,position,rotation,scale);
        //ESC
        updateInput(window);

        //UPDATE
        //Czyszczenie ekranu
        glClearColor(0.f,0.f,0.f,1.f);
        //Czyszczenie buforwa koloru, szablonu i głębokości
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);


        //DRAW
        core.use();

        //UNIFORMS UPDATE
        //Potrzebne by nakładać kolejne tekstury
        core.set1i(0,"texture0");
        core.set1i(1,"texture1");

        //Operacje
        //rotation.y += 2.f;
        //Translacja
        ModelMatrix = glm::mat4(1.f);
        ModelMatrix = glm::translate(ModelMatrix,position);
        //Obrót wybranej osi
        ModelMatrix = glm::rotate(ModelMatrix,glm::radians(rotation.x),glm::vec3(1.f,0.f,0.f));
        ModelMatrix = glm::rotate(ModelMatrix,glm::radians(rotation.y),glm::vec3(0.f,1.f,0.f));
        ModelMatrix = glm::rotate(ModelMatrix,glm::radians(rotation.z),glm::vec3(0.f,0.f,1.f));
        //Skalowanie
        ModelMatrix = glm::scale(ModelMatrix,glm::vec3(scale));

        //Transformacje
        core.setMat4fv(ModelMatrix,"ModelMatrix");

        //CAMERA
        //Pobranie rozmiarów ramki w każdej klatce ponieważ możemy zmieniać rozmiar okna
        glfwGetFramebufferSize(window,&framebufferwidth,&framebufferheight);

        //Zachowanie rozmiarów przy zmianie rozmiaru okna
        ProjectMatrix = glm::perspective(glm::radians(fov),static_cast<float>(framebufferwidth/framebufferheight),near,far);
        core.setMat4fv(ProjectMatrix,"ProjectMatrix");

        core.use();

        //Aktywowanie tekstury
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,texture0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,texture1);

        //Znajdowanie tablicy wierzechołków
        glBindVertexArray(VAO);

        //Narysuj element
        //glDrawArrays(GL_TRIANGLES, 0, nrOfVertices);
        glDrawElements(GL_TRIANGLES,nrOfIndices,GL_UNSIGNED_INT,0);

        //END
        //Zmiana bufforów i opróźnianie
        glfwSwapBuffers(window);
        glFlush();

        glBindVertexArray(0);
        glUseProgram(0);
        glActiveTexture(0);
        glBindTexture(GL_TEXTURE_2D,0);
    }

    //Niszczenie okna
    glfwDestroyWindow(window);
    //Kończenie programu
    glfwTerminate();

	return 0;
}

