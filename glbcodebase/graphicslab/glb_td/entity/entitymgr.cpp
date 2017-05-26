//------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017/05/26
// Brief: Manage all the entity in game
//------------------------------------------------------------
#include "entitymgr.h"

#include <assert.h>

namespace entity {

//----------------------------------------------------------------
// Pre Declaration

class EntityMgrImp;
static EntityMgrImp* s_EntityMgrImp;

//----------------------------------------------------------------
// Constant

static const int32_t kEntityMaxNum = 100;

//----------------------------------------------------------------
// Type Declaration

class EntityMgrImp {
public:
    EntityMgrImp();
    virtual ~EntityMgrImp();

public:
    void Initialize();
    void Update(float dt);
    void Destroy();

    int32_t AddEntity(Entity* entity);
    Entity* GetEntity(int32_t id) const;
    void RemoveEntity(int32_t id);

protected:
    int32_t FindEmptyID();

protected:
    Entity*         m_Entities[kEntityMaxNum];
};

//----------------------------------------------------------------
// Type definition

//----------------------------------------------------------------

EntityMgrImp::EntityMgrImp() {
    memset(m_Entities, 0, sizeof(m_Entities));
}

EntityMgrImp::~EntityMgrImp() {
    Destroy();
}

void EntityMgrImp::Initialize() {
}

void EntityMgrImp::Update(float dt) {
    for (int32_t i = 0; i < kEntityMaxNum; i++) {
        if (m_Entities[i] != NULL) {
            m_Entities[i]->Update(dt);
        }
    }
}

void EntityMgrImp::Destroy() {
    for (int32_t i = 0; i < kEntityMaxNum; i++) {
        if (m_Entities[i] != NULL) {
            delete m_Entities[i];
            m_Entities[i] = NULL;
        }
    }
}

int32_t EntityMgrImp::AddEntity(Entity* entity) {
    int32_t id = -1;

    if (entity != NULL) {
        id = FindEmptyID();
        if (id != -1) {
            m_Entities[id] = entity;
            entity->SetID(id);
        } else {
            printf("There is no empty id for entity, please add the capacity");
            assert(false);
        }
    }

    return id;
}

Entity* EntityMgrImp::GetEntity(int32_t id) const {
   Entity* entity = NULL;

   if (0 <= id < kEntityMaxNum) {
       entity = m_Entities[id];
   } else {
       printf("Invalid id");
       assert(false);
   }

   return entity;
}

void EntityMgrImp::RemoveEntity(int32_t id) {
   if (0 <= id < kEntityMaxNum) {
       Entity* entity = m_Entities[id];
       if (entity != NULL) {
           delete entity;
           m_Entities[id] = NULL;
           entity = NULL;
       }
   } else {
       printf("Invalid id");
       assert(false);
   }
}

int32_t EntityMgrImp::FindEmptyID() {
    int32_t id = -1;

    for (int32_t i = 0; i < kEntityMaxNum; i++) {
        if (m_Entities[i] == NULL) {
            id = i;
            break;
        }
    }

    return id;
}

//----------------------------------------------------------------

void EntityMgr::Initialize() {
    if (s_EntityMgrImp == NULL) {
        s_EntityMgrImp = new EntityMgrImp;
        if (s_EntityMgrImp != NULL) {
            s_EntityMgrImp->Initialize();
        } else {
            assert(false);
        }
    }
}

void EntityMgr::Update(float dt) {
    if (s_EntityMgrImp != NULL) {
        s_EntityMgrImp->Update(dt);
    }
}

void EntityMgr::Destroy() {
    if (s_EntityMgrImp != NULL) {
        s_EntityMgrImp->Destroy();
        delete s_EntityMgrImp;
        s_EntityMgrImp = NULL;
    }
}

int32_t EntityMgr::AddEntity(Entity* entity) {
    int32_t id = -1;

    if (s_EntityMgrImp != NULL) {
        id = s_EntityMgrImp->AddEntity(entity);
    }

    return id;
}

Entity* EntityMgr::GetEntity(int32_t id) {
    Entity* entity = NULL;

    if (s_EntityMgrImp != NULL) {
        entity = s_EntityMgrImp->GetEntity(id);
    }

    return entity;
}

void EntityMgr::RemoveEntity(int32_t id) {
    if (s_EntityMgrImp != NULL) {
        s_EntityMgrImp->RemoveEntity(id);
    }
}
};  // namespace entity