#ifndef _HM_gen_h
#define _HM_gen_h

#include <noise/noise.h>
#include <noiseutils.h>
#include <string>

class heightMapGen;

using namespace noise;

struct bounding_square 
{
    double lowerX;
    double upperX;
    double lowerZ;
    double upperZ;
};
class heightMapGen {

public:
    void SetSize(int width, int height);
    void SetPlanarBuilder();
    void SetSphericalBuilder();
    void Save(std::string output_file);
    void GenerateRandomHeightMap();
    void GeneratePlanarHeightMap(int width, int height);
    void GenerateSphericalHeightMap(int width, int height);
    void SetBoundingSquare(double lowerX, double upperX, double lowerZ, double upperZ);
    void SetOctave(int octave);
    void SetFrequency(double frequency);
    void SetPersistence(double persistence);
    void GenerateRandomHeightMap(int with, int height);

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
    const int MIN_X=-10;
    const int MAX_X=10;
    const int MIN_Z=-10;
    const int MAX_Z=10;
};

#endif