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
using namespace glm;

#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/controls.hpp>
#include <iostream>
#include <fstream>

//Zmiana rozmiaru okna
void resize(GLFWwindow *,int W, int H)
{
    glViewport(0,0,W,H);
}

bool shaderLoader(GLuint &program)
{
    bool load = true;
    //Tworzenie konsoli dla shaderow
    char log[512];
    GLint success;

    //Zmienna przechowujaca tekst
    std::string temp = "";
    //Zmienna do przechowywania ścieżki
    std::string src = "";
    std::ifstream file;

    //VERTEX
    file.open("TransformVertexShader.vertexshader");
    //Czytanie pliku
    if(file.is_open())
    {
        while(std::getline(file,temp))
            src += temp + "\n";
    }else {
        std::cout << "ERROR::LAB2::VERTEX_SHADER_LOADER" << "\n";
        load = false;
    }

    file.close();

    //Tworzenie vertex shadera w pamieci OpenGL i zwrócenie ID tego shadera
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //Ustawienie odpowiedniego formatu
    const GLchar* vert = src.c_str();
    //Ustawiamy źródło shadera
    glShaderSource(vertexShader,1,&vert,NULL);
    //Kompilacja
    glCompileShader(vertexShader);
    //Sprawdzanie stanu kompilacji
    glGetShaderiv(vertexShader,GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader,512,NULL,log);
        std::cout <<"ERROR::LAB2::VERTEX_SHADER FILE ERROR" << "\n";
        std::cout << log << "\n";
        load = false;
    }

    //Czyszczenie
    temp="";
    src="";

    //FRAGMENT
    file.open("TextureFragmentShader.fragmentshader");
    //Czytanie pliku
    if(file.is_open())
    {
        while(std::getline(file,temp))
            src += temp + "\n";
    }else {
        std::cout << "ERROR::LAB2::FRAGMENT_SHADER_LOADER" << "\n";
        load = false;
    }

    file.close();

    //Tworzenie vertex shadera w pamieci OpenGL i zwrócenie ID tego shadera
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    //Ustawienie odpowiedniego formatu
    const GLchar* fragment = src.c_str();
    //Ustawiamy źródło shadera
    glShaderSource(fragmentShader,1,&fragment,NULL);
    //Kompilacja
    glCompileShader(fragmentShader);
    //Sprawdzanie stanu kompilacji
    glGetShaderiv(fragmentShader,GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragmentShader,512,NULL,log);
        std::cout <<"ERROR::LAB2::FRAGMENT_SHADER_LOADER FILE ERROR" << "\n";
        std::cout << log << "\n";
        load = false;
    }

    //PROGRAM
    program = glCreateProgram();
    //dołączanie do programu shaderów
    glAttachShader(program,vertexShader);
    glAttachShader(program,fragmentShader);
    glLinkProgram(program);
    //Sprawdzanie kompilacji
    glGetProgramiv(program,GL_LINK_STATUS,&success);
    if(!success)
    {
        glGetProgramInfoLog(program,512,NULL,log);
        std::cout <<"ERROR::LAB2::PROGRAM_SHADER_LOADER FILE ERROR" << "\n";
        std::cout << log << "\n";
        load = false;
    }

    //END
    //Restowanie programu
    glUseProgram(0);
    //Nie potrzebujemy inwidualnych shaderow poniewaz zostaja zlaczone w 1 program
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return load;
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

    //Zmiana rozmiaru okna bufora
    glfwSetFramebufferSizeCallback(window,resize);
//Używane gdy nie zmieniamy rozmiaru
//    //Wybranie okna na którym chcemy działać
//    glfwGetFramebufferSize(window,&framebufferwidth,&framebufferheight);
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

    //INIT SHADER
    GLuint core;
    if(!shaderLoader(core)){
        glfwTerminate();
    }

    //MAIN LOOP
    while(!glfwWindowShouldClose(window))
    {
        //UPDATE INPUT
        //Pozwolenie na interakcji kursorowi
        glfwPollEvents();

        //UPDATE
        //Czyszczenie ekranu
        glClearColor(0.f,0.f,0.f,1.f);
        //Czyszczenie buforwa koloru, szablonu i głębokości
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        //DRAW

        //END
        //Zmiana bufforów i opróźnianie
        glfwSwapBuffers(window);
        glFlush();
    }

    //Niszczenie okna
    glfwDestroyWindow(window);
    //Kończenie programu
    glfwTerminate();

    glDeleteProgram(core);

	return 0;
}

