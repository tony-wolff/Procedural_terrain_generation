// Copyright (C) 2008-2011 Gael Guennebaud <gael.guennebaud@inria.fr>

#ifndef _Mesh_h_
#define _Mesh_h_

#include <Eigen/Core>
#include <string>
#include <vector>

class Shader;

/** \class Mesh
 * A class to represent a 3D triangular mesh
 */

class Mesh
{
  typedef Eigen::Matrix<float, 2, 1, Eigen::DontAlign> Vector2f;
  typedef Eigen::Vector3f Vector3f;
  typedef Eigen::Matrix<float, 4, 1, Eigen::DontAlign> Vector4f;
  typedef Eigen::Vector3i Vector3i;
  typedef Eigen::Vector4i Vector4i;

  struct Plane
  {
    Plane(Vector3f a, Vector3f b, Vector3f c)
    {
      d = a;
      n = ((-b + a).cross(c - a)).normalized();
    }
    Vector3f n;
    Vector3f d;
  };

public:
  Mesh() {}

  /** Destructor */
  virtual ~Mesh();

  /** load a triangular mesh from the file \a filename (.off or .obj) */
  bool load(const std::string &filename);

  void computeNormals();

  /** initialize OpenGL's Vertex Buffer Array (must be called once before calling draw()) */
  void init();

  // Copy vertex attributes from the CPU to GPU memory (needs to be called after editing any vertex attributes: positions, normals, texcoords, masks, etc.)
  void updateVBO();

  void Rmem(float m_fovY, float m_near, float m_far, int mVpWidth, int mVpHeight,
            Vector3f mPosition, Vector3f mForward, Vector3f mUp, Vector3f mRight);
  /*void createFrustum(float m_fovY, float m_near, float m_far, int mVpWidth, int mVpHeight,
                     Vector3f mPosition, Vector3f mForward, Vector3f mUp, Vector3f mRight);*/

  /** Send the mesh to OpenGL for drawing using shader \a shd */
  virtual void draw(const Shader &shd);

  void UpdateMesh();

  /** Quadtree **/

  void drawQTTerrain();
  unsigned int getRaw_width();
  unsigned int getRaw_height();

  // Axis Aligned Box structy
  struct AAB
  {
    // Points of the AAB
    Vector3f maxPoint;
    Vector3f minPoint;

    const Vector3f getPositiveVertex(Vector3f normal) const
    {
      Vector3f positive = minPoint;
      if (normal.x() >= 0)
        positive.x() = maxPoint.x();
      if (normal.y() >= 0)
        positive.y() = maxPoint.y();
      if (normal.z() >= 0)
        positive.z() = maxPoint.z();

      return positive;
    }

    const Vector3f getNegativeVertex(Vector3f normal) const
    {
      Vector3f negative = maxPoint;
      if (normal.x() >= 0)
        negative.x() = minPoint.x();
      if (normal.y() >= 0)
        negative.y() = minPoint.y();
      if (normal.z() >= 0)
        negative.z() = minPoint.z();

      return negative;
    }
  };
  // bool inFrustum(Vector3f p);

protected:
  /** Loads a triangular mesh in the OFF format */
  bool loadPNG(const std::string &filename);
  bool loadOFF(const std::string &filename);
  bool loadOBJ(const std::string &filename);
  float distance(const Vector4f &plane, const Vector3f &point) const;

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
  std::vector<Vertex> mVertices;

  /** The list of face indices */
  std::vector<Vector3i> mFaces;

  /// Pour tester
  std::vector<Vertex> initVertices;
  // std::vector<Vector3i> initFaces;
  std::vector<Plane> m_Planes;

  enum ePlanes
  {
    LEFT,
    RIGHT,
    TOP,
    BOTTOM,
    FAR,
    NEAR
  };

  unsigned int mVertexArrayId;
  unsigned int mVertexBufferId; ///< the id of the BufferObject storing the
                                ///< vertex attributes
  unsigned int
      mIndexBufferId; ///< the id of the BufferObject storing the faces indices
  bool mIsInitialized;

  unsigned int raw_width = 0;
  unsigned int raw_height = 0;
};

#endif
