#include "quadtree.h"

QuadTree::QuadTree()
{
}

QuadTree::QuadTree(const QuadTree &other)
{

}

QuadTree::~QuadTree()
{
    delete nodearray;
}

QuadTree::QuadTree(MatrixXf heightmap) {
    nodearray = (QTNode*) malloc( sizeof(QTNode) * (heightmap.rows() * heightmap.cols()) );
    for (ssize_t i = 0; i < heightmap.cols() - 1; i++) {
        for (ssize_t j = 0; j < heightmap.rows() - 1; j++) {
            QTNode test;
            test.x = i;
            test.z = j;
            test.y = heightmap(j, i);
            test.level = 0;

            nodearray[i + j] = test;
            size++;
        }
    }
}

MatrixXf QuadTree::getResult(int level) {
    MatrixXf mat;
    for (ssize_t i = 0; i < this->size; i++) {
        if (this->nodearray[i].level == level) {
            QTNode node = this->nodearray[i];
            mat(node.x, node.z) = node.y;
        }
    }

    return mat;
}