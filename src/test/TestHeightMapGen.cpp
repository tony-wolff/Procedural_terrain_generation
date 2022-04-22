#include <cppunit/TestCase.h>
#include <cppunit/TestCase.h>
#include <cppunit/TestFixture.h>
#include <cppunit/ui/text/TextTestRunner.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/XmlOutputter.h>
#include <chrono>
#include <stdexcept>
#include <ImageMagick-7/Magick++.h>
#include "../visualisation/HeightMapGen.h"
#include <unistd.h>

using namespace CppUnit;

class TestHeightMapGen : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestHeightMapGen);
    CPPUNIT_TEST(testRandomBetween);
    CPPUNIT_TEST(testRandomBoundingBox);
    CPPUNIT_TEST(testBoundingBox);
    //CPPUNIT_TEST(testBenchmarkStress);
    CPPUNIT_TEST(testSize);
    //CPPUNIT_TEST(testBenchmarkSize);
    CPPUNIT_TEST(testSave);
    CPPUNIT_TEST(testSphericalGeneration);
    CPPUNIT_TEST(testExistingHmap);
    CPPUNIT_TEST(testSetOctave);
    CPPUNIT_TEST(testSetFrequency);
    CPPUNIT_TEST(testRandomGeneration);
    CPPUNIT_TEST_SUITE_END();
    public:
        void setUp(void);
        void tearDown(void);
    protected:
        void testSize(void); //Done
        void testRandomBetween(void); //Done
        void testRandomBoundingBox(void); //Done
        void testBenchmarkSize(void); //Done
        void testBenchmarkStress(void); //Done
        void testBoundingBox(void); //Done
        void testSave(void); //Done
        void testSphericalGeneration(void); //Done
        void testExistingHmap(void); //Done
        void testSetOctave(void); //Done
        void testSetFrequency(void);// Done
        void testRandomGeneration(void);

    private:
        HeightMapGen *hmgen;
};

void TestHeightMapGen::setUp(void)
{
    hmgen = new HeightMapGen();
}


void TestHeightMapGen::testRandomBetween(void)
{
    double d1 = hmgen->randomBetween(0, 1);
    CPPUNIT_ASSERT(d1 >= 0 && d1 <= 1);
    double d2 = hmgen->randomBetween(-100, 0);
    CPPUNIT_ASSERT(d2 >= -100 && d2 <= 0);
    double d3 = hmgen->randomBetween(1.2, 1.8);
    CPPUNIT_ASSERT(d3 >= 1.2 && d3 <= 1.8);
    double d4 = hmgen->randomBetween(-10000.5, -10000.0);
    CPPUNIT_ASSERT(d4 >= -10000.5 && d4 <= -10000.0);
}

void TestHeightMapGen::testRandomBoundingBox(void)
{
    //Tests négatifs
    CPPUNIT_ASSERT_THROW(hmgen->RandomBoundingSquare(0, 0, 0, 0), std::invalid_argument);
    CPPUNIT_ASSERT_THROW(hmgen->RandomBoundingSquare(0, -1, 0, 10), std::invalid_argument);
    CPPUNIT_ASSERT_THROW(hmgen->RandomBoundingSquare(50.5, 50, 10, 9.9), std::invalid_argument);
    //Test à la frontière
    hmgen->RandomBoundingSquare(-10, 10, -5, 5);
    CPPUNIT_ASSERT_NO_THROW(hmgen->GeneratePlanarHeightMap(1, 1));
    CPPUNIT_ASSERT(hmgen->GetBuilderPlane().GetLowerXBound() >= -10 && hmgen->GetBuilderPlane().GetLowerXBound() < 10);
    CPPUNIT_ASSERT(hmgen->GetBuilderPlane().GetUpperXBound() > hmgen->GetBuilderPlane().GetLowerXBound() &&
    hmgen->GetBuilderPlane().GetUpperXBound() <= 10);
    CPPUNIT_ASSERT(hmgen->GetBuilderPlane().GetLowerZBound() >= -5 && hmgen->GetBuilderPlane().GetLowerZBound() < 5);
    CPPUNIT_ASSERT(hmgen->GetBuilderPlane().GetUpperZBound() >hmgen->GetBuilderPlane().GetLowerZBound() &&
    hmgen->GetBuilderPlane().GetUpperZBound() <= 5);

    //Test avec des double
    hmgen->RandomBoundingSquare(-1.4, 0.1, 4.1, 4.3);
    CPPUNIT_ASSERT_NO_THROW(hmgen->GeneratePlanarHeightMap(1, 1));
    double lowerX = hmgen->GetBuilderPlane().GetLowerXBound();
    double lowerZ = hmgen->GetBuilderPlane().GetLowerZBound();
    double upperX = hmgen->GetBuilderPlane().GetUpperXBound();
    double upperZ = hmgen->GetBuilderPlane().GetUpperZBound();

    CPPUNIT_ASSERT(lowerX >= -1.4 && lowerX < 0.1);
    CPPUNIT_ASSERT(upperX > lowerX && upperX <= 0.1);
    CPPUNIT_ASSERT(lowerZ >= 4.1 && upperZ < 4.3);
    CPPUNIT_ASSERT(upperZ > lowerZ && upperZ <= 4.3);
}

void TestHeightMapGen::testBoundingBox(void)
{
    //rectangle quelconque
    hmgen->SetBoundingSquare(0, 2, 1, 8);
    hmgen->GeneratePlanarHeightMap(1, 1);
    CPPUNIT_ASSERT(hmgen->GetBuilderPlane().GetLowerXBound() == 0);
    CPPUNIT_ASSERT(hmgen->GetBuilderPlane().GetUpperXBound() == 2);
    CPPUNIT_ASSERT(hmgen->GetBuilderPlane().GetLowerZBound() == 1);
    CPPUNIT_ASSERT(hmgen->GetBuilderPlane().GetUpperZBound() == 8);

    //pas un rectangle mais un point
    hmgen->SetBoundingSquare(0, 0, 0, 0);
    CPPUNIT_ASSERT_THROW(hmgen->GeneratePlanarHeightMap(1, 1), noise::ExceptionInvalidParam);

    //Pas un rectangle mais une ligne
    hmgen->SetBoundingSquare(0, 0, 4, 4);
    CPPUNIT_ASSERT_THROW(hmgen->GeneratePlanarHeightMap(1, 1), noise::ExceptionInvalidParam);

    //rectangle avec lowerX > upperX
    hmgen->SetBoundingSquare(5, 3, 1, 8);
    CPPUNIT_ASSERT_THROW(hmgen->GeneratePlanarHeightMap(1, 1), noise::ExceptionInvalidParam);

    //rectangle avec lowerZ > upperZ
    hmgen->SetBoundingSquare(4, 5, 7, 6);
    CPPUNIT_ASSERT_THROW(hmgen->GeneratePlanarHeightMap(1, 1), noise::ExceptionInvalidParam);

    //rectangle avec des double
    hmgen->SetBoundingSquare(0.2, 0.6, 1.5, 8.3);
    hmgen->GeneratePlanarHeightMap(1, 1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.2, hmgen->GetBuilderPlane().GetLowerXBound(), 0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.6, hmgen->GetBuilderPlane().GetUpperXBound(), 0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.5, hmgen->GetBuilderPlane().GetLowerZBound(), 0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(8.3, hmgen->GetBuilderPlane().GetUpperZBound(), 0);

    //rectangle négatif
    hmgen->SetBoundingSquare(-5, -3.5, -10, -2.2);
    CPPUNIT_ASSERT_NO_THROW(hmgen->GeneratePlanarHeightMap(1, 1));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-5, hmgen->GetBuilderPlane().GetLowerXBound(), 0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-3.5, hmgen->GetBuilderPlane().GetUpperXBound(), 0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-10, hmgen->GetBuilderPlane().GetLowerZBound(), 0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-2.2, hmgen->GetBuilderPlane().GetUpperZBound(), 0);

    //rectangle à la frontière positif-négatif
    hmgen->SetBoundingSquare(-2, 3, -1.7, 2.4);
    CPPUNIT_ASSERT_NO_THROW(hmgen->GeneratePlanarHeightMap(1, 1));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-2, hmgen->GetBuilderPlane().GetLowerXBound(), 0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3, hmgen->GetBuilderPlane().GetUpperXBound(), 0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.7, hmgen->GetBuilderPlane().GetLowerZBound(), 0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.4, hmgen->GetBuilderPlane().GetUpperZBound(), 0);
}

void TestHeightMapGen::testSize(void)
{
    hmgen->SetBoundingSquare(0, 1, 1, 5);
    //Taille quelconque
    CPPUNIT_ASSERT_NO_THROW(hmgen->GeneratePlanarHeightMap(150, 150));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(150, hmgen->GetBuilderPlane().GetDestHeight(), 0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(150, hmgen->GetBuilderPlane().GetDestWidth(), 0);

    //Taille hors du domaine
    CPPUNIT_ASSERT_THROW(hmgen->GeneratePlanarHeightMap(0, 0), noise::ExceptionInvalidParam);
    CPPUNIT_ASSERT_THROW(hmgen->GeneratePlanarHeightMap(-5, 5), noise::ExceptionInvalidParam);
    CPPUNIT_ASSERT_THROW(hmgen->GeneratePlanarHeightMap(-1, 0), noise::ExceptionInvalidParam);
    CPPUNIT_ASSERT_THROW(hmgen->GeneratePlanarHeightMap(0, 40), noise::ExceptionInvalidParam);

    //Taille gigantesque, ne pas décommentez si vous n'avez pas le temps
    // CPPUNIT_ASSERT_NO_THROW(hmgen->GeneratePlanarHeightMap(10000, 10000));
    // CPPUNIT_ASSERT_DOUBLES_EQUAL(10000, hmgen->GetBuilderPlane().GetDestHeight(), 0);
    // CPPUNIT_ASSERT_DOUBLES_EQUAL(10000, hmgen->GetBuilderPlane().GetDestWidth(), 0);

    //Taille max : 32767 A NE DECOMMENTEZ QUE SI VOUS AVEZ 10H DEVANT VOUS
    // CPPUNIT_ASSERT_NO_THROW(hmgen->GeneratePlanarHeightMap(32767, 32767));
    // CPPUNIT_ASSERT_DOUBLES_EQUAL(32767, hmgen->GetBuilderPlane().GetDestHeight(), 0);
    // CPPUNIT_ASSERT_DOUBLES_EQUAL(32767, hmgen->GetBuilderPlane().GetDestWidth(), 0);

    CPPUNIT_ASSERT_THROW(hmgen->GeneratePlanarHeightMap(33000, 33000), noise::ExceptionInvalidParam);
}

void TestHeightMapGen::testBenchmarkStress(void)
{
    std::ofstream benchFile("benchMarkStress.txt");
    //100 itérations sur des hmap de 500x500 = 100*0.5s = 50secondes
    benchFile << "size of 500x500\n\n";
    for(int i=0; i<100; i++)
    {
        hmgen->RandomBoundingSquare(-100, 100, -100, 100);
        benchFile << "bounding square" << "lowerX" << hmgen->GetBuilderPlane().GetLowerXBound() <<
        " upperX " << hmgen->GetBuilderPlane().GetUpperXBound() << " lowerZ " << hmgen->GetBuilderPlane().GetLowerZBound() <<
        "UpperZ " << hmgen->GetBuilderPlane().GetUpperZBound();
        auto start = std::chrono::high_resolution_clock::now();
        CPPUNIT_ASSERT_NO_THROW(hmgen->GeneratePlanarHeightMap(500, 500));
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
        benchFile << "\t duration: " << duration.count() << "\n";
    }
    benchFile.close();
}

void TestHeightMapGen::testBenchmarkSize(void)
{
    std::ofstream benchFile("benchmarkSize-time.txt");
    hmgen->SetBoundingSquare(-100, 100, 0, 50);
    benchFile << "Bouding Square: " << "lowerX " << -100 <<" upperX " << 100<< "lowerZ " << 0 << "upperZ "<< 50;
    benchFile << "\n\n";
    for(int i=100; i<=2000; i+=100)
    {
        auto start = std::chrono::high_resolution_clock::now();
        hmgen->GeneratePlanarHeightMap(i, i);
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
        benchFile <<"Size: " << i << "x" << i << "\tduration: " << duration.count() << "milliseconds\n";
    }
    benchFile.close();
}

void TestHeightMapGen::testSave(void)
{
    //sauvegarde d'un fichier au format raw
    hmgen->SetBoundingSquare(-10, 1, -10, 1);
    hmgen->GeneratePlanarHeightMap(10, 10);
    CPPUNIT_ASSERT_THROW(hmgen->Save("test_save.raw"), std::invalid_argument);
    CPPUNIT_ASSERT_THROW(hmgen->Save("test_save.png"), std::invalid_argument);
    CPPUNIT_ASSERT_THROW(hmgen->Save("test_save.jpg"), std::invalid_argument);
    CPPUNIT_ASSERT_THROW(hmgen->Save("test_save.JPEG"), std::invalid_argument);
    CPPUNIT_ASSERT_NO_THROW(hmgen->Save("test_save.bmp"));

    Magick::Image i;
    CPPUNIT_ASSERT_NO_THROW(i.read(DATA_DIR "/textures/test_save.bmp"));
    int width = i.columns();
    int height = i.rows();
    std::string format = i.magick();
    CPPUNIT_ASSERT_EQUAL(10, width);
    CPPUNIT_ASSERT_EQUAL(10, height);
    CPPUNIT_ASSERT_EQUAL(std::string("BMP3"), format);
}

void TestHeightMapGen::testSphericalGeneration(void)
{
    //Test négatif
    CPPUNIT_ASSERT_THROW(hmgen->GenerateSphericalHeightMap(0, 0), noise::ExceptionInvalidParam);

    //Heightmap 1
    CPPUNIT_ASSERT_NO_THROW(hmgen->GenerateSphericalHeightMap(512, 512));
    CPPUNIT_ASSERT_NO_THROW(hmgen->Save("test_sphericalhm.bmp"));

    Magick::Image i;
    CPPUNIT_ASSERT_NO_THROW(i.read(DATA_DIR "/textures/test_sphericalhm.bmp"));
    int width = i.columns();
    int height = i.rows();
    std::string format = i.magick();
    CPPUNIT_ASSERT_EQUAL(512, width);
    CPPUNIT_ASSERT_EQUAL(512, height);
    CPPUNIT_ASSERT_EQUAL(std::string("BMP3"), format);

    //Heightmap 2
    CPPUNIT_ASSERT_NO_THROW(hmgen->GenerateSphericalHeightMap(256, 512));
    CPPUNIT_ASSERT_NO_THROW(hmgen->Save("test_sphericalhm2.bmp"));

    Magick::Image i2;
    CPPUNIT_ASSERT_NO_THROW(i2.read(DATA_DIR "/textures/test_sphericalhm2.bmp"));
    width = i2.columns();
    height = i2.rows();
    format = i2.magick();
    CPPUNIT_ASSERT_EQUAL(256, width);
    CPPUNIT_ASSERT_EQUAL(512, height);
    CPPUNIT_ASSERT_EQUAL(std::string("BMP3"), format);
}

void createHmapForTesting(int width, int height, double lowerX, double upperX, double lowerZ, double upperZ, std::string name, int octave=-1, double frequency=-1, double persistence=-1)
{
  module::Perlin myModule;
  utils::NoiseMap heightMap;
  utils::NoiseMapBuilderPlane heightMapBuilder;
  if(octave != -1)
    myModule.SetOctaveCount(octave);
  if(frequency != -1)
    myModule.SetFrequency(frequency);
  if(persistence != -1)
    myModule.SetPersistence(persistence);
  heightMapBuilder.SetSourceModule (myModule);
  heightMapBuilder.SetDestNoiseMap (heightMap);
  heightMapBuilder.SetDestSize (width, height);
  heightMapBuilder.SetBounds (lowerX, upperX, lowerZ, upperZ);
  heightMapBuilder.Build();

  utils::RendererImage renderer;
  utils::Image image;
  renderer.SetSourceNoiseMap(heightMap);
  renderer.SetDestImage (image);
  renderer.EnableLight ();
  renderer.SetLightContrast (3.0);
  renderer.SetLightBrightness (2.0);
  renderer.Render();

  utils::WriterBMP writer;
  writer.SetSourceImage (image);
  writer.SetDestFilename (DATA_DIR "/textures/" + name);
  writer.WriteDestFile();

}

void TestHeightMapGen::testExistingHmap(void)
{
    createHmapForTesting(256, 256, 0.0, 5.0, 2.0, 8.0, std::string("existinghmap.bmp"));
    hmgen->SetBoundingSquare(0.0, 5.0, 2.0, 8.0);
    hmgen->GeneratePlanarHeightMap(256, 256);
    hmgen->Save("test_hmap_with_existing.bmp");

    Magick::Image i;
    Magick::Image i2;
    i.read(DATA_DIR "/textures/existinghmap.bmp");
    i2.read(DATA_DIR "/textures/test_hmap_with_existing.bmp");
    for(int row=0; row<i.rows(); row++)
    {
        for(int col=0; col<i.columns(); col++){
            Magick::Color color1 = i.pixelColor(row, col);
            Magick::Color color2 = i2.pixelColor(row, col);
            CPPUNIT_ASSERT(color1 == color2);
        }
    }
}

void TestHeightMapGen::testSetOctave(void)
{
    hmgen->SetBoundingSquare(2.0, 6.0, 1.0, 5.0);
    CPPUNIT_ASSERT_THROW(hmgen->SetOctave(0), noise::ExceptionInvalidParam);
    CPPUNIT_ASSERT_THROW(hmgen->SetOctave(-1), noise::ExceptionInvalidParam);
    CPPUNIT_ASSERT_THROW(hmgen->SetOctave(31), noise::ExceptionInvalidParam);
    CPPUNIT_ASSERT_NO_THROW(hmgen->SetOctave(8));
    hmgen->GeneratePlanarHeightMap(256, 256);

    createHmapForTesting(256, 256, 2.0, 6.0, 1.0, 5.0, "test_octave.bmp", 8);
    hmgen->Save("test_octave2.bmp");
    Magick::Image img;
    Magick::Image img2;

    img.read(DATA_DIR "/textures/test_octave.bmp");
    img.read(DATA_DIR "/textures/test_octave2.bmp");
    for(int i=0; i<img.rows(); i++){
        for(int j=0; j<img.columns(); j++){
            Magick::Color color1 = img.pixelColor(i, j);
            Magick::Color color2 = img2.pixelColor(i, j);
            CPPUNIT_ASSERT(color1 != color2);
        }
    }
}

void TestHeightMapGen::testSetFrequency(void)
{
    CPPUNIT_ASSERT_NO_THROW(hmgen->SetFrequency(-10));
    CPPUNIT_ASSERT_NO_THROW(hmgen->SetFrequency(800));
    CPPUNIT_ASSERT_NO_THROW(hmgen->SetFrequency(1));
    CPPUNIT_ASSERT_NO_THROW(hmgen->SetFrequency(200));
    CPPUNIT_ASSERT_NO_THROW(hmgen->SetFrequency(4.20));
}


void TestHeightMapGen::testRandomGeneration(void)
{
    //Test aléatoire
    for(int i=0; i<100; i++)
    {
        CPPUNIT_ASSERT_NO_THROW(hmgen->GenerateRandomHeightMap(256, 256, 0.0, 10.0, -5.2, 2.0));
    }

    //Test négatif
    CPPUNIT_ASSERT_THROW(hmgen->GenerateRandomHeightMap(0, 0, 1, 5, 2, 3),  noise::ExceptionInvalidParam);
    CPPUNIT_ASSERT_THROW(hmgen->GenerateRandomHeightMap(2, 2, 1, 0.9, 5, -10), std::invalid_argument);


    //Test d'une carte aléatoire
    hmgen->GenerateRandomHeightMap(256, 512, -10, 10, -10, 10);
    CPPUNIT_ASSERT_NO_THROW(hmgen->Save("randomMap.bmp"));

    Magick::Image img;
    CPPUNIT_ASSERT_NO_THROW(img.read(DATA_DIR "/textures/randomMap.bmp"));
    int w = img.columns();
    int h = img.rows();
    std::string format = img.magick();
    CPPUNIT_ASSERT_EQUAL(256, w);
    CPPUNIT_ASSERT_EQUAL(512, h);
    CPPUNIT_ASSERT_EQUAL(std::string("BMP3"), format);

}

void TestHeightMapGen::tearDown(void)
{
    delete hmgen;
}

CPPUNIT_TEST_SUITE_REGISTRATION(TestHeightMapGen);

int main(int argc, char* argv[])
{
    CPPUNIT_NS::TestResult testresult;
    CPPUNIT_NS::TestResultCollector collectedresults;
    testresult.addListener(&collectedresults);

    CPPUNIT_NS::BriefTestProgressListener progress;
    testresult.addListener(&progress);

    CPPUNIT_NS::TestRunner testrunner;
    testrunner.addTest(CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest());
    testrunner.run(testresult);

    CPPUNIT_NS::CompilerOutputter compileroutputter(&collectedresults, std::cerr);
    compileroutputter.write();

    std::ofstream xmlFileOut("TestHeightMapGen.xml");
    XmlOutputter xmlOut(&collectedresults, xmlFileOut);
    xmlOut.write();

    return collectedresults.wasSuccessful() ? 0 : 1;
}