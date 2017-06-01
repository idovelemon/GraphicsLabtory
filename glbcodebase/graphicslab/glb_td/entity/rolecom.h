//----------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017/05/28
// Brief: Role Component define which role type of this entity
//----------------------------------------------------------
#ifndef ENTITY_ROLECOM_H_
#define ENTITY_ROLECOM_H_

#include "component.h"

namespace entity {

//-------------------------------------------------------------
// Type Declaration

//------------------------------------------------------------

enum RoleMainType {
    RMT_NONE = -1,
    RMT_PLAYER,
    RMT_ENEMY,
    RMT_OPERATOR,
    RMT_MAX
};

enum RoleSubType {
    RST_NONE = -1,
    // Player Begin
    RST_MAINTOWER,
    RST_SUBTOWER,
    RST_WALL,
    RST_BULLET_LASER,
    RST_BULLET_MACHINEGUN,
    RST_PLAYER_END = 100,

    // Enemy Begin
    RST_E01,
    RST_E02,
    RST_ENEMY_END = 200,

    // Operator Begin
    RST_PLACER,
};

//-------------------------------------------------------------
class RoleCom : public Component {
public:
    RoleCom(Entity* entity, RoleMainType mt, RoleSubType st);
    virtual ~RoleCom();

public:
    RoleMainType GetMainType() const;
    RoleSubType GetSubType() const;

protected:
    RoleMainType        m_MainType;
    RoleSubType         m_SubType;
};

};  // namespace entity

#endif  // ENTITY_ROLECOM_H_