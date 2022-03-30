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

#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/controls.hpp>
#include <common/stb_image.h>
#include "Shader.h"
#include "Texture.h"
#include "Mesh.h"
#include "Primitives/Primitives.h"
#include "Primitives/Vertex.h"

//Funkcja do poruszania
void updateInput(GLFWwindow* window,Mesh &mesh)
{
    //Jeśli klikne klawisz W to przesune.....
    if(glfwGetKey(window,GLFW_KEY_W) == GLFW_PRESS){
        mesh.move(glm::vec3(0.f, 0.f, -0.01f));
    }
    if(glfwGetKey(window,GLFW_KEY_S) == GLFW_PRESS){
        mesh.move(glm::vec3(0.f, 0.f, 0.01f));
    }
    if(glfwGetKey(window,GLFW_KEY_A) == GLFW_PRESS){
        mesh.move(glm::vec3(-0.01f, 0.f, 0.f));
    }
    if(glfwGetKey(window,GLFW_KEY_D) == GLFW_PRESS){
        mesh.move(glm::vec3(0.01f, 0.f, 0.f));
    }
    if(glfwGetKey(window,GLFW_KEY_Q) == GLFW_PRESS){
        mesh.rotate(glm::vec3(0.f, -1.f, 0.f));
    }
    if(glfwGetKey(window,GLFW_KEY_E) == GLFW_PRESS){
        mesh.rotate(glm::vec3(0.f, 1.f, 0.f));
    }
    if(glfwGetKey(window,GLFW_KEY_Z) == GLFW_PRESS){
        mesh.scaleUp(glm::vec3(1.f));
    }
    if(glfwGetKey(window,GLFW_KEY_X) == GLFW_PRESS){
        mesh.scaleUp(glm::vec3(-1.f));
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
    //Stencil BUFFOR
    glEnable(GL_STENCIL_TEST);
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

    //MESH

    Primitives test = Pyramid();
    std::cout<<test.getNrOfVertices()<<std::endl;
    std::cout<<test.getVertices()<<std::endl;
    std::cout<<test.getIndices()<<std::endl;
    std::cout<<test.getNrOfIndices()<<std::endl;

//    Mesh quad(&test,
//              glm::vec3(0.f),
//              glm::vec3(0.f),
//              glm::vec3(1.f)
//    );

    Mesh quad(test.getVertices(),
              test.getNrOfVertices(),
              test.getIndices(),
              test.getNrOfIndices(),
              glm::vec3(0.f),
              glm::vec3(0.f),
              glm::vec3(1.f)
    );

    //TEXTURE
    Texture texture0("../Images/wood.png",GL_TEXTURE_2D,0);
    Texture texture1("../Images/awesomeface.png",GL_TEXTURE_2D,1);


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
        updateInput(window,quad);
        //ESC
        updateInput(window);

        //UPDATE
        //Czyszczenie ekranu
        glClearColor(0.f,0.f,0.f,1.f);
        //Czyszczenie buforwa koloru, szablonu i głębokości
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        //UNIFORMS UPDATE
        //Potrzebne by nakładać kolejne tekstury
        core.set1i(texture0.getTextureUnit(),"texture0");
        core.set1i(texture1.getTextureUnit(),"texture1");

        //CAMERA
        //Pobranie rozmiarów ramki w każdej klatce ponieważ możemy zmieniać rozmiar okna
        glfwGetFramebufferSize(window,&framebufferwidth,&framebufferheight);

        //Zachowanie rozmiarów przy zmianie rozmiaru okna
        ProjectMatrix = glm::perspective(glm::radians(fov),static_cast<float>(framebufferwidth/framebufferheight),near,far);
        core.setMat4fv(ProjectMatrix,"ProjectMatrix");

        core.use();

        //Aktywowanie tekstury
        texture0.bind();
        texture1.bind();


        //Narysuj element
        quad.render(&core);

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

