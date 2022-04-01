#include <Eigen/Dense>

using namespace Eigen;

class QuadTree
{
  private:
    struct QTNode {
      ssize_t x, z;
      double y;
      int level;
    };

    QTNode* nodearray;
    unsigned int size;
  public:
    QuadTree();
    QuadTree(MatrixXf heightmap);
    QuadTree(const QuadTree&);
    ~QuadTree();
    MatrixXf getResult(int level);

};