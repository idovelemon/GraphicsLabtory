//-------------------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016 / 06 / 25
// Brief: 3D camera
//-------------------------------------------------------------------------------
#include "glbcamera.h"

#include <stdio.h>

#include "glbmacro.h"

namespace glb {

namespace camera {

FreeCamera::FreeCamera() {
}

FreeCamera::~FreeCamera() {
}

FreeCamera* FreeCamera::Create(Vector pos, Vector target) {
    FreeCamera* camera = new FreeCamera();
    if (camera != NULL) {
        camera->m_Pos = pos;
        camera->m_Target = target;
        camera->m_ViewMatrix.MakeViewMatrix(pos, target);
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return camera;
}

void FreeCamera::Update(float dt) {
    m_ViewMatrix.MakeViewMatrix(m_Pos, m_Target);
}

void FreeCamera::Move(float dx, float dy, float dz) {
    Vector local_x(m_ViewMatrix.GetElement(0, 0), m_ViewMatrix.GetElement(0, 1), m_ViewMatrix.GetElement(0, 2));
    local_x.Normalize();

    Vector local_y(m_ViewMatrix.GetElement(1, 0), m_ViewMatrix.GetElement(1, 1), m_ViewMatrix.GetElement(1, 2));
    local_y.Normalize();

    Vector local_z(m_ViewMatrix.GetElement(2, 0), m_ViewMatrix.GetElement(2, 1), m_ViewMatrix.GetElement(2, 2));
    local_z.Normalize();

    m_Pos = m_Pos + local_x * dx;
    m_Pos = m_Pos + local_y * dy;
    m_Pos = m_Pos + local_z * dz;
    m_Target = m_Target + local_x * dx;
    m_Target = m_Target + local_y * dy;
    m_Target = m_Target + local_z * dz;
}

void FreeCamera::Rotate(float rx, float ry) {
    Vector local_x(m_ViewMatrix.GetElement(0, 0), m_ViewMatrix.GetElement(0, 1), m_ViewMatrix.GetElement(0, 2));
    local_x.Normalize();

    Vector local_y(m_ViewMatrix.GetElement(1, 0), m_ViewMatrix.GetElement(1, 1), m_ViewMatrix.GetElement(1, 2));
    local_y.Normalize();

    Matrix rot_x;
    rot_x.MakeRotateAxisMatrix(local_x, rx);

    Matrix rot_y;
    rot_y.MakeRotateAxisMatrix(local_y, ry);

    Vector to_target = m_Target - m_Pos;
    to_target = rot_x * to_target;
    to_target = rot_y * to_target;

    m_Target = m_Pos + to_target;
}

};  // namespace camera

};  // namespace glb