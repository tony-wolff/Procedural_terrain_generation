#include <GLFW/glfw3.h>
#include "maprenderer.h"

MapRenderer::MapRenderer()
{
    m_loader = new MapLoader();
}

MapRenderer::MapRenderer(const MapRenderer&)
{

}

MapRenderer::~MapRenderer()
{
    delete m_loader;
}

int MapRenderer::RenderMap(const char* filename, MatrixXf mat, float size, float h)
{
    if (m_loader->ReadMap(filename, mat) == -1)
    {
        return -1;
    }

    for (int i = 0; i < mat.rows() - 1; i++)
    {
        for (int j = 0; j < mat.cols() - 1; j++)
        {
            glBegin(GL_TRIANGLE_STRIP);
                glColor3f(mat(i, j), mat(i, j), mat(i, j));
                glVertex3f(i*size, h*mat(i, j), j*size);
                glVertex3f((i+1)*size, h*mat(i+1, j), j*size);
                glVertex3f(i*size, h*mat(i, j+1), (j+1)*size);
                glVertex3f((i+1)*size, h*mat(i+1, j+1), (j+1)*size);

            glEnd();
        }
    }

    return 0;
}