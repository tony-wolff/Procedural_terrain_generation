#include "quadtree.h"
#include <iostream>
#include <fstream>
#include <limits>

QuadTree::QuadTree()
{
}

QuadTree::QuadTree(const QuadTree &other)
{

}

QuadTree::~QuadTree()
{
}

QuadTree::QuadTree(MatrixXf heightmap) {
    width = heightmap.cols();
    height = heightmap.rows();

    for (ssize_t i = 0; i < (height - 1); i++) {
        for (ssize_t j = 0; j < (width - 1); j++) {
            QTNode test;
            test.x = i;
            test.z = j;
            test.y = heightmap(i, j);
            test.level = 0;
            nodearray.push_back(test);
        }
    }
}

vector<double> QuadTree::getResult(int level) {
    vector<double> vect;

    for (ssize_t i = 0; i < nodearray.size(); i++) {
            if (nodearray[i].level == level) {
                QTNode node = nodearray.at(i);
                vect.push_back(node.y);
            }
    }
    

    return vect;
}