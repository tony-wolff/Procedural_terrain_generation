#include <Eigen/Dense>
#include <vector>
#include "opengl.h"

using namespace std;
using namespace Eigen;

struct pos {int x, z; };

class QuadTree
{
  private:
    typedef struct
    {
      ssize_t x, z;
      double y;
      int level;
      int width, height;
      float minX, maxX, minZ, maxZ;
      
      pos vertices[3][3];
      unsigned int childrenIndex[4];

      bool visible;
      bool leaf;
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
    bool setVisible(QTNode currentNode, Vector3f position, float range);

};
