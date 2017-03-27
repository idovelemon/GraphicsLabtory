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

    virtual void Clone(CameraBase** cam) = 0;
    virtual void Restore(CameraBase** cam) = 0;
    virtual void Update(float dt) = 0;
    
    virtual Matrix GetViewMatrix() {
        return m_ViewMatrix;
    }

    virtual void SetViewMatrix(Matrix m) {
        m_ViewMatrix = m;
    }

    virtual Vector GetPos() {
        return m_Pos;
    }

    virtual void SetPos(Vector pos) {
        m_Pos = pos;
    }

    virtual Vector GetTarget() {
        return m_Target;
    }

    virtual void SetTarget(Vector target) {
        m_Target = target;
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
    virtual void Clone(CameraBase** cam);
    virtual void Restore(CameraBase** cam);
    virtual void Update(float dt);
    void Move(float dx, float dy, float dz);
    void Rotate(float rx, float ry);
};

class ModelCamera : public CameraBase {
public:
    ModelCamera();
    virtual ~ModelCamera();

    static ModelCamera* Create(Vector pos, Vector target);

public:
    virtual void Clone(CameraBase** cam);
    virtual void Restore(CameraBase** cam);
    virtual void Update(float dt);
    void Rotate(float rot);
};

};  // namespace camera

};  // namespace glb

#endif  // GLB_GLBCAMERA_H_