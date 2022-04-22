#include "HeightMapGen.h"
#include <iostream>
#include <random>
#include <stdexcept>


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
    if(output_file.substr(output_file.find_last_of(".") + 1) != "bmp")
    {
        std::string ext(output_file.substr(output_file.find_last_of(".") + 1));
        throw std::invalid_argument(ext + " is not bmp");
    }
    writer.SetSourceImage(image);
    writer.SetDestFilename(DATA_DIR "/textures/" + output_file);
    writer.WriteDestFile();
}

/**
 * Avant de générer quoi que ce soit, il faut indiquer les limites de la boite englobante
 * Cette boite sert à libnoise pour générer des valeurs de bruits cohérents contenu à l'intérieur de cette boite
 * Ces valeurs sont uniformément espacées dans le rectangle.
 * Cette boite sert aussi pour le tiling, permet de continuer la heightmap de tous les côtés.
 * */
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

int HeightMapGen::randomBetween(int min, int max)
{
    std::random_device seed;
    std::mt19937 rng(seed());
    std::uniform_int_distribution<int> uni(min,max);
    return uni(rng);
}

double HeightMapGen::randomBetween(double min, double max)
{
    std::random_device seed;
    std::mt19937 rng(seed());
    std::uniform_real_distribution<double> uni(min,max);
    return uni(rng);
}

/**
 * Génère une boite anglobante entre les coordonnées minX, minZ, maxX et maxZ
 * */
void HeightMapGen::RandomBoundingSquare(double minX, double maxX, double minZ, double maxZ)
{
    if(minX >= maxX || minZ >= maxZ)
    {
        throw std::invalid_argument("minX >= maxX or minZ >= maxZ");
    }
    MIN_X = minX;
    MAX_X = maxX;
    MIN_Z = minZ;
    MAX_Z = maxZ;

    bs.lowerX = randomBetween(MIN_X, MAX_X);
    bs.upperX = randomBetween(bs.lowerX+.01f, MAX_X);
    bs.lowerZ = randomBetween(MIN_Z, MAX_Z);
    bs.upperZ = randomBetween(bs.lowerZ+.01f, MAX_Z);

}



void HeightMapGen::GenerateRandomHeightMap(int width, int height, double minX, double maxX, double minZ, double maxZ)
{
    RandomBoundingSquare(minX, maxX, minZ, maxZ);

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

utils::NoiseMapBuilderPlane HeightMapGen::GetBuilderPlane(void)
{
    return this->builder_plane;
}