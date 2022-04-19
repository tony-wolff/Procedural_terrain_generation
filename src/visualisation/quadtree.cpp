#include "quadtree.h"
#include "math.h"
#include <iostream>
#include <fstream>
#include <limits>

QuadTree::QuadTree()
{
}

QuadTree::~QuadTree()
{
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
                unsigned int posX = currentNode.minX + (divs * x);
                unsigned int posZ = currentNode.minZ + (divs * z);
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
