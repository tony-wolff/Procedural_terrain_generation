#include "heightMapGen.h"
#include <iostream>
heightMapGen::heightMapGen()
{
    heightMapBuilder.SetSourceModule(pModule);
    heightMapBuilder.SetDestNoiseMap(heightMap);
    std::cout << "test\n" << std::endl;
    heightMapBuilder.SetDestSize(512, 512);
    std::cout << "test 2\n" << std::endl;
    heightMapBuilder.SetBounds(2.0, 6.0, 1.0, 5.0);
    std::cout << "test 3\n" << std::endl;
    heightMapBuilder.Build();
    
    renderer.SetSourceNoiseMap(heightMap);
    renderer.SetDestImage(image);
    renderer.EnableLight();
    renderer.SetLightContrast(3.0);
    renderer.SetLightBrightness(2.0);
    renderer.Render();
}

void heightMapGen::save()
{
    writer.SetSourceImage(image);
    writer.SetDestFilename(DATA_DIR "/textures/Heightmap.bmp");
    writer.WriteDestFile();
}