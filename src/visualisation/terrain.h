#include <Eigen/Core>
#include <Eigen/Dense>
#include <string>
#include <vector>

class Shader;

/** \class Terrain
 * A class to represent a 3D heightmap based terrain
 */
class Terrain
{
    typedef Eigen::Matrix<float, 2, 1, Eigen::DontAlign> Vector2f;
    typedef Eigen::Vector3f Vector3f;
    typedef Eigen::Matrix<float, 4, 1, Eigen::DontAlign> Vector4f;
    typedef Eigen::Vector3i Vector3i;

public:
    Terrain() {}

    /** Destructor */
    virtual ~Terrain();

    /** load a heightmap from an image file */
    int loadHeightmap(const std::string& filename);

    void computeNormals();

    /** initialize OpenGL's Vertex Buffer Array (must be called once before
    * calling draw()) */
    void initVBA();

    /** Send the mesh to OpenGL for drawing using shader \a shd */
    virtual void draw(const Shader &shd);

    /* TODO */
    float computeDistance(Vector3f camPos);
    void testVisibility(Vector3f planePos, Vector3f camPosition);
    void updateTerrain();

    /** Quadtree **/
    float distanceToPlane(Vector3f pos);
    void drawQTTerrain();
    unsigned int getRaw_width();
    unsigned int getRaw_height();

protected:
    /** Loads heightmap in png format */
    bool loadPNG(const std::string &filename);

    /** Represents a vertex of the mesh */
    struct Vertex
    {
        Vertex(const Vector3f &pos = Vector3f::Zero(),
            const Vector3f &n = Vector3f::Zero())
            : position(pos), normal(n), color(Vector4f(0.6, 0.6, 0.6, 1.0)),
            texcoord(Vector2f::Zero()) {}

        Vector3f position;
        Vector3f normal;
        Vector4f color;
        Vector2f texcoord;
        Vector3f tangent;
        Vector3f bitangent;
        bool visible;
  };

    /** The list of vertices */
    Eigen::MatrixXf heightmapMat;
    std::vector<Vertex> mVertices;

    /** The list of face indices */
    std::vector<Vector3i> mFaces;


    unsigned int mVertexArrayId;
    unsigned int mVertexBufferId; ///< the id of the BufferObject storing the
                                ///< vertex attributes
    unsigned int mIndexBufferId; ///< the id of the BufferObject storing the faces indices
    bool mIsInitialized;

    unsigned int raw_width = 0;
    unsigned int raw_height = 0; 
};
