/*  This class draws the heightmap on the OpenGL Window  */

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <png++/png.hpp>
#include <Eigen/Dense>
#include <SDL2/SDL.h>
#include "maploader.h"

using namespace std;
using namespace Eigen;

extern const string heightmap_location;


class MapRenderer
{
private :
    MapLoader* m_loader;
    MatrixXf m_valueMat;

public :
    MapRenderer();
    MapRenderer(const MapRenderer&);
    ~MapRenderer();

    int RenderMap(const char* filename, MatrixXf valueMat, float size, float h);
};