//
// Created by Ghost on 29.03.2022.
//

#include "Shader.h"


Shader::Shader(char *vertexFile, char *fragmentFile) {
    GLuint vertexShader = 0;
    GLuint fragmentShader = 0;

    vertexShader = loadShader(GL_VERTEX_SHADER,vertexFile);
    fragmentShader = loadShader(GL_FRAGMENT_SHADER,fragmentFile);

    this->linkProgram(vertexShader,fragmentShader);

    //END
    //Nie potrzebujemy inwidualnych shaderow poniewaz zostaja zlaczone w 1 program
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader() {

    glDeleteProgram(this->id);

}

std::string Shader::loadShaderFile(char *str) {
    //Zmienna przechowujaca tekst
    std::string temp = "";
    //Zmienna do przechowywania ścieżki
    std::string src = "";
    std::ifstream file;

    //SHADER
    file.open(str);
    //Czytanie pliku
    if(file.is_open())
    {
        while(std::getline(file,temp))
            src += temp + "\n";
    }else {
        std::cout << "ERROR::SHADER::SHADER_LOADER_FILE " << str << "\n";
    }

    file.close();

    return src;
}

GLuint Shader::loadShader(GLenum type, char *filename) {
    //Tworzenie konsoli dla shaderow
    char log[512];
    GLint success;

    //Tworzenie shadera w pamieci OpenGL i zwrócenie ID tego shadera
    GLuint shader = glCreateShader(type);
    //Ustawienie odpowiedniego formatu
    std::string str_src = this->loadShaderFile(filename);
    const GLchar* src = str_src.c_str();
    //Ustawiamy źródło shadera
    glShaderSource(shader,1,&src,NULL);
    //Kompilacja
    glCompileShader(shader);
    //Sprawdzanie stanu kompilacji
    glGetShaderiv(shader,GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(shader,512,NULL,log);
        std::cout <<"ERROR::SHADER::SHADER FILE LOAD ERROR " << filename << "\n";
        std::cout << log << "\n";
    }

    return shader;
}

void Shader::linkProgram(GLuint vertexShader, GLuint fragmentShader) {
    //Tworzenie konsoli dla shaderow
    char log[512];
    GLint success;
    //Tworzenie programu
    this->id = glCreateProgram();
    //Dołączanie shaderów
    glAttachShader(this->id,vertexShader);
    glAttachShader(this->id,fragmentShader);
    //Laczenie programu
    glLinkProgram(this->id);

    glGetProgramiv(this->id,GL_LINK_STATUS,&success);
    if(!success)
    {
        glGetProgramInfoLog(this->id,512,NULL,log);
        std::cout <<"ERROR::SHADER::PROGRAM_SHADER_LOADER LINK FILE ERROR" << "\n";
        std::cout << log << "\n";
    }
    //Restowanie programu
    glUseProgram(0);
}


void Shader::use() {
    glUseProgram(this->id);
}

void Shader::unuse() {
    glUseProgram(0);
}

void Shader::set1i(GLint value, const GLchar *name) {
    this->use();
    glUniform1i(glGetUniformLocation(this->id,"texture0"),value);

    this->unuse();
}

void Shader::set1f(GLfloat value, const GLchar *name) {
    this->use();
    glUniform1f(glGetUniformLocation(this->id,name),value);

    this->unuse();
}

void Shader::setVec2f(glm::fvec2 value, const GLchar *name) {
    this->use();
    glUniform2fv(glGetUniformLocation(this->id,name),1,glm::value_ptr(value));

    this->unuse();
}

void Shader::setVec3f(glm::fvec3 value, const GLchar *name) {
    this->use();
    glUniform3fv(glGetUniformLocation(this->id,name),1,glm::value_ptr(value));

    this->unuse();
}

void Shader::setVec4f(glm::fvec4 value, const GLchar *name) {
    this->use();
    glUniform4fv(glGetUniformLocation(this->id,name),1,glm::value_ptr(value));

    this->unuse();
}

void Shader::setMat3fv(glm::mat3 value, const GLchar *name, GLboolean transpose) {
    this->use();
    glUniformMatrix3fv(glGetUniformLocation(this->id,name),1,transpose,glm::value_ptr(value));

    this->unuse();
}

void Shader::setMat4fv(glm::mat4 value, const GLchar *name, GLboolean transpose) {
    this->use();
    glUniformMatrix4fv(glGetUniformLocation(this->id,name),1,transpose,glm::value_ptr(value));

    this->unuse();
}
