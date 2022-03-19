#include "maploader.h"

MapLoader::MapLoader()
{
    m_mapDimensions = new MapDimensions();
    m_vertexCount = 0;
}

MapLoader::MapLoader(const MapLoader &other)
{

}

MapLoader::~MapLoader()
{
    delete m_mapDimensions;
}

/* Read image in PNG format and return a list of values corresponding to the heightmap */
uint8_t* MapLoader::ReadMapPNG(const char* filename)
{
    // Create a 8bit gray alpha representation of filename
    png::image<png::rgb_pixel > image(filename);
    //png::image<png::basic_ga_pixel<unsigned char> > image(filename, png::require_color_space< png::basic_ga_pixel<unsigned char> >());

    // Set the dimensions
    m_mapDimensions->sizeX = image.get_width();
    m_mapDimensions->sizeY = image.get_height();

    const int size = m_mapDimensions->sizeX * m_mapDimensions->sizeY;

    uint8_t* pixels = new uint8_t[size];

    // Iterate over the image 
    for (png::uint_32 y = 0; y < m_mapDimensions->sizeY; y++) {
        for (png::uint_32 x = 0; x < m_mapDimensions->sizeX; x++) {
            uint8_t currentPixel = image[y][x].red;

            if (m_mapDimensions->minZ > currentPixel)
                m_mapDimensions->minZ = currentPixel;

            // Flatten the array
            pixels[y * m_mapDimensions->sizeY + x] = currentPixel;
            
        }
    }

    return pixels;
}