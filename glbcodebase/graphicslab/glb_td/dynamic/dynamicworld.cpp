//--------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017/06/01
// Brief: Dynamic world hold all dynamic objects
//--------------------------------------------------------------------
#include "dynamicworld.h"

#include <assert.h>
#include <memory.h>

#include "btBulletDynamicsCommon.h"
#include "dynamicobject.h"
#include "dynamicdrawer.h"
#include "../log.h"
#include "../version.h"

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
    btDiscreteDynamicsWorld*    m_DynamicsWorld;
    DynamicObject*      m_DynamicObjects[kMaxDynamicObject];
};

//---------------------------------------------------------------------
// Type Definition

//---------------------------------------------------------------------
DynamicWorldImp::DynamicWorldImp()
: m_DynamicsWorld(NULL) {
    memset(m_DynamicObjects, 0, sizeof(m_DynamicObjects));
}

DynamicWorldImp::~DynamicWorldImp() {
    Destroy();
}

void DynamicWorldImp::Initialize() {
	///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	btCollisionDispatcher* dispatcher = new	btCollisionDispatcher(collisionConfiguration);

	///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();

	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

	btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,overlappingPairCache,solver,collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0,-10,0));
    m_DynamicsWorld = dynamicsWorld;

#if ENABLE_DYNAMIC_DRAW
    m_DynamicsWorld->setDebugDrawer(new DynamicDebugDrawer());
#endif
}

void DynamicWorldImp::Update() {
    if (m_DynamicsWorld) {
        m_DynamicsWorld->stepSimulation(1.0f / 60.0f, 10);

        // Define ContactResultCallback
        class DynamicWorldContactResultCallback : public btCollisionWorld::ContactResultCallback {
        public:
            DynamicWorldContactResultCallback(std::vector<btCollisionObject*>& collObjs)
            : m_CollObjs(collObjs) {
            }

            virtual	btScalar addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap,int partId0,int index0,const btCollisionObjectWrapper* colObj1Wrap,int partId1,int index1) {
                // Note: addSingleResult is called for every contact point result
                std::vector<btCollisionObject*>::iterator it = std::find(m_CollObjs.begin(), m_CollObjs.end(), colObj1Wrap->getCollisionObject());
                if (it == m_CollObjs.end()) {
                    m_CollObjs.push_back(const_cast<btCollisionObject*>(colObj1Wrap->getCollisionObject()));
                }
                return 0.0f;
            }

        protected:
            std::vector<btCollisionObject*>&    m_CollObjs;
        };

        // Do collision detection
        for (int32_t i = 0; i < kMaxDynamicObject; i++) {
            if (m_DynamicObjects[i]) {
                std::vector<btCollisionObject*> collObjs;
                DynamicWorldContactResultCallback contactCallback(collObjs);

                contactCallback.m_collisionFilterGroup = m_DynamicObjects[i]->GetCollisionFilterGroup();
                contactCallback.m_collisionFilterMask = m_DynamicObjects[i]->GetCollisionFilterMask();

                m_DynamicsWorld->contactTest(m_DynamicObjects[i]->GetBtCollision(), contactCallback);

                for (int32_t j = 0; j < collObjs.size(); j++) {
                    int32_t id = reinterpret_cast<int32_t>(collObjs[j]->getUserPointer());
                    if (m_DynamicObjects[i]->GetCollisionHandle()) {
                        m_DynamicObjects[i]->GetCollisionHandle()(m_DynamicObjects[i], m_DynamicObjects[id]);
                    }
                }
            }
        }
    }

#if ENABLE_DYNAMIC_DRAW
    if (m_DynamicsWorld) {
        m_DynamicsWorld->debugDrawWorld();
    }
#endif
}

void DynamicWorldImp::Destroy() {
    if (m_DynamicsWorld) {
        delete m_DynamicsWorld;
        m_DynamicsWorld = NULL;
    }
}

int32_t DynamicWorldImp::AddDynamicObject(DynamicObject* object) {
    int32_t id = -1;
    id = FindEmptyId();
    if (id != -1) {
        m_DynamicObjects[id] = object;
        m_DynamicsWorld->addCollisionObject(object->GetBtCollision(), object->GetCollisionFilterGroup(), object->GetCollisionFilterMask());
        object->GetBtCollision()->setUserPointer(reinterpret_cast<void*>(id));
    } else {
        assert(false && "No empty id for dynamic object");
    }
    return id;
}

void DynamicWorldImp::RemoveDynamicObject(int32_t id) {
    if (0 <= id && id < kMaxDynamicObject) {
        if (m_DynamicObjects[id] != nullptr) {
            m_DynamicsWorld->removeCollisionObject(m_DynamicObjects[id]->GetBtCollision());
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