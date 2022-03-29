// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

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

class Texture {
private:
    GLuint id_texture;
    int width;
    int height;
    int channels = 0;
    unsigned int type;
    GLint textureUnit;
public:
    Texture(const char*filename,GLenum type,GLint texture_unit);
    ~Texture();

    inline GLuint getID() const;
    //Metoda do aktywacji tekstur
    void bind();
    //Metoda do deaktywacji tekstur
    void unbind();
    GLint getTextureUnit() const;
    void loadFile(const char* fileName);
};

