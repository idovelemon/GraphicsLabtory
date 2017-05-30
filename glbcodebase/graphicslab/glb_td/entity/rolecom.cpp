//----------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017/05/28
// Brief: Role Component define which role type of this entity
//----------------------------------------------------------
#include "rolecom.h"

namespace entity {

RoleCom::RoleCom(Entity* entity, RoleMainType mt, RoleSubType st)
: Component(CT_ROLETYPE, entity)
, m_MainType(mt)
, m_SubType(st) {
}

RoleCom::~RoleCom() {
}

RoleMainType RoleCom::GetMainType() const {
    return m_MainType;
}

RoleSubType RoleCom::GetSubType() const {
    return m_SubType;
}

};  // namespace entity