#include "HeightMapGen.h"
#include <iostream>
#include <random>


void HeightMapGen::GeneratePlanarHeightMap(int width, int height)
{
    builder_plane.SetSourceModule(pModule);
    builder_plane.SetDestNoiseMap(heightMap);
    builder_plane.SetDestSize(width, height);
    builder_plane.SetBounds(bs.lowerX, bs.upperX, bs.lowerZ, bs.upperZ);
    builder_plane.Build();

    RenderProcessing();
}

void HeightMapGen::GenerateSphericalHeightMap(int width, int height)
{
    builder_sphere.SetSourceModule(pModule);
    builder_sphere.SetDestNoiseMap(heightMap);
    builder_sphere.SetDestSize(width, height);
    builder_sphere.SetBounds(-90.0, 90.0, -180.0, 180.0);
    builder_sphere.Build();

    RenderProcessing();
}

void HeightMapGen::RenderProcessing()
{
    renderer.SetSourceNoiseMap(heightMap);
    renderer.SetDestImage(image);
    renderer.EnableLight();
    renderer.SetLightContrast(3.0);
    renderer.SetLightBrightness(2.0);
    renderer.Render();
}

void HeightMapGen::Save(std::string output_file)
{
    writer.SetSourceImage(image);
    writer.SetDestFilename(DATA_DIR "/textures/" + output_file);
    writer.WriteDestFile();
}

void HeightMapGen::SetBoundingSquare(double lowerX, double upperX, double lowerZ, double upperZ)
{
    bs.lowerX = lowerX;
    bs.upperX = upperX;
    bs.lowerZ = lowerZ;
    bs.upperZ = upperZ;
}

/**
 * Controls the number of details in Perlin Noise
 * The more octaves there are, the noisier it gets.
 * Each octave has double the frequency of the last one, and a smaller amplitude.
 * Also increase calculation time
 * */
void HeightMapGen::SetOctave(int octave)
{
    pModule.SetOctaveCount(octave);
}

/**
 * Sets the frequency for the first octave
 * A higher frequency means more Terrain features but smaller features.
 * Values between 1 and 16
 * */
void HeightMapGen::SetFrequency(double f)
{
    pModule.SetFrequency(f);
}

/**
 * determines how quickly the amplitudes fall for each successive octave
 * increasing the value = rougher Terrain
 * decreasing = smoother Terrain
 * Values between 0 and 1
 * */
void HeightMapGen::SetPersistence(double p)
{
    pModule.SetPersistence(p);
}

int randomBetween(int min, int max)
{
    std::random_device seed;
    std::mt19937 rng(seed());
    std::uniform_int_distribution<int> uni(min,max);
    return uni(rng);
}

double randomBetween(double min, double max)
{
    std::random_device seed;
    std::mt19937 rng(seed());
    std::uniform_real_distribution<double> uni(min,max);
    return uni(rng);
}



void HeightMapGen::GenerateRandomHeightMap(int width, int height)
{
    bs.lowerX = randomBetween(MIN_X, MAX_X);
    bs.upperX = randomBetween((int)bs.lowerX+1, MAX_X);
    bs.lowerZ = randomBetween(MIN_Z, MAX_Z);
    bs.upperZ = randomBetween((int)bs.lowerZ+1, MAX_Z);

    SetOctave(randomBetween(1, 8));
    SetFrequency(randomBetween(1, 16));
    SetPersistence(randomBetween(0.0, 1.0));
    builder_plane.SetSourceModule(pModule);
    builder_plane.SetDestNoiseMap(heightMap);
    builder_plane.SetDestSize(width, height);
    builder_plane.SetBounds(bs.lowerX, bs.upperX, bs.lowerZ, bs.upperZ);
    builder_plane.Build();
    RenderProcessing();
}