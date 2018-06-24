//----------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017/05/26
// Brief: Camera control how to view the game world
//----------------------------------------------------
#ifndef ENTITY_CAMERACOM_H_
#define ENTITY_CAMERACOM_H_

#include "component.h"

#include "math/glbvector.h"

//----------------------------------------------------------
// Pre Declaration

namespace glb {
namespace scene {
    class CameraBase;
}
}

namespace entity {

//-----------------------------------------------------------
// Pre Declaration

class Entity;

//-----------------------------------------------------------
// Type Declaration

class CameraCom : public Component {
public:
    CameraCom(Entity* owner, glb::math::Vector pos, glb::math::Vector target);
    virtual ~CameraCom();

public:
    virtual void Update();

protected:
    glb::scene::CameraBase*     m_Cam;
};

};  // namespace entity

#endif  // ENTITY_CAMERACOM_H_