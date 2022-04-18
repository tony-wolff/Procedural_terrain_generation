
#include "mesh.h"
#include "shader.h"
#include "maploader.h"
#include "quadtree.h"
#include <Eigen/Geometry>
#include <iostream>
#include <fstream>
#include <limits>
#include "camera.h"

using namespace Eigen;

bool Mesh::load(const std::string &filename)
{
  std::string ext = filename.substr(filename.size() - 3, 3);
  // if(ext=="png" || ext=="PNG")
  return loadPNG(filename);
  /*else if(ext=="off" || ext=="OFF")
    return loadOFF(filename);
  else if(ext=="obj" || ext=="OBJ")
    return loadOBJ(filename);*/

  std::cerr
      << "Mesh: extension \'" << ext << "\' not supported." << std::endl;
  return false;
}

void Mesh::computeNormals()
{
  /*// pass 1: set the normal to 0
  for(std::vector<Vertex>::iterator v_iter = _vertices.begin() ; v_iter!=_vertices.end() ; ++v_iter)
      v_iter->normal.setZero();

  // pass 2: compute face normals and accumulate
  for(std::size_t j=0; j<_faces.size(); ++j)
  {
      Vector3f v0 = _vertices[_faces[j][0]].position;
      Vector3f v1 = _vertices[_faces[j][1]].position;
      Vector3f v2 = _vertices[_faces[j][2]].position;

      Vector3f n = (v1-v0).cross(v2-v0).normalized();

      _vertices[_faces[j][0]].normal += n;
      _vertices[_faces[j][1]].normal += n;
      _vertices[_faces[j][2]].normal += n;
  }

  // pass 3: normalize
  for(std::vector<Vertex>::iterator v_iter = _vertices.begin() ; v_iter!=_vertices.end() ; ++v_iter)
      v_iter->normal.normalize();*/
}

void Mesh::init()
{

  glGenVertexArrays(1, &mVertexArrayId);
  glGenBuffers(1, &mVertexBufferId);
  glGenBuffers(1, &mIndexBufferId);

  updateVBO();

  mIsInitialized = true;
}

void Mesh::updateVBO()
{

  glBindVertexArray(mVertexArrayId);

  // activate the VBO:
  glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferId);
  // copy the data from host's RAM to GPU's video memory:
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * mVertices.size(), mVertices[0].position.data(), GL_STATIC_DRAW);
  // glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * initVertices.size(), initVertices[0].position.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferId);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Vector3i) * mFaces.size(), mFaces[0].data(), GL_STATIC_DRAW);
}

#include <ImageMagick-7/Magick++.h>

void Mesh::createFrustum(float m_fovY, float m_near, float m_far, int mVpWidth, int mVpHeight,
                         Vector3f mPosition, Vector3f mForward, Vector3f mUp, Vector3f mRight, const std::string &filename)
{

  // calculate generalized relative width and aspect ratio
  float normHalfWidth = tan(m_fovY);

  float aspectRatio = mVpWidth / mVpHeight;
  // float aspectRatio = 1;

  // calculate width and height for near and far plane
  float nearHW = normHalfWidth * m_near;
  float nearHH = nearHW / aspectRatio;
  float farHW = normHalfWidth * m_far * 0.5f;
  float farHH = farHW / aspectRatio;

  // calculate near and far plane centers
  Vector3f nCenter = mPosition + mForward * m_near;
  Vector3f fCenter = mPosition + mForward * m_far * 0.5f;

  // construct corners of the near plane in the culled objects world space
  Vector3f na = Vector3f(nCenter + mUp * nearHH - mRight * nearHW);
  Vector3f nb = Vector3f(nCenter + mUp * nearHH + mRight * nearHW);
  Vector3f nc = Vector3f(nCenter - mUp * nearHH - mRight * nearHW);
  Vector3f nd = Vector3f(nCenter - mUp * nearHH + mRight * nearHW);
  // construct corners of the far plane
  Vector3f fa = Vector3f(fCenter + mUp * farHH - mRight * farHW);
  Vector3f fb = Vector3f(fCenter + mUp * farHH + mRight * farHW);
  Vector3f fc = Vector3f(fCenter - mUp * farHH - mRight * farHW);
  Vector3f fd = Vector3f(fCenter - mUp * farHH + mRight * farHW);

  // winding in an outside perspective so the cross product creates normals pointing inward
  Plane nn = Plane(na, nb, nc);  // Near normal
  Plane nf = Plane(fb, fa, fd);  // Far normal
  Plane nl = Plane(fa, na, fc);  // Left normal
  Plane nr = Plane(nb, fb, nd);  // Right normal
  Plane nt = Plane(fa, fb, na);  // Top normal
  Plane nbo = Plane(nc, nb, fc); // Bottom normal

  m_Planes.clear();

  m_Planes.push_back(nn);  // Near
  m_Planes.push_back(nf);  // Far
  m_Planes.push_back(nl);  // Left
  m_Planes.push_back(nr);  // Right
  m_Planes.push_back(nt);  // Top
  m_Planes.push_back(nbo); // Bottom

  mVertices.clear();
  mFaces.clear();

  // si c'est dans le frustum on ajoute les sommets sinon on rend les sommets à false;

  unsigned int vertexNum = 0;
  unsigned int facesNum = 0;

  float heightmap_x = 16.0f;
  float heightmap_z = 16.0f;
  float heightmap_y = 1.25f;
  float heightmap_tex_x = 1.0f / 16.0f;
  float heightmap_tex_z = 1.0f / 16.0f;

  MapLoader *m_loader = new MapLoader();

  MatrixXf hmapMat;
  m_loader->ReadMap(filename, &hmapMat);

  // We build the QuadTree & get the first lod
  QuadTree *qt = new QuadTree(hmapMat);
  vector<double> qtMat = qt->getResult(0);

  // Getting the width and height
  raw_height = (hmapMat.rows());
  raw_width = (hmapMat.cols());
  vertexNum = raw_width * raw_height;
  facesNum = ((raw_width) * (raw_height)) * 2;
  mVertices.resize(vertexNum);
  mFaces.reserve(facesNum);

  std::cout << "#######################" << std::endl;
  std::cout << "Map Height : " << raw_height << std::endl;
  std::cout << "Map Width : " << raw_width << std::endl;
  std::cout << "vertexNum : " << vertexNum << std::endl;
  std::cout << "facesNum : " << facesNum << std::endl;

  // Acces image pixel by pixel
  for (unsigned int x = 0; x < raw_width; ++x)
  {
    for (unsigned int z = 0; z < raw_height; ++z)
    {
      unsigned int offset = x * raw_width + z;

      mVertices[offset] = Vertex(Vector3f(x * heightmap_x, (qtMat.at(offset) * 255) * heightmap_y, z * heightmap_z));
      mVertices[offset].texcoord = Vector2f(x * heightmap_tex_x, z * heightmap_tex_z);
    }
  }

  for (auto vertice : mVertices)
  {
    if ((nn.n).dot(vertice.position - na) < 0 ||
        (nf.n).dot(vertice.position - fa) < 0 ||
        (nl.n).dot(vertice.position - fa) < 0 ||
        (nr.n).dot(vertice.position - nb) < 0 ||
        (nt.n).dot(vertice.position - fa) < 0 ||
        (nbo.n).dot(vertice.position - nc) < 0)
    {
      vertice.visible = false;
    }
    else
    {
      vertice.visible = true;
      // mVertices.push_back(vertice);
    }
  }

  for (unsigned int x = 0; x < raw_width - 1; ++x)
  {
    for (unsigned int z = 0; z < raw_height - 1; ++z)
    {
      unsigned int a = x * raw_width + z;
      unsigned int b = (x + 1) * raw_width + z;
      unsigned int c = (x + 1) * raw_width + (z + 1);
      unsigned int d = x * raw_width + (z + 1);

      /*if (!mVertices[a].visible)
        break;
      if (!mVertices[b].visible)
        break;
      if (!mVertices[c].visible)
        break;
      if (!mVertices[d].visible)
        break;*/

      if (mVertices[a].visible && mVertices[b].visible && mVertices[c].visible && mVertices[d].visible)
      {
        mFaces.push_back(Vector3i(c, b, a));
        mFaces.push_back(Vector3i(a, d, c));
      }
    }
  }
  updateVBO();
}

Mesh::~Mesh()
{
  if (mIsInitialized)
  {
    glDeleteBuffers(1, &mVertexBufferId);
    glDeleteBuffers(1, &mIndexBufferId);
  }
}

void Mesh::draw(const Shader &shd)
{
  if (!mIsInitialized)
    init();

  // Activate the VBO of the current mesh:
  glBindVertexArray(mVertexArrayId);
  glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferId);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferId);

  // Specify vertex data

  // 1 - get id of the attribute "vtx_position" as declared as "in vec3 vtx_position" in the vertex shader
  int vertex_loc = shd.getAttribLocation("vtx_position");
  if (vertex_loc >= 0)
  {
    // 2 - tells OpenGL where to find the x, y, and z coefficients:
    glVertexAttribPointer(vertex_loc,     // id of the attribute
                          3,              // number of coefficients (here 3 for x, y, z)
                          GL_FLOAT,       // type of the coefficients (here float)
                          GL_FALSE,       // for fixed-point number types only
                          sizeof(Vertex), // number of bytes between the x coefficient of two vertices
                                          // (e.g. number of bytes between x_0 and x_1)
                          0);             // number of bytes to get x_0
    // 3 - activate this stream of vertex attribute
    glEnableVertexAttribArray(vertex_loc);
  }

  int normal_loc = shd.getAttribLocation("vtx_normal");
  if (normal_loc >= 0)
  {
    glVertexAttribPointer(normal_loc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)sizeof(Vector3f));
    glEnableVertexAttribArray(normal_loc);
  }

  int color_loc = shd.getAttribLocation("vtx_color");
  if (color_loc >= 0)
  {
    glVertexAttribPointer(color_loc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(2 * sizeof(Vector3f)));
    glEnableVertexAttribArray(color_loc);
  }

  int texcoord_loc = shd.getAttribLocation("vtx_texcoord");
  if (texcoord_loc >= 0)
  {
    glVertexAttribPointer(texcoord_loc, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(2 * sizeof(Vector3f) + sizeof(Vector4f)));
    glEnableVertexAttribArray(texcoord_loc);
  }

  // send the geometry
  glDrawElements(GL_TRIANGLES, 3 * mFaces.size(), GL_UNSIGNED_INT, 0);

  // at this point the mesh has been drawn and rasterized into the framebuffer!
  if (vertex_loc >= 0)
    glDisableVertexAttribArray(vertex_loc);
  if (normal_loc >= 0)
    glDisableVertexAttribArray(normal_loc);
  if (color_loc >= 0)
    glDisableVertexAttribArray(color_loc);
  if (texcoord_loc >= 0)
    glDisableVertexAttribArray(texcoord_loc);

  checkError();
}

unsigned int Mesh::getRaw_width()
{
  return raw_width;
}

unsigned int Mesh::getRaw_height()
{
  return raw_height;
}

//********************************************************************************
// Loaders...
//********************************************************************************

#include <ImageMagick-7/Magick++.h>
bool Mesh::loadPNG(const std::string &filename)
{

  unsigned int vertexNum = 0;
  unsigned int facesNum = 0;

  float heightmap_x = 16.0f;
  float heightmap_z = 16.0f;
  float heightmap_y = 1.25f;
  float heightmap_tex_x = 1.0f / 16.0f;
  float heightmap_tex_z = 1.0f / 16.0f;

  /* float heightmap_x = 160.0f;
  float heightmap_z = 160.0f;
  float heightmap_y = 12.5f;
  float heightmap_tex_x = 1.0f / 1.60f;
  float heightmap_tex_z = 1.0f / 1.60f;*/

  /*float heightmap_x = 1;
  float heightmap_z = 1;
  float heightmap_y = 1;
  float heightmap_tex_x = 1;
  float heightmap_tex_z = 1;*/

  MapLoader *m_loader = new MapLoader();

  MatrixXf hmapMat;
  m_loader->ReadMap(filename, &hmapMat);

  // We build the QuadTree & get the first lod
  QuadTree *qt = new QuadTree(hmapMat);

  vector<double> qtMat = qt->getResult(0);
  // Getting the width and height
  raw_height = (hmapMat.rows());
  raw_width = (hmapMat.cols());
  vertexNum = raw_width * raw_height;
  facesNum = ((raw_width) * (raw_height)) * 2;
  mVertices.resize(vertexNum);
  //  initVertices.clear();
  // initVertices.resize(vertexNum);
  mFaces.reserve(facesNum);

  std::cout << "Map Height : " << raw_height << std::endl;
  std::cout << "Map Width : " << raw_width << std::endl;
  std::cout << "vertexNum : " << vertexNum << std::endl;
  std::cout << "facesNum : " << facesNum << std::endl;

  // Acces image pixel by pixel
  for (unsigned int x = 0; x < raw_width; ++x)
  {
    for (unsigned int z = 0; z < raw_height; ++z)
    {
      unsigned int offset = x * raw_width + z;

      /*initVertices[offset] = Vertex(Vector3f(x * heightmap_x, (qtMat.at(offset) * 255) * heightmap_y, z * heightmap_z));
      initVertices[offset].texcoord = Vector2f(x * heightmap_tex_x, z * heightmap_tex_z);
      initVertices[offset].visible = true;*/

      mVertices[offset] = Vertex(Vector3f(x * heightmap_x, (qtMat.at(offset) * 255) * heightmap_y, z * heightmap_z));
      mVertices[offset].texcoord = Vector2f(x * heightmap_tex_x, z * heightmap_tex_z);
      mVertices[offset].visible = true;
    }
  }

  for (unsigned int x = 0; x < raw_width - 1; ++x)
  {
    for (unsigned int z = 0; z < raw_height - 1; ++z)
    {
      unsigned int a = x * raw_width + z;
      unsigned int b = (x + 1) * raw_width + z;
      unsigned int c = (x + 1) * raw_width + (z + 1);
      unsigned int d = x * raw_width + (z + 1);

      mFaces.push_back(Vector3i(c, b, a));
      mFaces.push_back(Vector3i(a, d, c));
    }
  }
  computeNormals();

  return true;
}
