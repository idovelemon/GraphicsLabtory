//----------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017/05/26
// Brief: Camera control how to view the game world
//----------------------------------------------------
#include "cameracom.h"

#include "scene/glbcamera.h"
#include "scene/glbscene.h"

#include "entity.h"
#include "transformcom.h"

namespace entity {

CameraCom::CameraCom(Entity* owner, glb::math::Vector pos, glb::math::Vector target)
: Component(CT_CAMERA, owner)
, m_Cam(NULL) {
    m_Cam = new glb::scene::CameraBase;
    if (m_Cam != NULL) {
        m_Cam->SetPos(pos);
        m_Cam->SetTarget(target);
        glb::scene::Scene::SetCamera(glb::scene::PRIMIAY_CAM, m_Cam);
    }
}

CameraCom::~CameraCom() {
    if (m_Cam != NULL) {
        delete m_Cam;
        m_Cam = NULL;
        glb::scene::Scene::SetCamera(glb::scene::PRIMIAY_CAM, NULL);
    }
}

void CameraCom::Update() {
    if (m_Entity) {
        TransformCom* transform = reinterpret_cast<TransformCom*>(m_Entity->GetComponent(CT_TRANSFORM));
        if (transform && m_Cam) {
            glb::math::Vector pos = transform->GetPosWorld();
            glb::math::Vector camPos = m_Cam->GetPos();
            glb::math::Vector camTgt = m_Cam->GetTarget();
            glb::math::Vector toTgt = camTgt - camPos;
            camPos = pos;
            camTgt = toTgt + camPos;
            m_Cam->SetPos(camPos);
            m_Cam->SetTarget(camTgt);
        }
    }

    if (m_Cam != NULL) {
        m_Cam->Update(1.0f);
    }
}

};  // namespace entity