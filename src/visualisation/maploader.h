/*  This class is used to read the different heightmap formats
    and provide a list of data to the CDLOD algorithm       */

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <png++/png.hpp>

using namespace std;

class MapLoader
{
private:

    struct MapDimensions
    {
	unsigned int minX;
	unsigned int minY;
	unsigned int minZ = 255;
	unsigned int sizeX;
	unsigned int sizeY;
	unsigned int sizeZ;

	unsigned int MaxX() const   { return minX + sizeX; }
	unsigned int MaxY() const   { return minY + sizeY; }
	unsigned int MaxZ() const   { return minZ + sizeZ; }
    };

public:

	MapLoader();
	MapLoader(const MapLoader&);
	~MapLoader();

	uint8_t* ReadMapPNG(const char* filename);

private:

	MapDimensions* m_mapDimensions;
	int m_vertexCount;
};