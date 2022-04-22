#ifndef _HM_gen_h
#define _HM_gen_h

#include <noise/noise.h>
#include <noiseutils.h>
#include <string>

class HeightMapGen;

using namespace noise;

struct bounding_square 
{
    double lowerX;
    double upperX;
    double lowerZ;
    double upperZ;
};
class HeightMapGen {

public:
    void SetSize(int width, int height);
    void Save(std::string output_file);
    void GenerateRandomHeightMap();
    void GeneratePlanarHeightMap(int width, int height);
    void GenerateSphericalHeightMap(int width, int height);
    void SetBoundingSquare(double lowerX, double upperX, double lowerZ, double upperZ);
    void SetOctave(int octave);
    void SetFrequency(double frequency);
    void SetPersistence(double persistence);
    void GenerateRandomHeightMap(int width, int height, double minX, double minZ, double maxX, double maxZ);
    int randomBetween(int, int);
    double randomBetween(double, double);
    void RandomBoundingSquare(double minX, double minZ, double maxX, double maxZ);
    utils::NoiseMapBuilderPlane GetBuilderPlane(void);
    
private:
    module::Perlin pModule;
    utils::NoiseMap heightMap;
    utils::NoiseMapBuilderPlane builder_plane;
    utils::NoiseMapBuilderSphere builder_sphere;
    utils::RendererImage renderer;
    utils::Image image;
    utils::WriterBMP writer;
    void RenderProcessing();
    bounding_square bs;
    double MIN_X=-10;
    double MAX_X=10;
    double MIN_Z=-10;
    double MAX_Z=10;
};

#endif