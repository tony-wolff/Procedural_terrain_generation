#include <vector>

class TerrainGeneration {
    private:
        unsigned int width;
        unsigned int height;

    public:
        struct pos {int x, z; };
        typedef struct
        {
            ssize_t x, z;
            double y;
            int level;
            int width, height;
            float minX, maxX, minZ, maxZ;
            
            pos vertices[3][3];
            unsigned int childrenIndex[4] = {};

            bool visible;
            bool leaf;
        } QTNode;
        
        virtual void compile(float minX, float maxX, float minZ, float maxZ, int _width, int _height) = 0;
        virtual std::vector<QTNode> getResult(int level) = 0;
};