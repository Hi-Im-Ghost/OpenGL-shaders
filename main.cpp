#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>

#include "dependeces/Common/Initialize.h"
#include <glm/glm.hpp>

using namespace glm;
GLFWwindow *window;
#include "dependeces/Common/objloader.hpp"
#include "dependeces/Common/shader.hpp"

//#include "Lab1/Lab02Zad1.h"
#include "Lab2_reflections/appL2.h"
#include "Lab3_perticles/appL3_rain.h"
#include "Lab3_perticles/appL3_smoke.h"
#include "Lab3_perticles/appL3_fog.h"
#include "Lab4_textures/appL4.h"
#include "Lab5_Shaders/appL5.h"
#include "Lab6_Light/appL6.h"
#include "Lab8_Cubemap/appL8.h"
#include "Lab9_BumpMap/appL9.h"

int main() {
    appL8 app;
    app.setUp();
    do {
        app.update();

    } // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0);

    app.clean();
    return 0;
}
