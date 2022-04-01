#include <Eigen/Dense>
#include <vector>

using namespace std;
using namespace Eigen;

class QuadTree
{
  private:
    struct QTNode {
      ssize_t x, z;
      double y;
      int level;
    };

    vector<QTNode> nodearray;
    unsigned int width;
    unsigned int height;

  public:
    QuadTree();
    QuadTree(MatrixXf heightmap);
    QuadTree(const QuadTree&);
    ~QuadTree();
    vector<double> getResult(int level);

};