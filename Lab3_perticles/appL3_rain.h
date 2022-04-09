#ifndef UNTITLED3_APPL3_RAIN_H
#define UNTITLED3_APPL3_RAIN_H

#include <algorithm>
#include <gl/GL.h>
#include "dependeces/Common/Initialize.h"
#include "dependeces/Common/controls.hpp"
#include "dependeces/Common/Object.h"

struct Particle{
    glm::vec3 pos, speed; //pozycja predkosc
    unsigned int r,g,b,a; // Color
    float size, angle, weight; //rozmiar itd
    float life; //czas zycia czasteczki
    float cameradistance; // Dystans od kamery jesli jest zbyt daleko to czasteczka umiera

    bool operator<(const Particle& that) const {
        // Sortowanie w odwrotnej kolejnosci by byly rysowane najpierw czasteczki ktore sa daleko od kamery
        return this->cameradistance > that.cameradistance;
    }
};

// wierzcholki VBO
static const GLfloat g_vertex_buffer_data[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f,
        0.5f, 0.5f, 0.0f,
};
class appL3_rain{
private:
    GLuint programID;
    GLuint TextureID;
    GLuint lightID;
    GLuint CameraRight_worldspace_ID;
    GLuint CameraUp_worldspace_ID;
    GLuint ViewProjMatrixID;

    GLuint VertexArrayID;

    // Zawiera pozycje i rozmiar oraz kolor czasteczek
    GLuint position_buffer;
    GLuint vertex_buffer;
    GLuint color_buffer;

    GLuint particle;
    double currentTime;
    double delta;
    double lastTime;
    int LastUsedParticle = 0;
    int newparticles;
    float spread = 1.5f;
    int particleIndex;
    int ParticlesCount;
    GLubyte* g_particule_color_data;
    GLfloat* g_particule_position_size_data;

    //Maksymalna ilosc czasteczek
    static const int MaxParticles = 10000;
    Particle ParticlesContainer[MaxParticles];

public:

    appL3_rain(){};
    ~appL3_rain() = default;


    // Funkcaj do znajdowania czasteczek które zgineły
    int FindUnusedParticle(){

        for(int i=LastUsedParticle; i<MaxParticles; i++){
            if (ParticlesContainer[i].life < 0){
                LastUsedParticle = i;
                return i;
            }
        }

        for(int i=0; i<LastUsedParticle; i++){
            if (ParticlesContainer[i].life < 0){
                LastUsedParticle = i;
                return i;
            }
        }

        return 0; // Napisz pierwsza czasteczke ktore znajdziesz
    }

    //Funkcja do sortowania czasteczek
    void SortParticles(){
        std::sort(&ParticlesContainer[0], &ParticlesContainer[MaxParticles]);
    }

    void setUp() {
        window = initGLFW(1024, 768, "Rain", nullptr, nullptr);
        if (window != nullptr) initGLEW();

        glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
        glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_DISABLED);

        glfwPollEvents();
        glfwSetCursorPos(window, 1024 / 2, 768 / 2);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

        glEnable(GL_DEPTH_TEST);

        glDepthFunc(GL_LESS);

        glGenVertexArrays(1, &VertexArrayID);
        glBindVertexArray(VertexArrayID);

        programID = LoadShaders("shaders/Particle.vertexshader", "shaders/Particle.fragmentshader");


        // Vertex shader
        CameraRight_worldspace_ID  = glGetUniformLocation(programID, "CameraRight_worldspace");
        CameraUp_worldspace_ID  = glGetUniformLocation(programID, "CameraUp_worldspace");
        ViewProjMatrixID = glGetUniformLocation(programID, "VP");

        lightID = glGetUniformLocation(programID, "LightPosition_worldspace");

        GLint out[2];
        LoadTexture(programID,"resource/rain.png","myTextureSampler",out);
        particle=out[1];
        TextureID = out[0];


        for(int i=0; i<MaxParticles; i++){
            ParticlesContainer[i].life = -1.0f;
            ParticlesContainer[i].cameradistance = -1.0f;
        }


        glGenBuffers(1, &vertex_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);


        glGenBuffers(1, &position_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
        glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);


        glGenBuffers(1, &color_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
        glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);

        g_particule_color_data = new GLubyte[MaxParticles * 4];
        g_particule_position_size_data = new GLfloat[MaxParticles * 4];

        lastTime = glfwGetTime();
    }

    void update() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        currentTime = glfwGetTime();
        delta = currentTime - lastTime;
        lastTime = currentTime;

        glUseProgram(programID);
        glUniform3f(lightID, 2.0f, 2.0f, 2.0f);

        computeMatricesFromInputs();
        glm::mat4 ProjectionMatrix = getProjectionMatrix();
        glm::mat4 ViewMatrix = getViewMatrix();

        glm::vec3 CameraPosition(glm::inverse(ViewMatrix)[3]);

        glm::mat4 ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;

        // Generuje 10 nowych cząstek co milisekundę
        // ale ogranicza to do 16 ms (60 fps) jeśli jest dłuższa przerwa (1 sek)
        // to wygeneruje duzo czasteczek i kolejna przerwa bedzie jeszcze dluzsza
        // w skróce ustawienie generowania czasteczek czy ma powoli byc ich coraz wiecej az do limitu czy co jakis czas ma np. wszystkie mozliwe wypuscic
        // okreslenie ile powinnismy wygenrowac w kazdej klatce
        newparticles = (int)(delta*100.0);
        if (newparticles > (int)(0.016f*100.0))
            newparticles = (int)(0.016f*100.0);

        for(int i=0; i<newparticles; i++){
            particleIndex = FindUnusedParticle();
            ParticlesContainer[particleIndex].life = 5.0f; // Czas zycia czasteczki
            ParticlesContainer[particleIndex].pos = glm::vec3(float(rand()%30),0,float(rand()%30)); //Pozycja emitera

            //Okreslenei kierunku spadania czasteczek (tworzenie w ktorym miejscu)
            glm::vec3 maindir = glm::vec3(0.0f, -10.0f, 0.0f);
            //Ustawianie pozycji losowej pojawiania sie czasteczek
            glm::vec3 randomdir = glm::vec3(
                    (rand()%2000 - 1000.0f)/1000.0f,
                    (rand()%2000 - 1000.0f)/1000.0f,
                    (rand()%2000 - 1000.0f)/1000.0f
            );

            ParticlesContainer[particleIndex].speed = maindir *spread ;


            //Ustawianie kolorow
            ParticlesContainer[particleIndex].r = 0;
            ParticlesContainer[particleIndex].g = 0;
            ParticlesContainer[particleIndex].b = 254;
            ParticlesContainer[particleIndex].a = 254;
            //Wielkosc czasteczki
            ParticlesContainer[particleIndex].size = (rand()%1000)/2000.0f + 0.1f;

        }

        //Symulacja particles
        ParticlesCount = 0;
        for(int i=0; i<MaxParticles; i++){

            Particle& p = ParticlesContainer[i];

            if(p.life > 0.0f){

                //Odejmowanie czasu życia
                p.life -= delta;
                if (p.life > 0.0f){

                    // Symulowanie grawitacji
                    p.speed += glm::vec3(0.0f,-9.81f, 0.0f) * (float)delta * 0.5f;
                    p.pos += p.speed * (float)delta;
                    p.cameradistance = glm::length2( p.pos - CameraPosition);
                    //ParticlesContainer[i].pos += glm::vec3(0.0f,10.0f, 0.0f) * (float)delta;

                    // Wypełnianie buffora
                    g_particule_position_size_data[4*ParticlesCount+0] = p.pos.x;
                    g_particule_position_size_data[4*ParticlesCount+1] = p.pos.y;
                    g_particule_position_size_data[4*ParticlesCount+2] = p.pos.z;

                    g_particule_position_size_data[4*ParticlesCount+3] = p.size;

                    g_particule_color_data[4*ParticlesCount+0] = p.r;
                    g_particule_color_data[4*ParticlesCount+1] = p.g;
                    g_particule_color_data[4*ParticlesCount+2] = p.b;
                    g_particule_color_data[4*ParticlesCount+3] = p.a;

                }else{
                    // Czasteczki które umierają sa umieszczone na końcu bufora podczas sortowania
                    p.cameradistance = -1.0f;
                }

                ParticlesCount++;

            }
        }

        SortParticles();
        //std::cout<<"Liczba particli"<<ParticlesCount<<std::endl;

        // Aktualizacja bufforów do rysowania
        glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
        glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); // Buffer orphaning, metoda na poprawe wydajnosci
        glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(GLfloat) * 4, g_particule_position_size_data);

        glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
        glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW); //  Buffer orphaning, metoda na poprawe wydajnosci
        glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(GLubyte) * 4, g_particule_color_data);


        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        //Uruchomienie shaderów
        glUseProgram(programID);

        // Powiazywanie tekstury z unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, particle);
        // Ustawienie naszej wgranej tekstury by uzywal tekstury 0
        glUniform1i(TextureID, 0);

        glUniform3f(CameraRight_worldspace_ID, ViewMatrix[0][0], ViewMatrix[1][0], ViewMatrix[2][0]);
        glUniform3f(CameraUp_worldspace_ID   , ViewMatrix[0][1], ViewMatrix[1][1], ViewMatrix[2][1]);


        glUniformMatrix4fv(ViewProjMatrixID, 1, GL_FALSE, &ViewProjectionMatrix[0][0]);

        //vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
        glVertexAttribPointer(
                0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                (void*)0            // array buffer offset
        );

        //positions of particles' centers
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
        glVertexAttribPointer(
                1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
                4,                                // size : x + y + z + size => 4
                GL_FLOAT,                         // type
                GL_FALSE,                         // normalized?
                0,                                // stride
                (void*)0                          // array buffer offset
        );

        //particles' colors
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
        glVertexAttribPointer(
                2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
                4,                                // size : r + g + b + a => 4
                GL_UNSIGNED_BYTE,                 // type
                GL_TRUE,                          // normalized?    *** YES, this means that the unsigned char[4] will be accessible with a vec4 (floats) in the shader ***
                0,                                // stride
                (void*)0                          // array buffer offset
        );

        // glDrawArrays*Instanced*.
        // Pierwszym parametr, bufor atrybutów, o którym mówimy
        // Drugi parametr szybkość, z jaką ogólne atrybuty wierzchołków przesuwają się podczas renderowania wielu instancj

        glVertexAttribDivisor(0, 0); // particles vertices
        glVertexAttribDivisor(1, 1); // positions
        glVertexAttribDivisor(2, 1); // color

        // Rysowanie czasteczek
        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, ParticlesCount);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    void clean() {
        delete[] g_particule_position_size_data;

        glDeleteBuffers(1, &color_buffer);
        glDeleteBuffers(1, &position_buffer);
        glDeleteBuffers(1, &vertex_buffer);
        glDeleteProgram(programID);
        glDeleteTextures(1, &particle);
        glDeleteVertexArrays(1, &VertexArrayID);

        glfwTerminate();
    }
};

#endif
