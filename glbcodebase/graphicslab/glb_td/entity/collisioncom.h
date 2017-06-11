//---------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017/06/08
// Brief: Collision deal with collision detection with dynamic world
//---------------------------------------------------------------
#ifndef ENTITY_COLLISIONCOM_H_
#define ENTITY_COLLISIONCOM_H_

#include <stdint.h>
#include <vector>

#include "component.h"
#include "math/glbvector.h"

namespace entity {

//-----------------------------------------------------------
// Pre Declaration

class Entity;

//-----------------------------------------------------------
// Type Declaration

class CollisionCom : public Component {
public:
    CollisionCom(Entity* owner);
    virtual ~CollisionCom();

public:
    void Update();
    void CheckCollision();
    void BeginIterate();
    int Iterate();
    void EndIterate();
    float GetWidth();
    float GetLength();

protected:
    int32_t                     m_DynamicObjectID;
    std::vector<int32_t>        m_CollIDs;
    int32_t                     m_Iterate;
    glb::math::Vector           m_OriMax;
    glb::math::Vector           m_OriMin;
};

};  // namespace entity

#endif  // ENTITY_COLLISIONCOM_H_