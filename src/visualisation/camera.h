
#ifndef CAMERA_H
#define CAMERA_H

#include <Eigen/Geometry>

class Camera
{

public:
  Camera();
  virtual ~Camera();

  /** Setup the camera position and orientation based on its \a position, \a a target point, \a the up vector */
  void lookAt(const Eigen::Vector3f &position, const Eigen::Vector3f &target, const Eigen::Vector3f &up);

  /** Setup the perspective projection matrix */
  void setPerspective(float fovY, float near, float far);

  void zoom(float x);

  void rotateAroundTarget(float angle, Eigen::Vector3f axis);

  void moveForward(float x);
  void moveBackward(float x);
  void moveLeft(float x);
  void moveRight(float x);
  void moveUp(float x);
  void moveDown(float x);
  void lookUp(float x);
  void lookDown(float x);
  void lookLeft(float x);
  void lookRight(float x);

  /** Returns the affine transformation matrix from the global space to the camera space */
  const Eigen::Matrix4f &viewMatrix() const;
  /** Returns the perspective projection matrix */
  Eigen::Matrix4f projectionMatrix() const;

  void setViewport(int width, int height);

  int vpWidth() const { return mVpWidth; }
  int vpHeight() const { return mVpHeight; }
  float getFovY() const { return m_fovY; }
  float getNear() const { return m_near; }
  float getFar() const { return m_far; }
  Eigen::Vector3f getPosition() { return mPosition; }
  Eigen::Vector3f getForward() { return mForward; }
  Eigen::Vector3f getUp() { return mUp; }
  Eigen::Vector3f getRight() { return mRight; }

protected:
  Eigen::Matrix4f mViewMatrix;
  Eigen::Vector3f mTarget;
  Eigen::Vector3f mPosition;
  Eigen::Vector3f mForward;
  Eigen::Vector3f mUp;
  Eigen::Vector3f mRight;

  float m_fovY, m_near, m_far;
  int mVpWidth, mVpHeight;
};

#endif // EIGEN_CAMERA_H
