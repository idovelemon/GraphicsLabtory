//----------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017/05/25
// Brief: Every object (like player, enemy, ui...) in game is an entity
//----------------------------------------------------------------
#include "entity.h"

#include <assert.h>

#include "scriptcom.h"

namespace entity {

Entity::Entity()
: m_ID(-1) {
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

void Entity::SetID(int32_t id) {
    m_ID = id;
}

int32_t Entity::GetID() const {
    return m_ID;
}

void Entity::AddComponent(Component* com) {
    if (com != NULL) {
        ComponentType type = com->GetType();
        m_Components.insert(std::pair<ComponentType, Component*>(type, com));
    } else {
        assert(false);
    }
}

Component* Entity::GetComponent(ComponentType type) const {
    Component* com = NULL;

    if (CT_UNKNOWN < type && type < CT_MAX) {
        com = m_Components.find(type)->second;
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
    ComponentPool::iterator it = m_Components.find(CT_SCRIPT);
    if (it != m_Components.end()) {
        Component* com = it->second;
        if (com != NULL) {
            ScriptCom* script = reinterpret_cast<ScriptCom*>(com);
            script->Update(dt);
        }
    }
}

};  // namespace entity