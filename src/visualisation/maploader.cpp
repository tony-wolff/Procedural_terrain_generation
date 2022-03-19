#include "maploader.h"

MapLoader::MapLoader()
{
    m_mapDimensions = 0;
    m_vertexCount = 0;
}

MapLoader::MapLoader(const MapLoader &other)
{

}

MapLoader::~MapLoader()
{

}

/* Read image in PNG format and return a list of values corresponding to the heightmap */
unsigned char* MapLoader::ReadMapPNG(const char* filename)
{
    png::image<png::rgb_pixel> image(filename);
    //image.write("dataset/output.png");
    return NULL;
}