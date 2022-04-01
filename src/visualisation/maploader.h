/*  This class is used to read the different heightmap formats
    and provide a list of data to the CDLOD algorithm       */

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <Eigen/Dense>
#include <ImageMagick-7/Magick++.h>

using namespace std;
using namespace Eigen;
using namespace Magick;

extern const string heightmap_location;

class MapLoader
{
private:
	int m_mapHeight, m_mapWidth;

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

	/* Read the image in 'any' format, return -1 if the files can't be loaded, 0 otherwise */
	int ReadMap(const std::string& filename, MatrixXf* mat);

private:

	MapDimensions* m_mapDimensions;
	int m_vertexCount;
};