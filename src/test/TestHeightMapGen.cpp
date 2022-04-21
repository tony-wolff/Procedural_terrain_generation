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

#include "../visualisation/HeightMapGen.h"

using namespace CppUnit;

class TestHeightMapGen : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestHeightMapGen);
    CPPUNIT_TEST(testBoundingBox);
    //CPPUNIT_TEST(testBenchmark);
    CPPUNIT_TEST_SUITE_END();
    public:
        void setUp(void);
        void tearDown(void);
    protected:
        void testBenchmark(void);
        void testSize(void);
        void testSphericalGeneration(void);
        void testBoundingBox(void);
        void testPlanarGeneration(void);
        void testExistingHmap(void);
        void testSetOctave(void);
        void testSetFrequency(void);
        void testSetPersistance(void);
        void testRandomGeneration(void);
        void testSave(void);

    private:
        HeightMapGen *hmgen;
};

void TestHeightMapGen::setUp(void)
{
    hmgen = new HeightMapGen();
}

void TestHeightMapGen::testBoundingBox(void)
{
    //rectangle quelconque
    hmgen->SetBoundingSquare(0, 2, 1, 8);
    hmgen->GeneratePlanarHeightMap(0, 0);
    CPPUNIT_ASSERT(hmgen->GetBuilderPlane().GetLowerXBound() == 0);
    CPPUNIT_ASSERT(hmgen->GetBuilderPlane().GetUpperXBound() == 2);
    CPPUNIT_ASSERT(hmgen->GetBuilderPlane().GetLowerZBound() == 1);
    CPPUNIT_ASSERT(hmgen->GetBuilderPlane().GetUpperZBound() == 8);

    //pas un rectangle mais un point
    hmgen->SetBoundingSquare(0, 0, 0, 0);
    CPPUNIT_ASSERT_THROW(hmgen->GeneratePlanarHeightMap(0, 0), noise::ExceptionInvalidParam);

    //Pas un rectangle mais une ligne
    hmgen->SetBoundingSquare(0, 0, 4, 4);
    CPPUNIT_ASSERT_THROW(hmgen->GeneratePlanarHeightMap(0, 0), noise::ExceptionInvalidParam);

    //rectangle avec lowerX > upperX
    hmgen->SetBoundingSquare(5, 3, 1, 8);
    CPPUNIT_ASSERT_THROW(hmgen->GeneratePlanarHeightMap(0, 0), noise::ExceptionInvalidParam);

    //rectangle avec lowerZ > upperZ
    hmgen->SetBoundingSquare(4, 5, 7, 6);
    CPPUNIT_ASSERT_THROW(hmgen->GeneratePlanarHeightMap(0, 0), noise::ExceptionInvalidParam);

    //rectangle avec des double
    hmgen->SetBoundingSquare(0.2, 0.6, 1.5, 8.3);
    hmgen->GeneratePlanarHeightMap(0, 0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.2, hmgen->GetBuilderPlane().GetLowerXBound(), 0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.6, hmgen->GetBuilderPlane().GetUpperXBound(), 0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.5, hmgen->GetBuilderPlane().GetLowerZBound(), 0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(8.3, hmgen->GetBuilderPlane().GetUpperZBound(), 0);

    //rectangle négatif
    hmgen->SetBoundingSquare(-5, -3.5, -10, -2.2);
    CPPUNIT_ASSERT_NO_THROW(hmgen->GeneratePlanarHeightMap(0, 0));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-5, hmgen->GetBuilderPlane().GetLowerXBound(), 0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-3.5, hmgen->GetBuilderPlane().GetUpperXBound(), 0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-10, hmgen->GetBuilderPlane().GetLowerZBound(), 0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-2.2, hmgen->GetBuilderPlane().GetUpperZBound(), 0);

    //rectangle à la frontière
    hmgen->SetBoundingSquare(-2, 3, -1.7, 2.4);
    CPPUNIT_ASSERT_NO_THROW(hmgen->GeneratePlanarHeightMap(0, 0));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-2, hmgen->GetBuilderPlane().GetLowerXBound(), 0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3, hmgen->GetBuilderPlane().GetUpperXBound(), 0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.7, hmgen->GetBuilderPlane().GetLowerZBound(), 0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.4, hmgen->GetBuilderPlane().GetUpperZBound(), 0);

}

void TestHeightMapGen::testBenchmark(void)
{
    for(int i=0; i<100; i++)
    {
        hmgen->GeneratePlanarHeightMap(0, 0);
    }
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