//--------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017/05/26
// Brief: Transform define position, rotation and scale of a entity
//--------------------------------------------------------
#include "transformcom.h"

#include "math/glbmatrix.h"

#include "entity.h"
#include "entitymgr.h"
#include "relationshipcom.h"

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

glb::math::Vector TransformCom::GetPosWorld() const {
    glb::math::Vector pos;

    if (m_Entity) {
        RelationshipCom* relationshipCom = reinterpret_cast<RelationshipCom*>(m_Entity->GetComponent(CT_RELATIONSHIP));
        if (relationshipCom && relationshipCom->GetParent() != -1) {
            TransformCom* transformCom = reinterpret_cast<TransformCom*>(EntityMgr::GetEntity(relationshipCom->GetParent())->GetComponent(CT_TRANSFORM));
            assert(transformCom != nullptr);

            glb::math::Matrix parentToWorld;
            parentToWorld = transformCom->GetWorldMatrix();

            pos = parentToWorld * m_Pos;
        } else {
            pos = m_Pos;
        }
    }

    return pos;
}

glb::math::Vector TransformCom::GetRotate() const {
    return m_Rotate;
}

glb::math::Vector TransformCom::GetRotateWorld() const {
    glb::math::Vector rotation;

    if (m_Entity) {
        RelationshipCom* relationshipCom = reinterpret_cast<RelationshipCom*>(m_Entity->GetComponent(CT_RELATIONSHIP));
        if (relationshipCom && relationshipCom->GetParent() != -1) {
            TransformCom* transformCom = reinterpret_cast<TransformCom*>(EntityMgr::GetEntity(relationshipCom->GetParent())->GetComponent(CT_TRANSFORM));
            assert(transformCom != nullptr);

            rotation = transformCom->GetRotate() + m_Rotate;
        } else {
            rotation = m_Rotate;
        }
    }

    return rotation;
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

glb::math::Matrix TransformCom::GetWorldMatrix() {
    glb::math::Matrix mat;

    glb::math::Matrix local;
    local.MakeScaleMatrix(m_Scale.x, m_Scale.y, m_Scale.z);
    local.RotateXYZ(m_Rotate.x, m_Rotate.y, m_Rotate.z);
    local.Translate(m_Pos.x, m_Pos.y, m_Pos.z);

    if (m_Entity) {
        glb::math::Matrix parentToWorld;
        parentToWorld.MakeIdentityMatrix();

        RelationshipCom* relationshipCom = reinterpret_cast<RelationshipCom*>(m_Entity->GetComponent(CT_RELATIONSHIP));
        if (relationshipCom && relationshipCom->GetParent() != -1) {
            TransformCom* transformCom = reinterpret_cast<TransformCom*>(EntityMgr::GetEntity(relationshipCom->GetParent())->GetComponent(CT_TRANSFORM));
            assert(transformCom != nullptr);

            parentToWorld = transformCom->GetWorldMatrix();
            mat = parentToWorld * local;
        } else {
            mat = local;
        }
    }

    return mat;
}

};  // namespace entity