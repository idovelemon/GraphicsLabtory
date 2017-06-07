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

static const int32_t kEntityMaxNum = 10000;

//----------------------------------------------------------------
// Type Declaration

class EntityMgrImp {
public:
    EntityMgrImp();
    virtual ~EntityMgrImp();

public:
    void Initialize();
    void Update(float dt);
    void Draw();
    void Destroy();

    int32_t CreateEntity();
    int32_t AddEntity(Entity* entity);
    Entity* GetEntity(int32_t id) const;
    void RemoveEntity(int32_t id);

    std::vector<Entity*> FindEntities(EntityFilter filter, std::vector<void*>& args);

    void BeginIterate();
    int32_t Iterate();
    void EndIterate();

protected:
    int32_t FindEmptyID();

protected:
    int32_t         m_Iterator;
    Entity*         m_Entities[kEntityMaxNum];
};

//----------------------------------------------------------------
// Type definition

//----------------------------------------------------------------

EntityMgrImp::EntityMgrImp()
: m_Iterator(0) {
    memset(m_Entities, 0, sizeof(m_Entities));
}

EntityMgrImp::~EntityMgrImp() {
    Destroy();
}

void EntityMgrImp::Initialize() {
}

void EntityMgrImp::Update(float dt) {
    // Remove dead entities
    for (int32_t i = 0; i < kEntityMaxNum; i++) {
        if (m_Entities[i] != NULL) {
            if (m_Entities[i]->IsDead() && m_Entities[i]->GetRefCount() <= 0) {
                delete m_Entities[i];
                m_Entities[i] = NULL;
            }
        }
    }

    // Update dead entities
    for (int32_t i = 0; i < kEntityMaxNum; i++) {
        if (m_Entities[i] != NULL && !m_Entities[i]->IsDead()) {
            m_Entities[i]->Update(dt);
        }
    }
}

void EntityMgrImp::Draw() {
    for (int32_t i = 0; i < kEntityMaxNum; i++) {
        if (m_Entities[i] != NULL && !m_Entities[i]->IsDead()) {
            m_Entities[i]->Draw();
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

int32_t EntityMgrImp::CreateEntity() {
    int32_t id = -1;

    Entity* entity = new Entity;
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

std::vector<Entity*> EntityMgrImp::FindEntities(EntityFilter filter, std::vector<void*>& args) {
    return filter(m_Entities, kEntityMaxNum, args);
}

void EntityMgrImp::BeginIterate() {
    m_Iterator = 0;
}

int32_t EntityMgrImp::Iterate() {
    int32_t result = -1;

    while (true) {
        if (m_Entities[m_Iterator] != NULL) {
            result = m_Iterator;
            m_Iterator++;
            break;
        }

        m_Iterator++;

        if (m_Iterator >= kEntityMaxNum) {
            break;
        }
    }

    return result;
}

void EntityMgrImp::EndIterate() {
    m_Iterator = 0;
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

void EntityMgr::Draw() {
    if (s_EntityMgrImp != NULL) {
        s_EntityMgrImp->Draw();
    }
}

void EntityMgr::Destroy() {
    if (s_EntityMgrImp != NULL) {
        s_EntityMgrImp->Destroy();
        delete s_EntityMgrImp;
        s_EntityMgrImp = NULL;
    }
}

int32_t EntityMgr::CreateEntity() {
    int32_t id = -1;

    if (s_EntityMgrImp != NULL) {
        id = s_EntityMgrImp->CreateEntity();
    }

    return id;
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

std::vector<Entity*> EntityMgr::FindEntities(EntityFilter filter, std::vector<void*>& args) {
    std::vector<Entity*> ents;

    if (s_EntityMgrImp != NULL) {
        ents = s_EntityMgrImp->FindEntities(filter, args);
    }

    return ents;
}

void EntityMgr::BeginIterate() {
    if (s_EntityMgrImp != NULL) {
        s_EntityMgrImp->BeginIterate();
    }
}

int32_t EntityMgr::Iterate() {
    int32_t id = -1;

    if (s_EntityMgrImp != NULL) {
        id = s_EntityMgrImp->Iterate();
    }

    return id;
}

void EntityMgr::EndIterate() {
    if (s_EntityMgrImp != NULL) {
        s_EntityMgrImp->EndIterate();
    }
}

};  // namespace entity