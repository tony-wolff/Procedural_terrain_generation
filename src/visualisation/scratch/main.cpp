#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include "maploader.h"

//MapLoader* ml;

int WIDTH = 600;
int HEIGHT = 600;

int g_pixel_ratio = 1;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

/*static void char_callback(GLFWwindow* window, unsigned int key)
{
    v->charPressed(key);
}

static void scroll_callback(GLFWwindow* window, double x, double y)
{
    v->mouseScroll(x,y);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if ((key == GLFW_KEY_ESCAPE) && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    v->keyPressed(key,action,mods);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    v->mousePressed(window, button, action);
}

void cursorPos_callback(GLFWwindow* window, double x, double y)
{
    v->mouseMoved(x*g_pixel_ratio,y*g_pixel_ratio);
}

void reshape_callback(GLFWwindow* window, int width, int height)
{
    v->reshape(width,height);
    v->updateAndDrawScene();
    glfwSwapBuffers(window);
}*/

GLFWwindow* initGLFW()
{
    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, (GLint)GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Procedural Planet Generation", NULL, NULL);
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    //glbinding::Binding::initialize(glfwGetProcAddress);
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    // Handle retina display:
    int w, h;
    glfwGetFramebufferSize(window, &w, &h);
    if(w==2*WIDTH)
      g_pixel_ratio = 2;

    // callbacks
    glfwSetKeyCallback(window, key_callback);
    /*glfwSetCharCallback(window, char_callback);
    glfwSetFramebufferSizeCallback(window, reshape_callback);
    glfwSetCursorPosCallback(window, cursorPos_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);*/



    return window;
}

static void error_callback(int /*error*/, const char* description)
{
    fputs(description, stderr);
}


// int main (int argc, char **argv)
// {
//     // Try heightmap rendering
//     MapLoader *loader = new MapLoader();
//     MatrixXf valueMat;

//     loader->ReadMap("../testmap.bmp", valueMat);

//     glfwSetErrorCallback(error_callback);

//     GLFWwindow* window = initGLFW();
//     int w, h;
//     glfwGetFramebufferSize(window, &w, &h);
//     //ml = new MapLoader();
//     //v->init(w,h);

//     double t0 = glfwGetTime();
//     double t1 = t0;
//     while (!glfwWindowShouldClose(window))
//     {
//         // render the scene
//         t1 = glfwGetTime();
//         if(t1-t0>0.03) {
//             //v->updateAndDrawScene();
//             glfwSwapBuffers(window);
//             t0 = t1;
//         }
//         glfwPollEvents();
//     }

//     //delete v;
//     //delete ml;

//     glfwDestroyWindow(window);
//     glfwTerminate();
//     exit(EXIT_SUCCESS);
// }

int main(void)
{
    // Heightmap parameters
    MapLoader *loader = new MapLoader();
    MatrixXf valueMat;
    loader->ReadMap("../input_data/testmap.bmp", valueMat);
    float size = 10.0;
    float h = 10.0;

    GLFWwindow* window;
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);
    window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    while (!glfwWindowShouldClose(window))
    {
        float ratio;
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float) height;
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        for (int i = 0; i < valueMat.rows() - 1; i++)
        {
            for (int j = 0; j < valueMat.cols() - 1; j++)
            {
                glBegin(GL_TRIANGLE_STRIP);
                glColor3f(valueMat(i, j), valueMat(i, j), valueMat(i, j));
                glVertex3f(i*size, h*valueMat(i, j), j*size);
                glVertex3f((i+1)*size, h*valueMat(i+1, j), j*size);
                glVertex3f(i*size, h*valueMat(i, j+1), (j+1)*size);
                glVertex3f((i+1)*size, h*valueMat(i+1, j+1), (j+1)*size);
                glEnd();
            }
        }
        //glEnd();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}