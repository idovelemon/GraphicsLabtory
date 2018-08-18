//--------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2018. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2018/08/18
// Brief: Relationship between entities
//--------------------------------------------------------
#include "relationshipcom.h"

namespace entity {

RelationshipCom::RelationshipCom(Entity* owner)
: Component(CT_RELATIONSHIP, owner)
, m_ParentEntityID(-1) {
    m_ChildrenEntityID.clear();
}

RelationshipCom::~RelationshipCom() {
}

void RelationshipCom::SetParent(int32_t id) {
    m_ParentEntityID = id;
}

int32_t RelationshipCom::GetParent() const {
    return m_ParentEntityID;
}

void RelationshipCom::AddChild(int32_t id) {
    m_ChildrenEntityID.push_back(id);
}

const std::vector<int32_t>& RelationshipCom::GetChildren() const {
    return m_ChildrenEntityID;
}

void RelationshipCom::RemoveChild(int32_t childID) {
    std::vector<int32_t>::iterator it = std::find(m_ChildrenEntityID.begin(), m_ChildrenEntityID.end(), childID);
    if (it != m_ChildrenEntityID.end()) {
        m_ChildrenEntityID.erase(it);
    }
}

void RelationshipCom::RemoveAllChildren() {
    m_ChildrenEntityID.clear();
}

};  // namespace entity