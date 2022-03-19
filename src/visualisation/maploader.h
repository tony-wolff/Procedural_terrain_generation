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
	float minX;
	float minY;
	float minZ;
	float sizeX;
	float sizeY;
	float sizeZ;

	float MaxX() const   { return minX + sizeX; }
	float MaxY() const   { return minY + sizeY; }
	float MaxZ() const   { return minZ + sizeZ; }
    };

public:

	MapLoader();
	MapLoader(const MapLoader&);
	~MapLoader();

	unsigned char* ReadMapPNG(const char* filename);

private:

	MapDimensions* m_mapDimensions;
	int m_vertexCount;
};