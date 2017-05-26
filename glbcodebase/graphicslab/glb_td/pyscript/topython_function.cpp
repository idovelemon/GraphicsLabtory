//------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017 / 05 / 23
// Brief: Interface that will be used in python file
//------------------------------------------------------------------
#include "topython_function.h"

#include <stdio.h>
#include <stdint.h>

#include "scene/glbobject.h"
#include "scene/glbscene.h"
#include "util/glbmacro.h"

#include "../entity/cameracom.h"
#include "../entity/entitymgr.h"
#include "../entity/rendercom.h"
#include "../entity/transformcom.h"

//-----------------------------------------------------------------
// Debug
void DebugPrint(const char* s) {
    printf(s);
}

//-----------------------------------------------------------------
// Entity
int EntityCreate() {
    return entity::EntityMgr::CreateEntity();
}

void EntityAddTransformCom(int id,
                           float px, float py, float pz,
                           float rx, float ry, float rz,
                           float sx, float sy, float sz) {
    entity::Entity* ent = entity::EntityMgr::GetEntity(id);
    if (ent != NULL) {
        entity::Component* com = new entity::TransformCom(glb::math::Vector(px, py, pz),
            glb::math::Vector(rx, ry, rz),
            glb::math::Vector(sx, sy, sz));
        ent->AddComponent(com);
    } else {
        printf("Wrong entity id\n");
        assert(false);
    }
}

void EntityAddRenderCom(int id, const char* name) {
    entity::Entity* ent = entity::EntityMgr::GetEntity(id);
    if (ent != NULL) {
        entity::Component* com = new entity::RenderCom(name);
        ent->AddComponent(com);
    } else {
        printf("Wrong entity id\n");
        assert(false);
    }
}

void EntityAddCameraCom(int id, float px, float py, float pz, float tx, float ty, float tz) {
    entity::Entity* ent = entity::EntityMgr::GetEntity(id);
    if (ent != NULL) {
        entity::Component* com = new entity::CameraCom(glb::math::Vector(px, py, pz), glb::math::Vector(tx, ty, tz));
        ent->AddComponent(com);
    } else {
        printf("Wrong entity id\n");
        assert(false);
    }
}