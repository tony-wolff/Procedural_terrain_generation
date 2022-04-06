
#include "mesh.h"
#include "shader.h"
#include "maploader.h"
#include "quadtree.h"
#include <Eigen/Geometry>
#include <iostream>
#include <fstream>
#include <limits>

using namespace Eigen;

bool Mesh::load(const std::string& filename)
{
  std::string ext = filename.substr(filename.size()-3,3);
  //if(ext=="png" || ext=="PNG")
    return loadPNG(filename);
  /*else if(ext=="off" || ext=="OFF")
    return loadOFF(filename);
  else if(ext=="obj" || ext=="OBJ")
    return loadOBJ(filename);*/

  std::cerr << "Mesh: extension \'" << ext << "\' not supported." << std::endl;
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

  glGenVertexArrays(1,&mVertexArrayId);
  glGenBuffers(1,&mVertexBufferId);
  glGenBuffers(1,&mIndexBufferId);

  Rmem();

  updateVBO();

  mIsInitialized = true;
}

void Mesh::updateVBO()
{
  Rmem();
  
  glBindVertexArray(mVertexArrayId);

  // activate the VBO:
  glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferId);
  // copy the data from host's RAM to GPU's video memory:
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*mVertices.size(), mVertices[0].position.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferId);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Vector3i)*mFaces.size(), mFaces[0].data(), GL_STATIC_DRAW);


}

void Mesh::Rmem() {
  long unsigned int f1 = initVertices.size();
  if(!mIsInitialized) {
    for(long unsigned int i = 0; i < f1; i++) {
      mVertices.push_back(initVertices[i]);
    }
  }
  else {
    mVertices.clear();
    mFaces.clear();
    for(long unsigned int i = 0; i < f1; i++) {
      if(i < f1/2)
        mVertices.push_back(initVertices[i]);
      else
        initVertices[i].visible = false;
    }

    for(unsigned int x = 0; x < raw_width-1; ++x)
    {
        for(unsigned int z = 0; z < raw_height-1; ++z)
        {
                unsigned int a = x * raw_width + z;
                unsigned int b = (x+1) * raw_width + z;
                unsigned int c = (x+1) * raw_width + (z+1);
                unsigned int d = x * raw_width + (z+1);

                if(!initVertices[a].visible) break;
                if(!initVertices[b].visible) break;
                if(!initVertices[c].visible) break;
                mFaces.push_back(Vector3i(c, b, a));
                if(!initVertices[d].visible) break;
                mFaces.push_back(Vector3i(a, d, c));
        }
    }
  }
  
}

Mesh::~Mesh()
{
  if(mIsInitialized)
  {
    glDeleteBuffers(1,&mVertexBufferId);
    glDeleteBuffers(1,&mIndexBufferId);
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

unsigned int Mesh::getRaw_width() {
  return raw_width;
}

unsigned int Mesh::getRaw_height() {
  return raw_height;
}





//********************************************************************************
// Loaders...
//********************************************************************************

#include <ImageMagick-7/Magick++.h>
bool Mesh::loadPNG(const std::string& filename)
{

    unsigned int vertexNum = 0;
    unsigned int facesNum = 0;

    /*float heightmap_x = 16.0f;
    float heightmap_z = 16.0f;
    float heightmap_y = 1.25f;
    float heightmap_tex_x = 1.0f / 16.0f;
    float heightmap_tex_z = 1.0f / 16.0f;*/

    /* float heightmap_x = 160.0f;
    float heightmap_z = 160.0f;
    float heightmap_y = 12.5f;
    float heightmap_tex_x = 1.0f / 1.60f;
    float heightmap_tex_z = 1.0f / 1.60f;*/

    float heightmap_x = 1;
    float heightmap_z = 1;
    float heightmap_y = 1;
    float heightmap_tex_x = 1;
    float heightmap_tex_z = 1;

    MapLoader* m_loader = new MapLoader();

    MatrixXf hmapMat;
    m_loader->ReadMap(filename, &hmapMat);

    // We build the QuadTree & get the first lod
    QuadTree* qt = new QuadTree(hmapMat);
    
    vector<double> qtMat = qt->getResult(0);
    // Getting the width and height 
    raw_height = (hmapMat.rows());
    raw_width = (hmapMat.cols());
    vertexNum = raw_width * raw_height;
    facesNum = ((raw_width) * (raw_height)) * 2;
    //mVertices.resize(vertexNum);
    initVertices.resize(vertexNum);
    mFaces.reserve(facesNum);

    std::cout << "Map Height : " << raw_height << std::endl;
    std::cout << "Map Width : " << raw_width << std::endl;
    std::cout << "vertexNum : " << vertexNum << std::endl;
    std::cout << "facesNum : " << facesNum << std::endl;

    // Acces image pixel by pixel
    for(unsigned int x = 0; x < raw_width; ++x)
    {
        for(unsigned int z = 0; z < raw_height; ++z)
        {
            unsigned int offset = x * raw_width + z;

            initVertices[offset] = Vertex(Vector3f(x * heightmap_x, (qtMat.at(offset) * 255) * heightmap_y, z * heightmap_z));
            initVertices[offset].texcoord = Vector2f(x * heightmap_tex_x, z * heightmap_tex_z);
            initVertices[offset].visible = true;
        }
    }

    for(unsigned int x = 0; x < raw_width-1; ++x)
    {
        for(unsigned int z = 0; z < raw_height-1; ++z)
        {
                unsigned int a = x * raw_width + z;
                unsigned int b = (x+1) * raw_width + z;
                unsigned int c = (x+1) * raw_width + (z+1);
                unsigned int d = x * raw_width + (z+1);

                mFaces.push_back(Vector3i(c, b, a));
                mFaces.push_back(Vector3i(a, d, c));
        }
    }

    computeNormals();
  
  return true;
}

// void Mesh::drawQTTerrain()
// {
//     QuadTree* qtTest = new QuadTree(0, 512, 0, 512, 512, 512);

//     std::cout << "------- QuadTree build -------" << std::endl;
//     std::cout << qtTest->nodearray.size() << " node created" << std::endl;

//     int vx, vz;
//     int indexVertices = 0;
//     mVertices.resize(qtTest->nodearray.size() * 8);

//     for (int i = 0; i < qtTest->nodearray.size(); i++)
//     {
//       if (qtTest->nodearray.at(i).level <= 6)
//       {
//         //std::cout << "Begin node draw" << std::endl;
//         for (int x = 0; x < 2; x++)
//         {
//           for (int z = 0; z < 2; z++)
//           {
//             /*std::cout << "------ -------" << std::endl;
//             std::cout << "height : " << hmapMat.rows() << " width " << hmapMat.cols() << std::endl;*/

//             vx = qtTest->nodearray.at(i).vertices[x][z].x;
//             vz = qtTest->nodearray.at(i).vertices[x][z].z;
            
//             //std::cout << vx << " -- " << vz << endl;
//             mVertices[indexVertices] = Vertex(Vector3f(vx* heightmap_x, (hmapMat(vx, vz) * 255) * heightmap_y, vz* heightmap_z));
//             mVertices[indexVertices].texcoord = Vector2f(vx* heightmap_tex_x, vz* heightmap_tex_z);
//             mVertices[indexVertices].visible = true;

//             indexVertices++;
//             vx = qtTest->nodearray.at(i).vertices[x][z+1].x ;
//             vz = qtTest->nodearray.at(i).vertices[x][z+1].z;
            
//             //std::cout << vx << " -- " << vz << endl;
//             mVertices[indexVertices] = Vertex(Vector3f(vx* heightmap_x, (hmapMat(vx, vz) * 255) * heightmap_y, vz* heightmap_z));
//             mVertices[indexVertices].texcoord = Vector2f(vx* heightmap_tex_x, vz* heightmap_tex_z);
//             mVertices[indexVertices].visible = true;

//             indexVertices++;
//             vx = qtTest->nodearray.at(i).vertices[x+1][z].x;
//             vz = qtTest->nodearray.at(i).vertices[x+1][z].z;
            
//             //std::cout << vx << " -- " << vz << endl;
//             mVertices[indexVertices] = Vertex(Vector3f(vx* heightmap_x, (hmapMat(vx, vz) * 255) * heightmap_y, vz* heightmap_z));
//             mVertices[indexVertices].texcoord = Vector2f(vx* heightmap_tex_x, vz* heightmap_tex_z);
//             mVertices[indexVertices].visible = true;

//             indexVertices++;
//             vx = qtTest->nodearray.at(i).vertices[x+1][z+1].x;
//             vz = qtTest->nodearray.at(i).vertices[x+1][z+1].z;
            
//             //std::cout << vx << " -- " << vz << endl;
//             mVertices[indexVertices] = Vertex(Vector3f(vx* heightmap_x, (hmapMat(vx, vz) * 255) * heightmap_y, vz* heightmap_z));
//             mVertices[indexVertices].texcoord = Vector2f(vx* heightmap_tex_x, vz* heightmap_tex_z);
//             mVertices[indexVertices].visible = true;
//             indexVertices++;
//           }
//         }
//       }
//     }
// }


/*bool Mesh::loadOFF(const std::string& filename)
{
  std::ifstream in(filename.c_str(),std::ios::in);
  if(!in)
  {
    std::cerr << "File not found " << filename << std::endl;
    return false;
  }

  std::string header;
  in >> header;

  bool hasColor = false;

  // check the header file
  if(header != "OFF")
  {
    if(header != "COFF")
    {
      std::cerr << "Wrong header = " << header << std::endl;
      return false;
    }
    hasColor = true;
  }

  int nofVertices, nofFaces, inull;
  int nb, id0, id1, id2;
  Vector3f v;

  in >> nofVertices >> nofFaces >> inull;

  for(int i=0 ; i<nofVertices ; ++i)
  {
    in >> v[0] >> v[1] >> v[2];
    mVertices.push_back(Vertex(v));

    if(hasColor) {
      Vector4f c;
      in >> c[0] >> c[1] >> c[2] >> c[3];
      mVertices[i].color = c/255.;
    }
  }

  for(int i=0 ; i<nofFaces ; ++i)
  {
    in >> nb >> id0 >> id1 >> id2;
    assert(nb==3);
    mFaces.push_back(Vector3i(id0, id1, id2));
  }

  in.close();

  computeNormals();

  return true;
}



#include <ObjFormat/ObjFormat.h>

bool Mesh::loadOBJ(const std::string& filename)
{
  ObjMesh* pRawObjMesh = ObjMesh::LoadFromFile(filename);

  if (!pRawObjMesh)
  {
      std::cerr << "Mesh::loadObj: error loading file " << filename << "." << std::endl;
      return false;
  }

  // Makes sure we have an indexed face set
  ObjMesh* pObjMesh = pRawObjMesh->createIndexedFaceSet(Obj::Options(Obj::AllAttribs|Obj::Triangulate));
  delete pRawObjMesh;
  pRawObjMesh = 0;

  // copy vertices
  mVertices.resize(pObjMesh->positions.size());

  for (std::size_t i=0 ; i<pObjMesh->positions.size() ; ++i)
  {
    mVertices[i] = Vertex(Vector3f(pObjMesh->positions[i].x, pObjMesh->positions[i].y, pObjMesh->positions[i].z));

    if(!pObjMesh->texcoords.empty())
      mVertices[i].texcoord = Vector2f(pObjMesh->texcoords[i]);

    if(!pObjMesh->normals.empty())
      mVertices[i].normal = Vector3f(pObjMesh->normals[i]);
  }

  // copy faces
  for (std::size_t smi=0 ; smi<pObjMesh->getNofSubMeshes() ; ++smi)
  {
    const ObjSubMesh* pSrcSubMesh = pObjMesh->getSubMesh((int)smi);

    mFaces.reserve(pSrcSubMesh->getNofFaces());

    for (std::size_t fid = 0 ; fid<pSrcSubMesh->getNofFaces() ; ++fid)
    {
      ObjConstFaceHandle srcFace = pSrcSubMesh->getFace((int)fid);
      mFaces.push_back(Vector3i(srcFace.vPositionId(0), srcFace.vPositionId(1), srcFace.vPositionId(2)));
    }
  }

  if(pObjMesh->normals.empty())
    computeNormals();
  
  return true;
}*/
