#include "ViewerOpenGL.h"
#include "Camera.h"

using namespace Eigen;

ViewerOpenGL::ViewerOpenGL()
    : _winWidth(0), _winHeight(0), _theta(0), _rotate(false), _wireframe(false)
{
}

ViewerOpenGL::~ViewerOpenGL()
{
}

////////////////////////////////////////////////////////////////////////////////
// GL stuff

// initialize OpenGL context
void ViewerOpenGL::init(int w, int h)
{

  // Background color
  glClearColor(0.0, 0.0, 0.0, 0.0);

  loadShaders();
  if (_terrain.loadHeightmap(DATA_DIR "/textures/Terrain.png") == -1)
    exit(1);
  _terrain.initVBA();

  reshape(w, h);
  _cam.setPerspective(M_PI / 3, 0.3f, 20000.0f);
  //_cam.setPerspective(0, 10000.0f, 0);
  //_cam.lookAt(Vector3f(_terrain.getRaw_width() / 2, 100, _terrain.getRaw_height() / 2), Vector3f(0, 0, 0), Vector3f(0, 1, 0));
  //_cam.lookAt(Vector3f(-100.0f, 100.0f, -100.0f), Vector3f(5.0f, 0.0f, 1.0f), Vector3f(0, 4, 0));
  //_cam.lookAt(Vector3f(-100.0f, 150.0f, -130.0f), Vector3f(5.0f, 50.0f, 30.0f), Vector3f(0, 10, 0));
  _cam.lookAt(Vector3f(_terrain.getRaw_width(), _terrain.getRaw_height()/2, _terrain.getRaw_height()), Vector3f(1, 1, 1), Vector3f(0, 1, 0));

  _trackball.setCamera(&_cam);

  glEnable(GL_DEPTH_TEST);
}

void ViewerOpenGL::reshape(int w, int h)
{
  _winWidth = w;
  _winHeight = h;
  _cam.setViewport(w, h);
}

/*!
   callback to draw graphic primitives
 */
void ViewerOpenGL::drawScene()
{
  // configure the rendering target size (viewport)
  glViewport(0, 0, _winWidth, _winHeight);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  _shader.activate();

  glUniform1i(_shader.getUniformLocation("wireframe"), 0);
  glUniformMatrix4fv(_shader.getUniformLocation("view_mat"), 1, GL_FALSE, _cam.viewMatrix().data());
  glUniformMatrix4fv(_shader.getUniformLocation("proj_mat"), 1, GL_FALSE, _cam.projectionMatrix().data());

  Affine3f M(AngleAxisf(_theta, Vector3f(0, 1, 0)));

  glUniformMatrix4fv(_shader.getUniformLocation("obj_mat"), 1, GL_FALSE, M.matrix().data());

  Matrix4f matLocal2Cam = _cam.viewMatrix() * M.matrix();
  Matrix3f matN = matLocal2Cam.topLeftCorner<3, 3>().inverse().transpose();
  glUniformMatrix3fv(_shader.getUniformLocation("normal_mat"), 1, GL_FALSE, matN.data());

  Vector3f lightDir = Vector3f(1, 0, 1).normalized();
  lightDir = (matLocal2Cam.topLeftCorner<3, 3>() * lightDir).normalized();
  glUniform3fv(_shader.getUniformLocation("lightDir"), 1, lightDir.data());

  _terrain.draw(_shader);

  if (_wireframe)
  {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_LINE_SMOOTH);
    glDepthFunc(GL_LEQUAL);
    glUniform1i(_shader.getUniformLocation("wireframe"), 1);
    _terrain.draw(_shader);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glUniform1i(_shader.getUniformLocation("wireframe"), 0);
  }

  _shader.deactivate();
}

void ViewerOpenGL::updateAndDrawScene()
{
 // _terrain.updateTerrain(_cam.getPosition(), _cam.getFar());
  if (_rotate)
    _theta += 0.01 * M_PI;
  drawScene();
}

void ViewerOpenGL::loadShaders()
{
  // Here we can load as many shaders as we want
  _shader.loadFromFiles(DATA_DIR "/shaders/simple.vert", DATA_DIR "/shaders/simple.frag");
  checkError();
}

////////////////////////////////////////////////////////////////////////////////
// Events

/*!
   callback to manage mouse : called when user press or release mouse button
   You can change in this function the way the user
   interact with the application.
 */
void ViewerOpenGL::mousePressed(int /*button*/, int action)
{
  if (action == GLFW_PRESS)
  {
    _trackingMode = TM_ROTATE_AROUND;
    _trackball.start();
    _trackball.track(_lastMousePos);
  }
  else if (action == GLFW_RELEASE)
  {
    _trackingMode = TM_NO_TRACK;
  }
}

/*!
   callback to manage mouse : called when user move mouse with button pressed
   You can change in this function the way the user
   interact with the application.
 */
void ViewerOpenGL::mouseMoved(int x, int y)
{
  if (_trackingMode == TM_ROTATE_AROUND)
  {
    _trackball.track(Vector2i(x, y));
  }

  _lastMousePos = Vector2i(x, y);
}

void ViewerOpenGL::mouseScroll(double /*x*/, double y)
{
  _cam.zoom(-0.1 * y);
}

/*!
   callback to manage keyboard interactions
   You can change in this function the way the user
   interact with the application.
 */
void ViewerOpenGL::keyPressed(int key, int action, int /*mods*/)
{
  if (action == GLFW_PRESS)
  {
    if (key == GLFW_KEY_R)
    {
      loadShaders();
    }
    else if (key == GLFW_KEY_A)
    {
      _rotate = !_rotate;
    }
    else if (key == GLFW_KEY_W)
    {
      _wireframe = !_wireframe;
    }
    else if (key == GLFW_KEY_S)
    {
      //_terrain.subdivide();
    }
  }

  if (action == GLFW_PRESS || action == GLFW_REPEAT)
  {
    if (key == GLFW_KEY_UP)
    {
      _cam.lookUp(0.01);
    }
    else if (key == GLFW_KEY_DOWN)
    {
      _cam.lookDown(0.01);
    }
    else if (key == GLFW_KEY_LEFT)
    {
      _cam.lookLeft(0.01);
    }
    else if (key == GLFW_KEY_RIGHT)
    {
      _cam.lookRight(0.01);
    }
    else if (key == GLFW_KEY_PAGE_UP)
    {
      _cam.moveUp(0.02);
    }
    else if (key == GLFW_KEY_PAGE_DOWN)
    {
      _cam.moveDown(0.02);
    }
    else if (key == GLFW_KEY_E)
    {
      _cam.moveForward(0.01);
    }
    else if (key == GLFW_KEY_D)
    {
      _cam.moveBackward(0.01);
    }
    else if (key == GLFW_KEY_S)
    {
      _cam.moveLeft(0.01);
    }
    else if (key == GLFW_KEY_F)
    {
      _cam.moveRight(0.01);
    }
  }
}

void ViewerOpenGL::charPressed(int /*key*/)
{
}
