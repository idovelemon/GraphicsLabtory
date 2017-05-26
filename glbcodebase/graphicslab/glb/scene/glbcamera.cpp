//-------------------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016 / 06 / 25
// Brief: 3D camera
//-------------------------------------------------------------------------------
#include "glbcamera.h"

#include <stdio.h>

#include "util/glbmacro.h"

namespace glb {

namespace scene {

//-----------------------------------------------------------------------------------
// CLASS DEFINITION
//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// CameraBase DEFINITION
//-----------------------------------------------------------------------------------
void CameraBase::Clone(CameraBase** cam) {
    if (cam != NULL) {
        if (*cam == NULL) {
            FreeCamera* free_cam = new FreeCamera;
            if (free_cam != NULL) {
                free_cam->m_Pos = m_Pos;
                free_cam->m_Target = m_Target;
                free_cam->m_ViewMatrix = m_ViewMatrix;
                *cam = free_cam;
            } else {
                GLB_SAFE_ASSERT(false);
            }
        } else {
            GLB_SAFE_ASSERT(false);
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void CameraBase::Restore(CameraBase** cam) {
    if (cam != NULL) {
        if (*cam != NULL) {
            FreeCamera* free_cam = reinterpret_cast<FreeCamera*>(*cam);
            m_Pos = free_cam->m_Pos;
            m_Target = free_cam->m_Target;
            m_ViewMatrix = free_cam->m_ViewMatrix;
            GLB_SAFE_DELETE((*cam));
        } else {
            GLB_SAFE_ASSERT(false);
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void CameraBase::Update(float dt) {
    m_ViewMatrix.MakeViewMatrix(m_Pos, m_Target);
}

//-----------------------------------------------------------------------------------
// FreeCamera DEFINITION
//-----------------------------------------------------------------------------------
FreeCamera::FreeCamera() {
}

FreeCamera::~FreeCamera() {
}

FreeCamera* FreeCamera::Create(math::Vector pos, math::Vector target) {
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

void FreeCamera::Clone(CameraBase** cam) {
    if (cam != NULL) {
        if (*cam == NULL) {
            FreeCamera* free_cam = new FreeCamera;
            if (free_cam != NULL) {
                free_cam->m_Pos = m_Pos;
                free_cam->m_Target = m_Target;
                free_cam->m_ViewMatrix = m_ViewMatrix;
                *cam = free_cam;
            } else {
                GLB_SAFE_ASSERT(false);
            }
        } else {
            GLB_SAFE_ASSERT(false);
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void FreeCamera::Restore(CameraBase** cam) {
    if (cam != NULL) {
        if (*cam != NULL) {
            FreeCamera* free_cam = reinterpret_cast<FreeCamera*>(*cam);
            m_Pos = free_cam->m_Pos;
            m_Target = free_cam->m_Target;
            m_ViewMatrix = free_cam->m_ViewMatrix;
            GLB_SAFE_DELETE((*cam));
        } else {
            GLB_SAFE_ASSERT(false);
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void FreeCamera::Update(float dt) {
    m_ViewMatrix.MakeViewMatrix(m_Pos, m_Target);
}

void FreeCamera::Move(float dx, float dy, float dz) {
    math::Vector local_x(m_ViewMatrix.GetElement(0, 0), m_ViewMatrix.GetElement(0, 1), m_ViewMatrix.GetElement(0, 2));
    local_x.Normalize();

    math::Vector local_y(m_ViewMatrix.GetElement(1, 0), m_ViewMatrix.GetElement(1, 1), m_ViewMatrix.GetElement(1, 2));
    local_y.Normalize();

    math::Vector local_z(m_ViewMatrix.GetElement(2, 0), m_ViewMatrix.GetElement(2, 1), m_ViewMatrix.GetElement(2, 2));
    local_z.Normalize();

    m_Pos = m_Pos + local_x * dx;
    m_Pos = m_Pos + local_y * dy;
    m_Pos = m_Pos + local_z * dz;
    m_Target = m_Target + local_x * dx;
    m_Target = m_Target + local_y * dy;
    m_Target = m_Target + local_z * dz;
}

void FreeCamera::Rotate(float rx, float ry) {
    math::Vector local_x(m_ViewMatrix.GetElement(0, 0), m_ViewMatrix.GetElement(0, 1), m_ViewMatrix.GetElement(0, 2));
    local_x.Normalize();

    math::Vector local_y(m_ViewMatrix.GetElement(1, 0), m_ViewMatrix.GetElement(1, 1), m_ViewMatrix.GetElement(1, 2));
    local_y.Normalize();

    math::Matrix rot_x;
    rot_x.MakeRotateAxisMatrix(local_x, rx);

    math::Matrix rot_y;
    rot_y.MakeRotateAxisMatrix(local_y, ry);

    math::Vector to_target = m_Target - m_Pos;
    to_target = rot_x * to_target;
    to_target = rot_y * to_target;

    m_Target = m_Pos + to_target;
}

//-----------------------------------------------------------------------------------
// ModelCamera DEFINITION
//-----------------------------------------------------------------------------------
ModelCamera::ModelCamera() {
}

ModelCamera::~ModelCamera() {
}

ModelCamera* ModelCamera::Create(math::Vector pos, math::Vector target) {
    ModelCamera* camera = new ModelCamera();
    if (camera != NULL) {
        camera->m_Pos = pos;
        camera->m_Target = target;
        camera->m_ViewMatrix.MakeViewMatrix(pos, target);
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return camera;
}

void ModelCamera::Clone(CameraBase** cam) {
    if (cam != NULL) {
        if (*cam == NULL) {
            ModelCamera* model_cam = new ModelCamera;
            if (model_cam != NULL) {
                model_cam->m_Pos = m_Pos;
                model_cam->m_Target = m_Target;
                model_cam->m_ViewMatrix = m_ViewMatrix;
                *cam = model_cam;
            } else {
                GLB_SAFE_ASSERT(false);
            }
        } else {
            GLB_SAFE_ASSERT(false);
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void ModelCamera::Restore(CameraBase** cam) {
    if (cam != NULL) {
        if (*cam != NULL) {
            ModelCamera* free_cam = reinterpret_cast<ModelCamera*>(*cam);
            m_Pos = free_cam->m_Pos;
            m_Target = free_cam->m_Target;
            m_ViewMatrix = free_cam->m_ViewMatrix;
            GLB_SAFE_DELETE((*cam));
        } else {
            GLB_SAFE_ASSERT(false);
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void ModelCamera::Update(float dt) {
    m_ViewMatrix.MakeViewMatrix(m_Pos, m_Target);
}

void ModelCamera::Rotate(float rot) {
    math::Vector to_cam = m_Pos - m_Target;
    math::Matrix m;
    m.MakeTranslateMatrix(-m_Target.x, -m_Target.y, -m_Target.z);
    m.RotateY(rot);
    m.Translate(m_Target.x, m_Target.y, m_Target.z);
    to_cam = m * to_cam;
    m_Pos = m_Target + to_cam;
}

};  // namespace scene

};  // namespace glb