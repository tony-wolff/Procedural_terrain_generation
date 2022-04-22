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
    CPPUNIT_TEST(testChildrenSize);
    CPPUNIT_TEST_SUITE_END();
    public:
        void setUp(void);
        void tearDown(void);
    private:
        QuadTree* qt;
        std::vector<TerrainGeneration::QTNode> nodearray;

    protected:
        void testChildrenInBox(void);
        void testChildrenSize(void);
        void recurseThroughChild(TerrainGeneration::QTNode parent, bool (* fun)(TerrainGeneration::QTNode, TerrainGeneration::QTNode));
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

/*
*   Predicate function, 1st parameter is the parent, 2nd is the child
*/
typedef bool (*test_function)(TerrainGeneration::QTNode, TerrainGeneration::QTNode);

bool isInBox(TerrainGeneration::QTNode parent, TerrainGeneration::QTNode children) {
    if (children.minX >= parent.minX && children.maxX <= parent.maxX 
        && children.minZ >= parent.minZ && children.maxZ <= parent.maxZ)
            return true;
    
    return false;
}

bool isRightSize(TerrainGeneration::QTNode parent, TerrainGeneration::QTNode children) {
    return children.width == parent.width / 2 && children.height == parent.height / 2;
}

/*
*   Check if all the nodes in the QuadTree respects the predicate pred
*/
void TestQuadTree::recurseThroughChild(TerrainGeneration::QTNode parent, test_function pred) {
    CPPUNIT_ASSERT(pred(parent, nodearray.at(parent.childrenIndex[0])));
    recurseThroughChild(nodearray.at(parent.childrenIndex[0]), pred);

    CPPUNIT_ASSERT(pred(parent, nodearray.at(parent.childrenIndex[1])));
    recurseThroughChild(nodearray.at(parent.childrenIndex[1]), pred);

    CPPUNIT_ASSERT(pred(parent, nodearray.at(parent.childrenIndex[2])));
    recurseThroughChild(nodearray.at(parent.childrenIndex[2]), pred);

    CPPUNIT_ASSERT(pred(parent, nodearray.at(parent.childrenIndex[3])));
    recurseThroughChild(nodearray.at(parent.childrenIndex[3]), pred);
}

void TestQuadTree::testChildrenInBox(void) {
    recurseThroughChild(nodearray.at(0), &isInBox);
}

void TestQuadTree::testChildrenSize(void) {
    recurseThroughChild(nodearray.at(0), &isRightSize);
}

CPPUNIT_TEST_SUITE_REGISTRATION( TestQuadTree );

int main() {
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