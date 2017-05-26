//-------------------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016 / 06 / 25
// Brief: 3D camera
//-------------------------------------------------------------------------------
#ifndef GLB_GLBCAMERA_H_
#define GLB_GLBCAMERA_H_

#include "math/glbmatrix.h"
#include "math/glbvector.h"

namespace glb {

namespace scene {

class CameraBase {
public:
    CameraBase()
    : m_ViewMatrix()
    , m_Pos(0.0f, 0.0f, 0.0f)
    , m_Target(0.0f, 0.0f, 0.0f) {
    }

    virtual ~CameraBase() {
    }

    virtual void Clone(CameraBase** cam);
    virtual void Restore(CameraBase** cam);
    virtual void Update(float dt);
    
    virtual math::Matrix GetViewMatrix() {
        return m_ViewMatrix;
    }

    virtual void SetViewMatrix(math::Matrix m) {
        m_ViewMatrix = m;
    }

    virtual math::Vector GetPos() {
        return m_Pos;
    }

    virtual void SetPos(math::Vector pos) {
        m_Pos = pos;
    }

    virtual math::Vector GetTarget() {
        return m_Target;
    }

    virtual void SetTarget(math::Vector target) {
        m_Target = target;
    }

protected:
    math::Matrix m_ViewMatrix;
    math::Vector m_Pos;
    math::Vector m_Target;
};

class FreeCamera : public CameraBase {
public:
    FreeCamera();
    virtual ~FreeCamera();

    static FreeCamera* Create(math::Vector pos, math::Vector target);

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

    static ModelCamera* Create(math::Vector pos, math::Vector target);

public:
    virtual void Clone(CameraBase** cam);
    virtual void Restore(CameraBase** cam);
    virtual void Update(float dt);
    void Rotate(float rot);
};

};  // namespace scene

};  // namespace glb

#endif  // GLB_GLBCAMERA_H_