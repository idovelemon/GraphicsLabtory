//----------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017/05/25
// Brief: Every object (like player, enemy, ui...) in game is an entity
//----------------------------------------------------------------
#ifndef ENTITY_ENTITY_H_
#define ENTITY_ENTITY_H_

#include <stdint.h>

#include <map>

#include "component.h"

namespace entity {

//--------------------------------------------------------------------
// Class declaration
class Entity {
public:
    enum {
        KILL_ONLY_ME = 0,
        KILL_ALL,
    };

public:
    Entity();
    virtual ~Entity();

public:
    virtual void SetDead(bool dead, int32_t killType = KILL_ALL);
    virtual bool IsDead();
    virtual void SetID(int32_t id);
    virtual int32_t GetID() const;
    virtual void AddRefCount();
    virtual void DecRefCount();
    virtual int32_t GetRefCount() const;
    virtual void AddComponent(Component* com);
    virtual Component* GetComponent(ComponentType type);
    virtual void RemoveComponent(ComponentType type);
    virtual void Update(float dt);
    virtual void Draw();

protected:
    bool            m_IsDead;
    int32_t         m_ID;
    int32_t         m_RefCount;
    typedef std::map<ComponentType, Component*>     ComponentPool;
    ComponentPool   m_Components;
};

};  // namespace entity

#endif  // ENTITY_ENTITY_H_