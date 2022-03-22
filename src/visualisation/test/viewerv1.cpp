/*#include <iostream>
#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "camera.h"
//#include "functions.h"
//#include "maploader.h"
#include "viewer.h"
#include <vector>
Viewer *v;

std::vector<std::vector<float>> heights;

Viewer::Viewer()
{
}

Viewer::~Viewer()
{
}

void Viewer::renderHeightmap(float size, float h)
{
    for (int i = 0; i < heights.size() - 1; i++)
        for (int j = 0; j < heights[0].size() - 1; j++)
        {
            glBegin(GL_TRIANGLE_STRIP);
            glColor3f(heights[i][j], heights[i][j], heights[i][j]);
            glVertex3f(i * size, heights[i][j] * h, j * size);
            glVertex3f((i + 1) * size, heights[i + 1][j] * h, j * size);
            glVertex3f(i * size, heights[i][j + 1] * h, (j + 1) * size);
            glVertex3f((i + 1) * size, heights[i + 1][j + 1] * h, (j + 1) * size);
            glEnd();
        }
}

void Viewer::init(float angle)
{
    glClearColor(0, 0, 0, 1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(angle, 640.0 / 480.0, 1, 1000);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
    map.ReadMapPNG("heightmap.png");
}

void Viewer::display()
{
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    cam.Control();

    cam.UpdateCamera();
    renderHeightmap(0.01, 0.4);
}

int main()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_SetVideoMode(640, 480, 32, SDL_OPENGL);
    Uint32 start;
    SDL_Event event;
    bool running = true;
    float angle = 50;
    v = new Viewer();
    v->init(angle);
    bool b = false;
    while (running)
    {
        start = SDL_GetTicks();
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    running = false;
                    break;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                cam.mouseIn(true);
                break;
            }
        }
        display();
        SDL_GL_SwapBuffers();
        if (1000.0 / 30 > SDL_GetTicks() - start)
            SDL_Delay(1000.0 / 30 - (SDL_GetTicks() - start));
    }
    SDL_Quit();
}
*/