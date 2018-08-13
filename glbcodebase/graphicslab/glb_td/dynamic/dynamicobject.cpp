//---------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017/06/01
// Brief: Dynamic object implement basic primitive test
//---------------------------------------------------------
#include "dynamicobject.h"

#include <assert.h>

#include "math/glbmatrix.h"

#include "BulletCollision/CollisionShapes/btBoxShape.h"

namespace dynamic {

//-----------------------------------------------------------------
DTAabb::DTAabb(glb::math::Vector max, glb::math::Vector min, glb::math::Vector pos)
: DynamicObject(DOT_AABB)
, m_OriMax(max)
, m_OriMin(min) {
    glb::math::Vector half = max - min;
    half = half * 0.5f;

    btVector3 halfExt(half.x, half.y, half.z);

    m_btCollisionShape = new btBoxShape(halfExt);
    m_btCollision = new btCollisionObject();
    m_btCollision->setCollisionShape(m_btCollisionShape);

    glb::math::Matrix mat;
    mat.MakeIdentityMatrix();
    mat.Translate(pos.x, pos.y, pos.z);
    btTransform transform;
    transform.setFromOpenGLMatrix(mat.m_Matrix.v);
    m_btCollision->setWorldTransform(transform);
}

DTAabb::~DTAabb() {
    if (m_btCollision) {
        delete m_btCollision;
        m_btCollision = nullptr;
    }

    if (m_btCollisionShape) {
        delete m_btCollisionShape;
        m_btCollisionShape = nullptr;
    }
}

void DTAabb::Update(glb::math::Vector pos, glb::math::Vector rot, glb::math::Vector scale) {
    glb::math::Matrix mat;
    mat.MakeIdentityMatrix();
    mat.Scale(scale.x, scale.y, scale.z);
    mat.Translate(pos.x, pos.y, pos.z);
    btTransform transform;
    transform.setFromOpenGLMatrix(mat.m_Matrix.v);
    m_btCollision->setWorldTransform(transform);
}

};  // namespace dynamic