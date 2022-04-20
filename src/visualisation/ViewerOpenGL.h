#ifndef VIEWER_H
#define VIEWER_H

#include "opengl.h"
#include "Shader.h"
#include "Camera.h"
#include "TrackBall.h"
// #include "Mesh.h"
#include "Terrain.h"

#include "ViewerAbstract.h"

#include <iostream>

class ViewerOpenGL : public ViewerAbstract {
public:
    //! Constructor
    ViewerOpenGL();
    virtual ~ViewerOpenGL();

    // gl stuff
    void init(int w, int h) override;
    void drawScene() override;
    void updateAndDrawScene() override;
    void reshape(int w, int h) override;
    void loadShaders() override;

    // events
    void mousePressed(int button, int action) override;
    void mouseMoved(int x, int y) override;
    void mouseScroll(double x, double y) override;
    void keyPressed(int key, int action, int mods) override;
    void charPressed(int key) override;

private:
    int _winWidth, _winHeight;

    Camera _cam;
    Shader _shader;
    Terrain _terrain;
    // Mesh   _mesh;

    float _theta;
    bool _rotate;
    bool _wireframe;


    // Mouse parameters for the TrackBall
    enum TrackMode
    {
      TM_NO_TRACK=0, TM_ROTATE_AROUND, TM_ZOOM,
      TM_LOCAL_ROTATE, TM_FLY_Z, TM_FLY_PAN
    };
    TrackMode _trackingMode = TM_NO_TRACK;
    TrackBall _trackball;
    Eigen::Vector2i _lastMousePos;
};

#endif
