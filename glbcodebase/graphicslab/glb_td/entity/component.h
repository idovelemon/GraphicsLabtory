//-------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017/05/25
// Brief: Every entity composed by components
//-------------------------------------------------------------
#ifndef ENTITY_COMPONENT_H_
#define ENTITY_COMPONENT_H_

#include <stdio.h>

namespace entity {

//------------------------------------------------------------
// Pre Declaration

class Entity;

//------------------------------------------------------------
// Type declaration

//--------------------------------------------------------------
enum ComponentType {
    CT_UNKNOWN = -1,
    CT_TRANSFORM,
    CT_RENDER,
    CT_ROLETYPE,
    CT_CAMERA,
    CT_SCRIPT,
    CT_ARSENAL,
    CT_DATA,
    CT_COLLISION,
    CT_MAX,
};

//--------------------------------------------------------------
class Component {
public:
    Component(ComponentType type, Entity* owner)
    : m_Type(type)
    , m_Entity(owner) {
    }

    virtual ~Component() {
        m_Entity = NULL;
    }

    virtual void Update() {};

public:
    virtual ComponentType GetType() const { return m_Type; }
    virtual Entity* GetOwner() const { return m_Entity; }

protected:
    ComponentType           m_Type;
    Entity*                 m_Entity;
};

};  // namespace entity

#endif  // ENTITY_COMPONENT_H_