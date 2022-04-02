#include <Eigen/Dense>
#include <vector>

using namespace std;
using namespace Eigen;

class QuadTree
{
  private:
    typedef struct {
      ssize_t x, z;
      double y;
      int level;
      int width, height;
      float minX, maxX, minZ, maxZ;
      
      vector<Vertex> vertices;
      unsigned int childrenIndex[4];
    } QTNode;

    vector<QTNode> nodearray;
    unsigned int width;
    unsigned int height;
    unsigned int nodeIndex;

    void createNode(QTNode parent);

  public:
    QuadTree(float minX, float maxX, float minZ, float maxZ, int width, int height);
    QuadTree();
    QuadTree(MatrixXf heightmap);
    QuadTree(const QuadTree&);
    ~QuadTree();
    vector<double> getResult(int level);

};