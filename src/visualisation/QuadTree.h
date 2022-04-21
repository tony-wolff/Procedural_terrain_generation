#include <Eigen/Dense>
#include <vector>
#include "opengl.h"
#include "TerrainGeneration.h"

using namespace std;
using namespace Eigen;


class QuadTree : public TerrainGeneration
{
  private:
    unsigned int width;
    unsigned int height;
    int maxLevel;

    void createNode(QTNode parent);

  public:
    vector<QTNode> nodearray;

    void compile(float minX, float maxX, float minZ, float maxZ, int _width, int _height) {
      QTNode root;
      root.minX = minX;
      root.maxX = maxX;
      root.minZ = minZ;
      root.maxZ = maxZ;
      root.width = _width;
      root.height = _height;

      width = _width;
      height = _height;
      root.level = 0;

      createNode(root);
    }

    vector<QTNode> getResult(int level) {
      vector<QTNode> vect;

      for (unsigned long i = 0; i < nodearray.size(); i++)
      {
          if (nodearray[i].level <= level)
          {
              QTNode node = nodearray.at(i);
              vect.push_back(node);
          }
      }

      return vect;
    }
    
    QuadTree();
    ~QuadTree();
    bool setVisible(QTNode currentNode, Vector3f position, float range);
    int getMaxLevel();
};
