#include <Eigen/Dense>
#include <vector>
#include "opengl.h"

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
      
      vector<Vector2f> vertices;
      unsigned int childrenIndex[4];
    } QTNode;
    unsigned int width;
    unsigned int height;
    unsigned int nodeIndex;

    void createNode(QTNode parent);

  public:
    vector<QTNode> nodearray;

    QuadTree(float minX, float maxX, float minZ, float maxZ, int width, int height);
    QuadTree();
    QuadTree(MatrixXf heightmap);
    QuadTree(const QuadTree&);
    ~QuadTree();
    vector<double> getResult(int level);

};
