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

QuadTree::QuadTree(MatrixXf heightmap)
{
    width = heightmap.cols();
    height = heightmap.rows();

    for (ssize_t i = 0; i < height; i++)
    {
        for (ssize_t j = 0; j < width; j++)
        {
            QTNode test;
            test.x = i;
            test.z = j;
            test.y = heightmap(i, j);
            test.level = 0;
            nodearray.push_back(test);
        }
    }
}

QuadTree::QuadTree(float minX, float maxX, float minZ, float maxZ, int _width, int _height)
{
    // Notre premier node englobe tout le terrain
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

void QuadTree::createNode(QTNode currentNode)
{
    // A définir
    maxLevel = 8;
    float threshold = width * pow(0.5, maxLevel);
    // Une fois le cap de triangle atteint
    if ((currentNode.height) == threshold) 
    {
        currentNode.leaf = true;
        return;
    }
    // Sinon on continue récursivement à créer les enfants
    // J'en créer qu'un ici mais normalement on en créer 4
    else
    {
        int divs = (width / pow(2, currentNode.level));

        for (int x = 0; x < 3; x ++)
        {
            for (int z = 0; z < 3; z ++)
            {
                int posX = currentNode.minX + (divs * x);
                int posZ = currentNode.minZ + (divs * z);
                if (posX > 0)
                    posX--;
                if (posZ > 0)
                    posZ--;
                if (posX >= width)
                    posX = width - 1;
                if (posZ >= height)
                    posZ = height - 1;

                currentNode.vertices[x][z].x = posX;
                currentNode.vertices[x][z].z = posZ;
            }
        }
        nodearray.push_back(currentNode);

        QTNode childNode;

        childNode.width = currentNode.width / 2;
        childNode.height = currentNode.height / 2;

//-----------------------------------------------------
//             New Children properties

        /*
        * Fils en haut à gauche
        */
        childNode.minX = currentNode.minX;
        childNode.minZ = currentNode.minZ;
        childNode.maxX = currentNode.minX + childNode.width;
        childNode.maxZ = currentNode.minZ + childNode.height;

        childNode.level = currentNode.level + 1;

        nodeIndex++;
        currentNode.childrenIndex[0] = nodeIndex;
        createNode(childNode);

        /*
        * Fils en haut à droite
        */
        childNode.minX = currentNode.minX + childNode.width;
        childNode.minZ = currentNode.minZ;
        childNode.maxX = currentNode.minX + currentNode.width;
        childNode.maxZ = currentNode.minZ + childNode.height;

        childNode.level = currentNode.level + 1;

        nodeIndex++;
        currentNode.childrenIndex[1] = nodeIndex;
        createNode(childNode);

        /* 
        * Fils en bas à gauche
        */
        childNode.minX = currentNode.minX;
        childNode.minZ = currentNode.minZ + childNode.height;
        childNode.maxX = currentNode.minX + childNode.width;
        childNode.maxZ = currentNode.minZ + currentNode.height;

        childNode.level = currentNode.level + 1;

        nodeIndex++;
        currentNode.childrenIndex[2] = nodeIndex;
        createNode(childNode);

        /*
        * Fils en bas à droite
        */
        childNode.minX = currentNode.minX + childNode.width;
        childNode.minZ = currentNode.minZ + childNode.height;
        childNode.maxX = currentNode.minX + currentNode.width;
        childNode.maxZ = currentNode.minZ + currentNode.height;

        childNode.level = currentNode.level + 1;

        nodeIndex++;
        currentNode.childrenIndex[3] = nodeIndex;
        createNode(childNode);
    }
}

vector<double> QuadTree::getResult(int level)
{
    vector<double> vect;

    for (ssize_t i = 0; i < nodearray.size(); i++)
    {
        if (nodearray[i].level == level)
        {
            QTNode node = nodearray.at(i);
            vect.push_back(node.y);
        }
    }

    return vect;
}


float distance(int x, int z, int x1, int z1) {
    return sqrt(pow((x - x1), 2) + pow((z - z1), 2));
}

bool QuadTree::setVisible(QTNode currentNode, Vector3f position, float range) {
    // Stopping condition
    if (currentNode.leaf) {
        return false;
    }
    // std::cout << currentNode.childrenIndex[0] << std::endl;
    // std::cout << currentNode.childrenIndex[1] << std::endl;
    // std::cout << currentNode.childrenIndex[2] << std::endl;
    // std::cout << currentNode.childrenIndex[3] << std::endl;
    float topLeft = distance(currentNode.minX, currentNode.maxZ, position.x(), position.z());
    float topRight = distance(currentNode.maxX, currentNode.maxZ, position.x(), position.z());
    float bottomLeft = distance(currentNode.minX, currentNode.minZ, position.x(), position.z());
    float bottomRight = distance(currentNode.maxX, currentNode.minZ, position.x(), position.z());

    if (topLeft < range || topRight < range || bottomLeft < range || bottomRight < range) {
        currentNode.visible = true;

        // Recurse over all childs && set the current to false if they're all visible
        if (setVisible(nodearray.at(currentNode.childrenIndex[0]), position, range) 
            && setVisible(nodearray.at(currentNode.childrenIndex[1]), position, range)
            && setVisible(nodearray.at(currentNode.childrenIndex[2]), position, range)
            && setVisible(nodearray.at(currentNode.childrenIndex[3]), position, range))
             {
                currentNode.visible = false;
             }
        return true;
    }

    return false;
}

int QuadTree::getMaxLevel()
{
    return this->maxLevel;
}
