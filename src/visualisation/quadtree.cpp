#include "quadtree.h"
#include "math.h"
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

QuadTree::QuadTree(float minX, float maxX, float minZ, float maxZ, int _width, int _height) {
    // Notre première node englobe tout le terrain
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

void QuadTree::createNode(QTNode parent) {
    // A définir
    float threshold = 0.0f;

    // Une fois le cap de triangle atteind
    if ((parent.minZ - parent.maxZ) == threshold) {
        return;    
    }
    // Sinon on continue récursivement à créer les enfants
    // J'en créer qu'un ici mais normalement on en créer 4
    else {
        QTNode currentNode;
        
        // Paramètres valable pour tout les fils
        currentNode.width = parent.minZ - parent.maxZ;
        currentNode.height = parent.minX - parent.maxX;

        nodeIndex++;
        nodearray.push_back(currentNode);

        int divs = (width / pow(2, parent.level));

        for (int x = parent.minX; x < parent.maxX; x += parent.maxX / divs)
            for (int z = parent.minZ; z < parent.maxZ; z += parent.maxZ / divs)
                parent.vertices.push_back(Vector2f(x, z));

        QTNode childNode;

        childNode.width = currentNode.width / 2;
        childNode.height = currentNode.height / 2;

        /* 
        * Fils en bas à gauche
        */
        childNode.minX = parent.minX;
        childNode.maxX = parent.minX + childNode.width;
        childNode.minZ = parent.minZ;
        childNode.maxZ = parent.minZ + childNode.height;
        childNode.level = parent.level + 1;

        
        parent.childrenIndex[0] = nodeIndex;
        createNode(childNode);

        /*
        * Fils en bas à droite
        */
        childNode.minX = parent.minX + childNode.width;
        childNode.maxX = parent.width;
        childNode.minZ = parent.minZ;
        childNode.maxZ = parent.minZ + childNode.height;
        childNode.level = parent.level + 1;

        parent.childrenIndex[1] = nodeIndex;
        createNode(childNode);
        /*
        * Fils en haut à gauche
        */
        childNode.minX = parent.minX;
        childNode.maxX = parent.minX + childNode.width;
        childNode.minZ = parent.minZ + childNode.height;
        childNode.maxZ = parent.height;
        childNode.level = parent.level + 1;

        parent.childrenIndex[2] = nodeIndex;
        createNode(childNode);

        /*
        * Fils en haut à droite
        */
        childNode.minX = parent.minX + childNode.width;
        childNode.maxX = parent.width;
        childNode.minZ = parent.minZ + childNode.height;
        childNode.maxZ = parent.height;
        childNode.level = parent.level + 1;

        parent.childrenIndex[3] = nodeIndex;
        createNode(childNode);
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
