//--------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017/05/26
// Brief: Transform define position, rotation and scale of a entity
//--------------------------------------------------------
#include "transformcom.h"

namespace entity {

TransformCom::TransformCom(Entity* owner, glb::math::Vector pos, glb::math::Vector rotate, glb::math::Vector scale)
: Component(CT_TRANSFORM, owner)
, m_Pos(pos)
, m_Rotate(rotate)
, m_Scale(scale) {
}

TransformCom::~TransformCom() {
}

glb::math::Vector TransformCom::GetPos() const {
    return m_Pos;
}

glb::math::Vector TransformCom::GetRotate() const {
    return m_Rotate;
}

glb::math::Vector TransformCom::GetScale() const {
    return m_Scale;
}

void TransformCom::SetPos(glb::math::Vector pos) {
    m_Pos = pos;
}

void TransformCom::SetRotate(glb::math::Vector rotate) {
    m_Rotate = rotate;
}

void TransformCom::SetScale(glb::math::Vector scale) {
    m_Scale = scale;
}

};  // namespace entity