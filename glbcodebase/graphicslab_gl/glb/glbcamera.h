//-------------------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016 / 06 / 25
// Brief: 3D camera
//-------------------------------------------------------------------------------
#ifndef GLB_GLBCAMERA_H_
#define GLB_GLBCAMERA_H_

#include "glbmatrix.h"
#include "glbvector.h"

namespace glb {

namespace camera {

class CameraBase {
public:
    CameraBase()
    : m_ViewMatrix()
    , m_Pos(0.0f, 0.0f, 0.0f)
    , m_Target(0.0f, 0.0f, 0.0f) {
    }

    virtual ~CameraBase() {
    }

    virtual void Update(float dt) = 0;
    
    virtual Matrix GetViewMatrix() {
        return m_ViewMatrix;
    }

    virtual Vector GetPos() {
        return m_Pos;
    }

    virtual Vector GetTarget() {
        return m_Target;
    }

protected:
    Matrix m_ViewMatrix;
    Vector m_Pos;
    Vector m_Target;
};

class FreeCamera : public CameraBase {
public:
    FreeCamera();
    virtual ~FreeCamera();

    static FreeCamera* Create(Vector pos, Vector target);

public:
    virtual void Update(float dt);
    void Move(float dx, float dy, float dz);
    void Rotate(float rx, float ry);
};

};  // namespace camera

};  // namespace glb

#endif  // GLB_GLBCAMERA_H_