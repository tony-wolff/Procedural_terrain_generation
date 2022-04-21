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
    CPPUNIT_TEST(testChildrenInBox);
    CPPUNIT_TEST_SUITE_END();
    public:
        void setUp(void);
        void tearDown(void);
    private:
        QuadTree* qt;
        std::vector<TerrainGeneration::QTNode> nodearray;

    protected:
        void testChildrenInBox(void);
};

void TestQuadTree::setUp(void) {
    qt = new QuadTree();
    qt->compile(0, 512, 0, 512, 512, 512);
    nodearray = qt->nodearray;
}

void TestQuadTree::tearDown(void) {
    // Clear vector
    std::vector<TerrainGeneration::QTNode>().swap(nodearray);
}

bool isInBox(TerrainGeneration::QTNode parent, TerrainGeneration::QTNode children) {
    if (children.minX >= parent.minX && children.maxX <= parent.maxX 
        && children.minZ >= parent.minZ && children.maxZ <= parent.maxZ)
            return true;
    
    return false;
}

void TestQuadTree::testChildrenInBox(void) {
    for (unsigned long index = 0; index < nodearray.size(); index++) {
        TerrainGeneration::QTNode parentNode = nodearray.at(index);
        
        if (parentNode.level < qt->getMaxLevel()) {
            std::cout << "level " << parentNode.level << std::endl;
            std::cout << "index " << parentNode.childrenIndex[0] << std::endl;
            std::cout << "parent " << parentNode.minX << std::endl;
            std::cout << "parent " << parentNode.maxX << std::endl;
            std::cout << "parent " << parentNode.minZ << std::endl;
            std::cout << "parent " << parentNode.maxZ << std::endl;
            std::cout << "childLevel " << nodearray.at(parentNode.childrenIndex[3]).minX << std::endl;
            std::cout << "childLevel " << nodearray.at(parentNode.childrenIndex[3]).maxX << std::endl;
            std::cout << "childLevel " << nodearray.at(parentNode.childrenIndex[3]).minZ << std::endl;
            std::cout << "childLevel " << nodearray.at(parentNode.childrenIndex[3]).maxZ << std::endl;
            CPPUNIT_ASSERT(isInBox(parentNode, nodearray.at(parentNode.childrenIndex[0])));
            CPPUNIT_ASSERT(isInBox(parentNode, nodearray.at(parentNode.childrenIndex[1])));
            CPPUNIT_ASSERT(isInBox(parentNode, nodearray.at(parentNode.childrenIndex[2])));
            CPPUNIT_ASSERT(isInBox(parentNode, nodearray.at(parentNode.childrenIndex[3])));
        }
    }
        
}

CPPUNIT_TEST_SUITE_REGISTRATION( TestQuadTree );

int main(int argc, char* argv[]) {
    CPPUNIT_NS::TestResult testresult;

    CPPUNIT_NS::TestResultCollector collectedresults;
    testresult.addListener (&collectedresults);

    CPPUNIT_NS::BriefTestProgressListener progress;
    testresult.addListener (&progress);

    CPPUNIT_NS::TestRunner testrunner;
    testrunner.addTest (CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest ());
    testrunner.run(testresult);

    CPPUNIT_NS::CompilerOutputter compileroutputter(&collectedresults, std::cerr);
    compileroutputter.write ();

    ofstream xmlFileOut("testResult.xml");
    XmlOutputter xmlOut(&collectedresults, xmlFileOut);
    xmlOut.write();

    return collectedresults.wasSuccessful() ? 0 : 1;
}