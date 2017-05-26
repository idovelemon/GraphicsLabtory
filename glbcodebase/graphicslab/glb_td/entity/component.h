//-------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017/05/25
// Brief: Every entity composed by components
//-------------------------------------------------------------
#ifndef ENTITY_COMPONENT_H_
#define ENTITY_COMPONENT_H_

namespace entity {

//------------------------------------------------------------
// Type declaration

//--------------------------------------------------------------
enum ComponentType {
    CT_UNKNOWN = -1,
    CT_SCRIPT,
    CT_MAX,
};

//--------------------------------------------------------------
class Component {
public:
    Component(ComponentType type)
    : m_Type(type) {
    }

    virtual ~Component() {
    }

public:
    virtual ComponentType GetType() { return m_Type; }

protected:
    ComponentType           m_Type;
};

};  // namespace entity

#endif  // ENTITY_COMPONENT_H_