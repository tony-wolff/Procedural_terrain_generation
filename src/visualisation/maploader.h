/*  This class is used to read the different heightmap formats
    and provide a list of data to the CDLOD algorithm       */

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <Eigen/Dense>
#include <ImageMagick-7/Magick++.h>

using namespace Magick;
using namespace Eigen;

// extern const string heightmap_location;

class MapLoader
{
private:
	int m_mapHeight, m_mapWidth;
	int m_vertexCount;

public:

	MapLoader();
	MapLoader(const MapLoader&);
	~MapLoader();

	/* Read the image in 'any' format and return a Eigen::Matrix containing the values in the heightmap */
	int ReadMapMatrix(const std::string& filename, MatrixXf* matrix);

	/* Read the image in 'any' format and return a Vector containing the values in the heightmap */
	int ReadMapVector(const std::string& filename, std::vector<float> vertices);
};