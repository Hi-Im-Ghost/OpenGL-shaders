//
// Created by Ghost on 29.03.2022.
//

#include "Texture.h"

Texture::Texture(const char*filename,GLenum type,GLint texture_unit) {

    this->type = type;
    this->textureUnit = texture_unit;
    //Generowanie tekstury
    glGenTextures(1,&this->id_texture);
    //Wybranie tekstury
    glBindTexture(type,this->id_texture);

    //Powtarzanie tekstury
    glTexParameteri(type,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(type,GL_TEXTURE_WRAP_T,GL_REPEAT);
    //Antyaliasing i inne takie
    glTexParameteri(type,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(type,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

    unsigned char* image = stbi_load(filename,&this->width,&this->height,&this->channels,STBI_rgb_alpha);

    if(image)
    {
        glTexImage2D(type,0,GL_RGBA,this->width,this->height,0,GL_RGBA,GL_UNSIGNED_BYTE,image);
        //Generowanie kilku obrazów o różnych rozmiarach w zaleznosci od odleglosci
        glGenerateMipmap(type);
    }else{
        std::cout << "ERROR::TEXUTRE LOADING " << filename << "\n";
    }

    glActiveTexture(0);
    //Wyłączenie tekstury
    glBindTexture(type,0);
    stbi_image_free(image);
}

Texture::~Texture() {
    glDeleteTextures(1,&this->id_texture);
}

GLuint Texture::getID() const {
    return this->id_texture;
}

void Texture::bind() {
    //Dodajemy gdy chcemy wiecej tekstur aktywować
    glActiveTexture(GL_TEXTURE0 + this->textureUnit);
    glBindTexture(this->type,this->id_texture);
}

void Texture::unbind() {
    glActiveTexture(0);
    glBindTexture(this->type,0);
}

GLint Texture::getTextureUnit() const {
    return this->textureUnit;
}

void Texture::loadFile(const char *filename) {

    if(this->id_texture)
    {
        glDeleteTextures(1,&this->id_texture);
    }
    //Generowanie tekstury
    glGenTextures(1,&this->id_texture);
    //Wybranie tekstury
    glBindTexture(this->type,this->id_texture);

    //Powtarzanie tekstury
    glTexParameteri(this->type,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(this->type,GL_TEXTURE_WRAP_T,GL_REPEAT);
    //Antyaliasing i inne takie
    glTexParameteri(this->type,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(this->type,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

    unsigned char* image = stbi_load(filename,&this->width,&this->height,&this->channels,STBI_rgb_alpha);

    if(image)
    {
        glTexImage2D(this->type,0,GL_RGBA,this->width,this->height,0,GL_RGBA,GL_UNSIGNED_BYTE,image);
        //Generowanie kilku obrazów o różnych rozmiarach w zaleznosci od odleglosci
        glGenerateMipmap(this->type);
    }else{
        std::cout << "ERROR::LOAD TEXTURE  " << filename << "\n";
    }

    glActiveTexture(0);
    //Wyłączenie tekstury
    glBindTexture(this->type,0);
    stbi_image_free(image);
}
