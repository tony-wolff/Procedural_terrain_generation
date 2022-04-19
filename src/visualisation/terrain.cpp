
#include "terrain.h"
#include "shader.h"
#include "maploader.h"
#include <Eigen/Geometry>
#include <iostream>
#include <fstream>
#include <limits>

using namespace Eigen;

int Terrain::loadHeightmap(const std::string& filename)
{
    MapLoader loader;
    if (loader.ReadMapMatrix(filename, &heightmapMat) == -1)
    {
        return -1;
    }
    unsigned int vertexNum = 0;
    unsigned int facesNum = 0;

    float heightmap_x = 1;
    float heightmap_z = 1;
    float heightmap_y = 1;
    float heightmap_tex_x = 1;
    float heightmap_tex_z = 1;

    // We build the QuadTree & get the first lod
    QuadTree* qt = new QuadTree(heightmapMat);
    
    vector<double> qtMat = qt->getResult(0);

    // Getting the width and height 
    raw_height = (heightmapMat.rows());
    raw_width = (heightmapMat.cols());
    vertexNum = raw_width * raw_height;
    facesNum = ((raw_width) * (raw_height)) * 2;
    mVertices.resize(vertexNum);
    mFaces.reserve(facesNum);

    std::cout << "Map Height : " << raw_height << std::endl;
    std::cout << "Map Width : " << raw_width << std::endl;
    std::cout << "vertexNum : " << vertexNum << std::endl;
    std::cout << "facesNum : " << facesNum << std::endl;

    qtTest = new QuadTree(0, 512, 0, 512, 512, 512);

    std::cout << "------- QuadTree build -------" << std::endl;
    std::cout << qtTest->nodearray.size() << " node created" << std::endl;

    int vx, vz;
    int indexVertices = 0;
    mVertices.resize(qtTest->nodearray.size() * 8);

    for (int i = 0; i < qtTest->nodearray.size(); i++)
    {
      if (qtTest->nodearray.at(i).level <= qtTest->getMaxLevel()-2)
      {
        //std::cout << "Begin node draw" << std::endl;
        for (int x = 0; x < 2; x++)
        {
          for (int z = 0; z < 2; z++)
          {
            /*std::cout << "------ -------" << std::endl;
            std::cout << "height : " << heightmapMat.rows() << " width " << heightmapMat.cols() << std::endl;*/

            vx = qtTest->nodearray.at(i).vertices[x][z].x;
            vz = qtTest->nodearray.at(i).vertices[x][z].z;
            
            //std::cout << vx << " -- " << vz << endl;
            mVertices[indexVertices] = Vertex(Vector3f(vx* heightmap_x, (heightmapMat(vx, vz) * 255) * heightmap_y, vz* heightmap_z));
            mVertices[indexVertices].texcoord = Vector2f(vx* heightmap_tex_x, vz* heightmap_tex_z);
            mVertices[indexVertices].visible = true;

            indexVertices++;
            vx = qtTest->nodearray.at(i).vertices[x][z+1].x ;
            vz = qtTest->nodearray.at(i).vertices[x][z+1].z;
            
            //std::cout << vx << " -- " << vz << endl;
            mVertices[indexVertices] = Vertex(Vector3f(vx* heightmap_x, (heightmapMat(vx, vz) * 255) * heightmap_y, vz* heightmap_z));
            mVertices[indexVertices].texcoord = Vector2f(vx* heightmap_tex_x, vz* heightmap_tex_z);
            mVertices[indexVertices].visible = true;

            indexVertices++;
            vx = qtTest->nodearray.at(i).vertices[x+1][z].x;
            vz = qtTest->nodearray.at(i).vertices[x+1][z].z;
            
            //std::cout << vx << " -- " << vz << endl;
            mVertices[indexVertices] = Vertex(Vector3f(vx* heightmap_x, (heightmapMat(vx, vz) * 255) * heightmap_y, vz* heightmap_z));
            mVertices[indexVertices].texcoord = Vector2f(vx* heightmap_tex_x, vz* heightmap_tex_z);
            mVertices[indexVertices].visible = true;

            indexVertices++;
            vx = qtTest->nodearray.at(i).vertices[x+1][z+1].x;
            vz = qtTest->nodearray.at(i).vertices[x+1][z+1].z;
            
            //std::cout << vx << " -- " << vz << endl;
            mVertices[indexVertices] = Vertex(Vector3f(vx* heightmap_x, (heightmapMat(vx, vz) * 255) * heightmap_y, vz* heightmap_z));
            mVertices[indexVertices].texcoord = Vector2f(vx* heightmap_tex_x, vz* heightmap_tex_z);
            mVertices[indexVertices].visible = true;
            indexVertices++;
          }
        }
      }
    }

    for(unsigned int i = 0; i < raw_width - 1; ++i)
    {
        for(unsigned int j = 0; j < raw_height - 1; ++j)
        {
            unsigned int offset = (i * raw_width) + j;

            if(mVertices[offset].visible == true)
            {
                unsigned int a = i * raw_width + j;
                unsigned int b = (i+1) * raw_width + j;
                unsigned int c = (i+1) * raw_width + (j+1);
                unsigned int d = i * raw_width + (j+1);

                mFaces.push_back(Vector3i(c, b, a));
                mFaces.push_back(Vector3i(a, d, c));
            }
        }
    }

    computeNormals();
  
  return true;
}

void Terrain::computeNormals()
{
    // pass 1: set the normal to 0
    for(std::vector<Vertex>::iterator v_iter = mVertices.begin() ; v_iter != mVertices.end() ; ++v_iter)
        v_iter->normal.setZero();

    // pass 2: compute face normals and accumulate
    for (std::size_t j = 0; j < mFaces.size(); ++j)
    {
        Vector3f v0 = mVertices[mFaces[j][0]].position;
        Vector3f v1 = mVertices[mFaces[j][1]].position;
        Vector3f v2 = mVertices[mFaces[j][2]].position;

        Vector3f n = (v1-v0).cross(v2-v0).normalized();

        mVertices[mFaces[j][0]].normal += n;
        mVertices[mFaces[j][1]].normal += n;
        mVertices[mFaces[j][2]].normal += n;
    }

    // pass 3: normalize
    for(std::vector<Vertex>::iterator v_iter = mVertices.begin() ; v_iter != mVertices.end() ; ++v_iter)
        v_iter->normal.normalize();
}

void Terrain::initVBA()
{
  // create the BufferObjects and copy the related data into them.

  // create a VBO identified by a unique index:
  glGenBuffers(1,&mVertexBufferId);
  // activate the VBO:
  glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferId);
  // copy the data from host's RAM to GPU's video memory:
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*mVertices.size(), mVertices[0].position.data(), GL_STATIC_DRAW);
  
  glGenBuffers(1,&mIndexBufferId);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferId);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Vector3i)*mFaces.size(), mFaces[0].data(), GL_STATIC_DRAW);
  
  glGenVertexArrays(1,&mVertexArrayId);
  
  mIsInitialized = true;
}

Terrain::~Terrain()
{
  if(mIsInitialized)
  {
    glDeleteBuffers(1,&mVertexBufferId);
    glDeleteBuffers(1,&mIndexBufferId);
  }
}


void Terrain::draw(const Shader &shd)
{
  // Activate the VBO of the current mesh:
  glBindVertexArray(mVertexArrayId);
  glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferId);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferId);
  
  // Specify vertex data

  // 1 - get id of the attribute "vtx_position" as declared as "in vec3 vtx_position" in the vertex shader
  int vertex_loc = shd.getAttribLocation("vtx_position");
  if(vertex_loc>=0)
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
  if(normal_loc>=0)
  {
    glVertexAttribPointer(normal_loc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(Vector3f));
    glEnableVertexAttribArray(normal_loc);
  }

  int color_loc = shd.getAttribLocation("vtx_color");
  if(color_loc>=0)
  {
    glVertexAttribPointer(color_loc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2*sizeof(Vector3f)));
    glEnableVertexAttribArray(color_loc);
  }

  int texcoord_loc = shd.getAttribLocation("vtx_texcoord");
  if(texcoord_loc>=0)
  {
    glVertexAttribPointer(texcoord_loc, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2*sizeof(Vector3f)+sizeof(Vector4f)));
    glEnableVertexAttribArray(texcoord_loc);
  }

  // send the geometry
  glDrawElements(GL_TRIANGLES, 3*mFaces.size(), GL_UNSIGNED_INT, 0);

  // at this point the mesh has been drawn and rasterized into the framebuffer!
  if(vertex_loc>=0) glDisableVertexAttribArray(vertex_loc);
  if(normal_loc>=0) glDisableVertexAttribArray(normal_loc);
  if(color_loc>=0)  glDisableVertexAttribArray(color_loc);
  if(texcoord_loc>=0)  glDisableVertexAttribArray(texcoord_loc);

  checkError();
}

unsigned int Terrain::getRaw_width()
{
  return raw_width;
}

unsigned int Terrain::getRaw_height()
{
  return raw_height;
}


void Terrain::updateTerrain(Vector3f cam_pos, float range)
{
  int vx, vz;
  unsigned int vertexNum = 0;
  unsigned int facesNum = 0;

  float heightmap_x = 1;
  float heightmap_z = 1;
  float heightmap_y = 1;
  float heightmap_tex_x = 1;
  float heightmap_tex_z = 1;
  int indexVertices = 0;
  raw_width = (heightmapMat.cols());
  vertexNum = raw_width * raw_height;
  facesNum = ((raw_width) * (raw_height)) * 2;
  mFaces.reserve(facesNum);

  for (int i = 0; i < qtTest->nodearray.size(); i++)
  {
    if (qtTest->nodearray.at(i).level <= qtTest->getMaxLevel()-3 && qtTest->setVisible(qtTest->nodearray.at(i), cam_pos, range))
    {
      std::cout << "Begin node draw" << std::endl;
      for (int x = 0; x < 2; x++)
      {
        for (int z = 0; z < 2; z++)
        {

          vx = qtTest->nodearray.at(i).vertices[x][z].x;
          vz = qtTest->nodearray.at(i).vertices[x][z].z;
          
          mVertices[indexVertices] = Vertex(Vector3f(vx* heightmap_x, (heightmapMat(vx, vz) * 255) * heightmap_y, vz* heightmap_z));
          mVertices[indexVertices].texcoord = Vector2f(vx* heightmap_tex_x, vz* heightmap_tex_z);
          mVertices[indexVertices].visible = true;

          indexVertices++;
          vx = qtTest->nodearray.at(i).vertices[x][z+1].x ;
          vz = qtTest->nodearray.at(i).vertices[x][z+1].z;
          
          mVertices[indexVertices] = Vertex(Vector3f(vx* heightmap_x, (heightmapMat(vx, vz) * 255) * heightmap_y, vz* heightmap_z));
          mVertices[indexVertices].texcoord = Vector2f(vx* heightmap_tex_x, vz* heightmap_tex_z);
          mVertices[indexVertices].visible = true;

          indexVertices++;
          vx = qtTest->nodearray.at(i).vertices[x+1][z].x;
          vz = qtTest->nodearray.at(i).vertices[x+1][z].z;
          
          mVertices[indexVertices] = Vertex(Vector3f(vx* heightmap_x, (heightmapMat(vx, vz) * 255) * heightmap_y, vz* heightmap_z));
          mVertices[indexVertices].texcoord = Vector2f(vx* heightmap_tex_x, vz* heightmap_tex_z);
          mVertices[indexVertices].visible = true;

          indexVertices++;
          vx = qtTest->nodearray.at(i).vertices[x+1][z+1].x;
          vz = qtTest->nodearray.at(i).vertices[x+1][z+1].z;
          
          mVertices[indexVertices] = Vertex(Vector3f(vx* heightmap_x, (heightmapMat(vx, vz) * 255) * heightmap_y, vz* heightmap_z));
          mVertices[indexVertices].texcoord = Vector2f(vx* heightmap_tex_x, vz* heightmap_tex_z);
          mVertices[indexVertices].visible = true;
          indexVertices++;
        }
      }
    }
  }
  for(unsigned int i = 0; i < raw_width - 1; ++i)
  {
      for(unsigned int j = 0; j < raw_height - 1; ++j)
      {
          unsigned int offset = (i * raw_width) + j;

          if(mVertices[offset].visible == true)
          {
              unsigned int a = i * raw_width + j;
              unsigned int b = (i+1) * raw_width + j;
              unsigned int c = (i+1) * raw_width + (j+1);
              unsigned int d = i * raw_width + (j+1);

              mFaces.push_back(Vector3i(c, b, a));
              mFaces.push_back(Vector3i(a, d, c));
          }
      }
  }
  computeNormals();
}