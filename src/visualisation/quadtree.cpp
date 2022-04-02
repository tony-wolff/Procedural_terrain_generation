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

QuadTree::QuadTree(float minX, float maxX, float minZ, float maxZ, int width, int height) {
    // Notre première node englobe tout le terrain
    QTNode root;
    root.minX = minX;
    root.maxX = maxX;
    root.minZ = minZ;
    root.maxZ = maxZ;
    root.width = width;
    root.height = height;

    root.level = 0;

    createNode(root);
}

void QuadTree::createNode(QTNode parent) {
    // A définir
    float threshold = 0.0f;

    // Une fois le cap de triangle atteinds (8 pour une image de 512)
    // On créer les vertex dans les feuilles 
    // Dedans on enregistre les positions (On récuperera la valeur de y plus tard ?)
    if ((parent.minZ - parent.maxZ) == threshold) {

        // On calcule les vertex des triangle
        // Il faudra aussi récupérer la scale du terrain
        // C'est dans le main (ou mesh.cpp) qu'on construira les vertex3F avec la valeur de hauteur 
        for (int x = parent.minX; x < parent.width; x++) {
            for (int z = parent.minZ; z < parent.height; z++) {
                parent.vertices.push_back(Vector2f(x, z));
            }
        }
    }
    // Sinon on continue récursivement à créer les enfants
    // J'en créer qu'un ici mais normalement on en créer 4
    else {
        QTNode childNode;
        
        // Paramètres valable pour tout les fils
        childNode.width = parent.width / 2;
        childNode.height = parent.height / 2;
        childNode.level = parent.level + 1;
        /* 
        * Fils en bas à gauche
        */

        childNode.minX = parent.minX;
        childNode.maxX = parent.minX + childNode.width;
        childNode.minZ = parent.minZ;
        childNode.maxZ = parent.minZ + childNode.height;

        nodeIndex++;
        // On sauvegarde l'index du children dans le tableau du node current
        nodearray.push_back(childNode);
        parent.childrenIndex[0] = nodeIndex;

        createNode(childNode);

        /* Redondance de code mais on voudra surement faire des traitements spécifique pour chaque fils */
        nodeIndex++;
        /*
        * Fils en bas à droite
        */
        childNode.minX = parent.minX + childNode.width;
        childNode.maxX = parent.width;
        childNode.minZ = parent.minZ;
        childNode.maxZ = parent.minZ + childNode.height;

        nodearray.push_back(childNode);
        parent.childrenIndex[1] = nodeIndex;
        createNode(childNode);
        
        nodeIndex++;
        /*
        * Fils en haut à gauche
        */
        childNode.minX = parent.minX;
        childNode.maxX = parent.minX + childNode.width;
        childNode.minZ = parent.minZ + childNode.height;
        childNode.maxZ = parent.height;

        nodearray.push_back(childNode);
        parent.childrenIndex[2] = nodeIndex;
        createNode(childNode);

        nodeIndex++;
        /*
        * Fils en haut à droite
        */
        childNode.minX = parent.minX + childNode.width;
        childNode.maxX = parent.width;
        childNode.minZ = parent.minZ + childNode.height;
        childNode.maxZ = parent.height;
        nodearray.push_back(childNode);
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
