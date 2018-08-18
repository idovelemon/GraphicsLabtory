//----------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017/05/25
// Brief: Every object (like player, enemy, ui...) in game is an entity
//----------------------------------------------------------------
#include "entity.h"

#include <assert.h>

#include "entitymgr.h"
#include "relationshipcom.h"
#include "rendercom.h"
#include "scriptcom.h"
#include "transformcom.h"

namespace entity {

Entity::Entity()
: m_IsDead(false)
, m_ID(-1)
, m_RefCount(0) {
    m_Components.clear();
}

Entity::~Entity() {
    ComponentPool::iterator it = m_Components.begin();
    for (; it != m_Components.end(); ++it) {
        Component* com = it->second;
        if (com != NULL) {
            delete com;
            com = NULL;
        }
    }
    m_Components.clear();
}

void Entity::SetDead(bool dead, int32_t killType) {
    m_IsDead = dead;

    // Find relationship component
    RelationshipCom* relationShip = reinterpret_cast<RelationshipCom*>(GetComponent(CT_RELATIONSHIP));
    if (relationShip) {
        // Remove from parent
        int32_t parentID = relationShip->GetParent();
        if (parentID != -1) {
            relationShip->SetParent(-1);

            Entity* parentEntity = EntityMgr::GetEntity(parentID);
            if (parentEntity) {
                RelationshipCom* parentRelationShip = reinterpret_cast<RelationshipCom*>(GetComponent(CT_RELATIONSHIP));
                parentRelationShip->RemoveChild(m_ID);
            }
        }

        // If need kill all childrens
        if (killType == KILL_ALL) {
            const std::vector<int32_t>& childrenID = relationShip->GetChildren();

            for (auto childID : childrenID) {
                Entity* childEntity = EntityMgr::GetEntity(childID);
                if (childEntity) {
                    childEntity->SetDead(dead, KILL_ALL);
                }
            }

            relationShip->RemoveAllChildren();
        }
    }
}

bool Entity::IsDead() {
    return m_IsDead;
}

void Entity::SetID(int32_t id) {
    m_ID = id;
}

int32_t Entity::GetID() const {
    return m_ID;
}

void Entity::AddRefCount() {
    m_RefCount++;
}

void Entity::DecRefCount() {
    m_RefCount--;
}

int32_t Entity::GetRefCount() const {
    return m_RefCount;
}

void Entity::AddComponent(Component* com) {
    if (com != NULL) {
        ComponentType type = com->GetType();
        m_Components.insert(std::pair<ComponentType, Component*>(type, com));
    } else {
        assert(false);
    }
}

Component* Entity::GetComponent(ComponentType type) {
    Component* com = NULL;

    if (CT_UNKNOWN < type && type < CT_MAX) {
        ComponentPool::iterator it = m_Components.end();
        ComponentPool::iterator com_it = m_Components.find(type);
        if (com_it != it) {
            com = com_it->second;
        }
    } else {
        assert(false);
    }

    return com;
}

void Entity::RemoveComponent(ComponentType type) {
    if (CT_UNKNOWN < type && type < CT_MAX) {
        ComponentPool::iterator it = m_Components.find(type);
        if (it != m_Components.end()) {
            Component* com = it->second;
            if (com != NULL) {
                delete com;
                com = NULL;
            }
            m_Components.erase(it);
        }
    } else {
        assert(false);
    }
}

void Entity::Update(float dt) {
    //ComponentPool::iterator it = m_Components.find(CT_SCRIPT);
    //if (it != m_Components.end()) {
    //    Component* com = it->second;
    //    if (com != NULL) {
    //        ScriptCom* script = reinterpret_cast<ScriptCom*>(com);
    //        script->Update(dt);
    //    }
    //}
    for (ComponentPool::iterator it = m_Components.begin(); it != m_Components.end(); ++it) {
        Component* com = it->second;
        if (com != NULL) {
            com->Update();
        }
    }
}

void Entity::Draw() {
    ComponentPool::iterator it_render = m_Components.find(CT_RENDER);
    ComponentPool::iterator it_transform = m_Components.find(CT_TRANSFORM);
    if (it_render != m_Components.end() && it_transform != m_Components.end()) {
        if (it_render->second != NULL && it_transform->second != NULL) {
            RenderCom* render = reinterpret_cast<RenderCom*>(it_render->second);
            TransformCom* transform = reinterpret_cast<TransformCom*>(it_transform->second);
            render->Render(transform);
        }
    }
}

};  // namespace entity