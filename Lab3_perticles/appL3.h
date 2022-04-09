#ifndef UNTITLED3_APPL3_H
#define UNTITLED3_APPL3_H

#include <algorithm>
#include <gl/GL.h>
#include "dependeces/Common/Initialize.h"
#include "dependeces/Common/controls.hpp"
#include "dependeces/Common/Object.h"

// CPU representation of a particle
struct Particle{
    glm::vec3 pos, speed;
    unsigned char r,g,b,a; // Color
    float size, angle, weight;
    float life; // Remaining life of the particle. if <0 : dead and unused.
    float cameradistance; // *Squared* distance to the camera. if dead : -1.0f

    bool operator<(const Particle& that) const {
        // Sort in reverse order : far particles drawn first.
        return this->cameradistance > that.cameradistance;
    }
};

class appL3{
private:
    GLuint programID;
    GLuint MatrixID;
    GLuint ViewMatrixID;
    GLuint ModelMatrixID;
    GLuint TextureID;
    GLuint lightID;

    GLuint particles_color_buffer;
    // The VBO containing the positions and sizes of the particles
    GLuint particles_position_buffer;
    GLuint billboard_vertex_buffer;

    Object cube;


    Object particle;
    double currentTime;
    double delta;
    double lastTime;
    int LastUsedParticle = 0;
    int newparticles;
    float spread = 1.5f;
    int particleIndex;
    int ParticlesCount;
    // The VBO containing the 4 vertices of the particles.
    // Thanks to instancing, they will be shared by all particles.
    constexpr static const GLfloat g_vertex_buffer_data[] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            -0.5f, 0.5f, 0.0f,
            0.5f, 0.5f, 0.0f,
    };

    static const int MaxParticles = 100;
    Particle ParticlesContainer[MaxParticles];
public:

    appL3(){};
    ~appL3() = default;
    GLubyte* g_particule_color_data = new GLubyte[MaxParticles * 4];
    GLfloat* g_particule_position_size_data = new GLfloat[MaxParticles * 4];

    // Finds a Particle in ParticlesContainer which isn't used yet.
    // (i.e. life < 0);
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

        return 0; // All particles are taken, override the first one
    }

    void SortParticles(){
        std::sort(&ParticlesContainer[0], &ParticlesContainer[MaxParticles]);
    }

    void setUp() {
        window = initGLFW(1024, 768, "Reflections", nullptr, nullptr);
        if (window != nullptr) initGLEW();

        glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
        glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_DISABLED);

        glfwPollEvents();
        glfwSetCursorPos(window, 1024 / 2, 768 / 2);
        glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

        glEnable(GL_DEPTH_TEST);
        // Accept fragment if it's closer to the camera than the former one
        glDepthFunc(GL_LESS);


        programID = LoadShaders("shaders/Particle.vertexshader", "shaders/Particle.fragmentshader");
        //FRAGMNET SHADER
        TextureID = glGetUniformLocation(programID, "myTextureSampler");

        // Get a handle for our "MVP" uniform
        MatrixID = glGetUniformLocation(programID, "MVP");
        ViewMatrixID = glGetUniformLocation(programID, "V");
        ModelMatrixID = glGetUniformLocation(programID, "M");
        lightID = glGetUniformLocation(programID, "LightPosition_worldspace");

        cube.initFromFile("resource/cube.obj");
        cube.loadTexture(programID, "resource/uvmap.png", "myTextureSampler");



        for(int i=0; i<MaxParticles; i++){
            ParticlesContainer[i].life = -1.0f;
            ParticlesContainer[i].cameradistance = -1.0f;
        }

        particle.loadTexture(programID,"resource/uvmap.png", "myTextureSampler");


        glGenBuffers(1, &billboard_vertex_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);


        glGenBuffers(1, &particles_position_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
        // Initialize with empty (NULL) buffer : it will be updated later, each frame.
        glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);


        glGenBuffers(1, &particles_color_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
        // Initialize with empty (NULL) buffer : it will be updated later, each frame.
        glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);

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

        cube.setProjectionMatrix(getProjectionMatrix());
        cube.setViewMatrix(getViewMatrix());
        cube.draw(MatrixID, ViewMatrixID, ModelMatrixID);

        // Generate 10 new particule each millisecond,
        // but limit this to 16 ms (60 fps), or if you have 1 long frame (1sec),
        // newparticles will be huge and the next frame even longer.
        newparticles = (int)(delta*10000.0);
        if (newparticles > (int)(0.016f*10000.0))
            newparticles = (int)(0.016f*10000.0);

        for(int i=0; i<newparticles; i++){
            particleIndex = FindUnusedParticle();
            ParticlesContainer[particleIndex].life = 5.0f; // This particle will live 5 seconds.
            ParticlesContainer[particleIndex].pos = glm::vec3(0,0,-20.0f);


            glm::vec3 maindir = glm::vec3(0.0f, 10.0f, 0.0f);
            // Very bad way to generate a random direction;
            // See for instance http://stackoverflow.com/questions/5408276/python-uniform-spherical-distribution instead,
            // combined with some user-controlled parameters (main direction, spread, etc)
            glm::vec3 randomdir = glm::vec3(
                    (rand()%2000 - 1000.0f)/1000.0f,
                    (rand()%2000 - 1000.0f)/1000.0f,
                    (rand()%2000 - 1000.0f)/1000.0f
            );

            ParticlesContainer[particleIndex].speed = maindir + randomdir*spread;


            // Very bad way to generate a random color
            ParticlesContainer[particleIndex].r = rand() % 256;
            ParticlesContainer[particleIndex].g = rand() % 256;
            ParticlesContainer[particleIndex].b = rand() % 256;
            ParticlesContainer[particleIndex].a = (rand() % 256) / 3;

            ParticlesContainer[particleIndex].size = (rand()%1000)/2000.0f + 0.1f;

        }

        // Simulate all particles
        ParticlesCount = 0;
        for(int i=0; i<MaxParticles; i++){

            Particle& p = ParticlesContainer[i]; // shortcut

            if(p.life > 0.0f){

                // Decrease life
                p.life -= delta;
                if (p.life > 0.0f){

                    // Simulate simple physics : gravity only, no collisions
                    p.speed += glm::vec3(0.0f,-9.81f, 0.0f) * (float)delta * 0.5f;
                    p.pos += p.speed * (float)delta;
                    p.cameradistance = glm::length2( p.pos);
                    //ParticlesContainer[i].pos += glm::vec3(0.0f,10.0f, 0.0f) * (float)delta;

                    // Fill the GPU buffer
                    g_particule_position_size_data[4*ParticlesCount+0] = p.pos.x;
                    g_particule_position_size_data[4*ParticlesCount+1] = p.pos.y;
                    g_particule_position_size_data[4*ParticlesCount+2] = p.pos.z;

                    g_particule_position_size_data[4*ParticlesCount+3] = p.size;

                    g_particule_color_data[4*ParticlesCount+0] = p.r;
                    g_particule_color_data[4*ParticlesCount+1] = p.g;
                    g_particule_color_data[4*ParticlesCount+2] = p.b;
                    g_particule_color_data[4*ParticlesCount+3] = p.a;

                }else{
                    // Particles that just died will be put at the end of the buffer in SortParticles();
                    p.cameradistance = -1.0f;
                }

                ParticlesCount++;

            }
        }

        SortParticles();


        // Update the buffers that OpenGL uses for rendering.
        glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
        glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
        glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(GLfloat) * 4, g_particule_position_size_data);

        glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
        glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
        glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(GLubyte) * 4, g_particule_color_data);


        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Use our shader
        glUseProgram(programID);

        // Bind our texture in Texture Unit 0
        glActiveTexture(GL_TEXTURE0);
       // glBindTexture(GL_TEXTURE_2D, cube);
        // Set our "myTextureSampler" sampler to use Texture Unit 0
        glUniform1i(TextureID, 0);

//        // Same as the billboards tutorial
//        glUniform3f(CameraRight_worldspace_ID, ViewMatrix[0][0], ViewMatrix[1][0], ViewMatrix[2][0]);
//        glUniform3f(CameraUp_worldspace_ID   , ViewMatrix[0][1], ViewMatrix[1][1], ViewMatrix[2][1]);
//
//        glUniformMatrix4fv(ViewProjMatrixID, 1, GL_FALSE, &ViewProjectionMatrix[0][0]);

        // 1rst attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
        glVertexAttribPointer(
                0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                (void*)0            // array buffer offset
        );

        // 2nd attribute buffer : positions of particles' centers
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
        glVertexAttribPointer(
                1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
                4,                                // size : x + y + z + size => 4
                GL_FLOAT,                         // type
                GL_FALSE,                         // normalized?
                0,                                // stride
                (void*)0                          // array buffer offset
        );

        // 3rd attribute buffer : particles' colors
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
        glVertexAttribPointer(
                2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
                4,                                // size : r + g + b + a => 4
                GL_UNSIGNED_BYTE,                 // type
                GL_TRUE,                          // normalized?    *** YES, this means that the unsigned char[4] will be accessible with a vec4 (floats) in the shader ***
                0,                                // stride
                (void*)0                          // array buffer offset
        );

        // These functions are specific to glDrawArrays*Instanced*.
        // The first parameter is the attribute buffer we're talking about.
        // The second parameter is the "rate at which generic vertex attributes advance when rendering multiple instances"

        glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
        glVertexAttribDivisor(1, 1); // positions : one per quad (its center)                 -> 1
        glVertexAttribDivisor(2, 1); // color : one per quad                                  -> 1

        // Draw the particules !
        // This draws many times a small triangle_strip (which looks like a quad).
        // This is equivalent to :
        // for(i in ParticlesCount) : glDrawArrays(GL_TRIANGLE_STRIP, 0, 4),
        // but faster.
        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, ParticlesCount);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    void clean() {
        delete[] g_particule_position_size_data;

        // Cleanup VBO and shader
        glDeleteBuffers(1, &particles_color_buffer);
        glDeleteBuffers(1, &particles_position_buffer);
        glDeleteBuffers(1, &billboard_vertex_buffer);
        glDeleteProgram(programID);
        //glDeleteTextures(1, &Texture);
        glfwTerminate();
    }
};

#endif
