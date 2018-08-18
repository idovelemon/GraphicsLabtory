//--------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017/05/26
// Brief: Transform define position, rotation and scale of a entity
//--------------------------------------------------------
#ifndef ENTITY_TRANSFORMCOM_H_
#define ENTITY_TRANSFORMCOM_H_

#include "component.h"

#include "math/glbmath.h"

namespace entity {

//-------------------------------------------------------
// Pre Declaration

class Entity;

//-------------------------------------------------------
// Type Declaration

class TransformCom : public Component {
public:
    TransformCom(Entity* owner, glb::math::Vector pos, glb::math::Vector rotate, glb::math::Vector scale);
    virtual ~TransformCom();

public:
    glb::math::Vector GetPos() const;
    glb::math::Vector GetPosWorld() const;
    glb::math::Vector GetRotate() const;
    glb::math::Vector GetRotateWorld() const;
    glb::math::Vector GetScale() const;
    void SetPos(glb::math::Vector pos);
    void SetRotate(glb::math::Vector rotate);
    void SetScale(glb::math::Vector scale);
    glb::math::Matrix GetWorldMatrix();

protected:
    glb::math::Vector   m_Pos;
    glb::math::Vector   m_Rotate;
    glb::math::Vector   m_Scale;
};

};  // namespace entity

#endif  // ENTITY_TRANSFORMCOM_H_