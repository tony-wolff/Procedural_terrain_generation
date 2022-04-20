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

#include "../visualisation/QuadTree.h"

using namespace CppUnit;

class TestQuadTree : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestQuadTree);
    CPPUNIT_TEST_SUITE_END();
    public:
        void setUp(void);
        void tearDown(void);
    private:
        TerrainGeneration* qt;
        std::vector<TerrainGeneration::QTNode> nodearray;
};

void TestQuadTree::setUp(void) {
    qt = new QuadTree();
    qt->compile(0, 512, 0, 512, 512, 512);
    nodearray = qt->getResult(6);
}

void TestQuadTree::tearDown(void) {
    // Clear vector
    std::vector<TerrainGeneration::QTNode>().swap(nodearray);
}

CPPUNIT_TEST_SUITE_REGISTRATION( TestQuadTree );

int main(int argc, char* argv[]) {
    return 0;
}