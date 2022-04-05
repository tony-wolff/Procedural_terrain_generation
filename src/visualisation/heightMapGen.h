#ifndef _HM_gen_h
#define _HM_gen_h

#include <noise/noise.h>
#include <noiseutils.h>

class heightMapGen;

using namespace noise;

class heightMapGen {

public:
    heightMapGen();
    void setSize();
    void setBuilder();
    void setWriter();
    void save();
    void generateRandomHeightMap();

private:
    module::Perlin pModule;
    utils::NoiseMap heightMap;
    utils::NoiseMapBuilderPlane heightMapBuilder;
    utils::RendererImage renderer;
    utils::Image image;
    utils::WriterBMP writer;
};

#endif