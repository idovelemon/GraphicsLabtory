//--------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017/06/01
// Brief: Dynamic world hold all dynamic objects
//--------------------------------------------------------------------
#include "dynamicworld.h"

#include <assert.h>
#include <memory.h>

#include "dynamicobject.h"

namespace dynamic {

//---------------------------------------------------------------------
// Pre Declaration
class DynamicWorldImp;
static DynamicWorldImp* s_DynamicWorldImp = nullptr;

static const int32_t kMaxDynamicObject = 10000;

//---------------------------------------------------------------------
// Type Declaration

//---------------------------------------------------------------------
class DynamicWorldImp {
public:
    DynamicWorldImp();
    virtual ~DynamicWorldImp();

public:
    void Initialize();
    void Update();
    void Destroy();
    int32_t AddDynamicObject(DynamicObject* object);
    void RemoveDynamicObject(int32_t id);
    DynamicObject* GetDynamicObject(int32_t id);
    void CheckCollision(int32_t id, std::vector<int32_t>& coll_ids);

protected:
    int32_t FindEmptyId();

protected:
    DynamicObject*      m_DynamicObjects[kMaxDynamicObject];
};

//---------------------------------------------------------------------
// Type Definition

//---------------------------------------------------------------------
DynamicWorldImp::DynamicWorldImp() {
    memset(m_DynamicObjects, 0, sizeof(m_DynamicObjects));
}

DynamicWorldImp::~DynamicWorldImp() {
    Destroy();
}

void DynamicWorldImp::Initialize() {
}

void DynamicWorldImp::Update() {
}

void DynamicWorldImp::Destroy() {
}

int32_t DynamicWorldImp::AddDynamicObject(DynamicObject* object) {
    int32_t id = -1;
    id = FindEmptyId();
    if (id != -1) {
        m_DynamicObjects[id] = object;
    } else {
        assert(false && "No empty id for dynamic object");
    }
    return id;
}

void DynamicWorldImp::RemoveDynamicObject(int32_t id) {
    if (0 <= id && id < kMaxDynamicObject) {
        if (m_DynamicObjects[id] != nullptr) {
            delete m_DynamicObjects[id];
            m_DynamicObjects[id] = nullptr;
        }
    }
}

DynamicObject* DynamicWorldImp::GetDynamicObject(int32_t id) {
    DynamicObject* object = nullptr;

    if (0 <= id && id < kMaxDynamicObject) {
        object = m_DynamicObjects[id];
    }

    return object;
}

void DynamicWorldImp::CheckCollision(int32_t id, std::vector<int32_t>& coll_ids) {
    coll_ids.clear();

    DynamicObject* a = m_DynamicObjects[id];
    for (int32_t i = 0; i < kMaxDynamicObject; i++) {
        DynamicObject* b = m_DynamicObjects[i];
        if (i != id && b != nullptr) {
            if (a->IsIntersection(b)) {
                int32_t entityid = reinterpret_cast<int32_t>(b->GetUserData());
                coll_ids.push_back(entityid);
            }
        }
    }
}

int32_t DynamicWorldImp::FindEmptyId() {
    int32_t id = -1;
    for (int32_t i = 0; i < kMaxDynamicObject; i++) {
        if (m_DynamicObjects[i] == nullptr) {
            id = i;
            break;
        }
    }
    return id;
}

//---------------------------------------------------------------------
void DynamicWorld::Initialize() {
    if (s_DynamicWorldImp == nullptr) {
        s_DynamicWorldImp = new DynamicWorldImp;
        if (s_DynamicWorldImp != nullptr) {
            s_DynamicWorldImp->Initialize();
        }
    }
}

void DynamicWorld::Update() {
    if (s_DynamicWorldImp != nullptr) {
        s_DynamicWorldImp->Update();
    }
}

void DynamicWorld::Destroy() {
    if (s_DynamicWorldImp != nullptr) {
        s_DynamicWorldImp->Destroy();
        delete s_DynamicWorldImp;
        s_DynamicWorldImp = nullptr;
    }
}

int32_t DynamicWorld::AddDynamicObject(DynamicObject* object) {
    int32_t id = -1;

    if (s_DynamicWorldImp != nullptr) {
        id = s_DynamicWorldImp->AddDynamicObject(object);
    }

    return id;
}

void DynamicWorld::RemoveDynamicObject(int32_t id) {
    if (s_DynamicWorldImp != nullptr) {
        s_DynamicWorldImp->RemoveDynamicObject(id);
    }
}

DynamicObject* DynamicWorld::GetDynamicObject(int32_t id) {
    DynamicObject* object = nullptr;

    if (s_DynamicWorldImp != nullptr) {
        object = s_DynamicWorldImp->GetDynamicObject(id);
    }

    return object;
}

void DynamicWorld::CheckCollision(int id, std::vector<int32_t>& coll_ids) {
    if (s_DynamicWorldImp != nullptr) {
        s_DynamicWorldImp->CheckCollision(id, coll_ids);
    }
}
};  // namespace dynamic