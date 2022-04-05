#include "maploader.h"

MapLoader::MapLoader()
{
    m_mapHeight = 0;
    m_mapWidth = 0;
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

int MapLoader::ReadMap(const std::string& filename, MatrixXf* matrix)
{
    InitializeMagick(NULL);
    Image heightmap;

    try
    { 
        // Read a file into image object 
        heightmap.read(filename);
    } 
    catch(Exception &error_) 
    { 
        cerr << "Caught Magick++ exception opening file " << filename << ": "<< error_.what() << endl;
        return -1;
    }

    // Getting the width and height and resizing the matrix
    m_mapHeight = heightmap.rows();
    m_mapWidth = heightmap.columns();
    matrix->resize(m_mapHeight, m_mapWidth);

    // cout << "Map Height : " << m_mapHeight << endl;
    // cout << "Map Width : " << m_mapWidth << endl;

    // Acces image pixel by pixel
    try
    {
        heightmap.modifyImage();

        for (ssize_t i = 0; i < m_mapHeight; i++)
        {
            for (ssize_t j = 0; j < m_mapWidth; j++)
            {
                ColorRGB pixel = heightmap.pixelColor(j, i);
                (*matrix)(i, j) = pixel.red();
            }
        }
    }
    catch(Exception &error_) 
    { 
        cerr << "Caught Magick++ exception processing image pixels: " << error_.what() << endl;
        return -1;
    }

    // cout << mat << endl;
    // cout << mat.size() << endl;

    return 0;
}