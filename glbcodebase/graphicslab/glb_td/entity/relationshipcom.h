//--------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2018. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2018/08/18
// Brief: Relationship between entities
//--------------------------------------------------------
#ifndef ENTITY_RELATIONSHIP_COM_H_
#define ENTITY_RELATIONSHIP_COM_H_

#include "component.h"

#include <stdint.h>

#include <vector>

namespace entity {

//-------------------------------------------------------
// Pre Declaration

class Entity;

//-------------------------------------------------------
// Type Declaration

class RelationshipCom : public Component {
public:
    RelationshipCom(Entity* owner);
    virtual ~RelationshipCom();

public:
    void SetParent(int32_t id);
    int32_t GetParent() const;
    void AddChild(int32_t id);
    const std::vector<int32_t>& GetChildren() const;
    void RemoveChild(int32_t childID);
    void RemoveAllChildren();

protected:
    int32_t                 m_ParentEntityID;
    std::vector<int32_t>    m_ChildrenEntityID;
};

};  // namespace entity

#endif  // ENTITY_RELATIONSHIP_COM_H_