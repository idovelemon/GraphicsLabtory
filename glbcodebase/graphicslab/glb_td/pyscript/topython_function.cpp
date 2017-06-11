//------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017 / 05 / 23
// Brief: Interface that will be used in python file
//------------------------------------------------------------------
#include "topython_function.h"

#include <stdio.h>
#include <stdint.h>

#include "../gametimer.h"

#include "glbinput.h"
#include "scene/glbobject.h"
#include "scene/glbscene.h"
#include "util/glbmacro.h"

#include "../entity/arsenalcom.h"
#include "../entity/cameracom.h"
#include "../entity/collisioncom.h"
#include "../entity/datacom.h"
#include "../entity/entitymgr.h"
#include "../entity/entityfilter.h"
#include "../entity/rendercom.h"
#include "../entity/rolecom.h"
#include "../entity/scriptcom.h"
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

void EntitySetDead(int id, int dead) {
    entity::Entity* ent = entity::EntityMgr::GetEntity(id);
    if (ent != NULL) {
        if (dead != 0) {
            ent->SetDead(true);
        } else {
            ent->SetDead(false);
        }
    } else {
        printf("Wrong entity id\n");
        assert(false);
    }
}

int EntityIsDead(int id) {
    int result = 0;

    entity::Entity* ent = entity::EntityMgr::GetEntity(id);
    if (ent != NULL) {
        if (ent->IsDead()) {
            result = 1;
        }
    } else {
        result = 1;
        printf("Warning: Please make sure the id is correct\n");
    }

    return result;
}

void EntityAddRefCount(int id) {
    entity::Entity* ent = entity::EntityMgr::GetEntity(id);
    if (ent != NULL) {
        ent->AddRefCount();
    } else {
        printf("Wrong entity id\n");
        assert(false);
    }
}

void EntityDecRefCount(int id) {
    entity::Entity* ent = entity::EntityMgr::GetEntity(id);
    if (ent != NULL) {
        ent->DecRefCount();
    } else {
        printf("Wrong entity id\n");
        assert(false);
    }
}

void EntityAddTransformCom(int id,
                           float px, float py, float pz,
                           float rx, float ry, float rz,
                           float sx, float sy, float sz) {
    entity::Entity* ent = entity::EntityMgr::GetEntity(id);
    if (ent != NULL) {
        entity::Component* com = new entity::TransformCom(ent, glb::math::Vector(px, py, pz),
            glb::math::Vector(rx, ry, rz),
            glb::math::Vector(sx, sy, sz));
        ent->AddComponent(com);
    } else {
        printf("Wrong entity id\n");
        assert(false);
    }
}

void EntityAddRenderCom(int id, const char* name,
                        float px, float py, float pz,
                        float rx, float ry, float rz,
                        float sx, float sy, float sz) {
    entity::Entity* ent = entity::EntityMgr::GetEntity(id);
    if (ent != NULL) {
        entity::RenderCom* com = new entity::RenderCom(ent, name, glb::math::Vector(px, py, pz), glb::math::Vector(rx, ry, rz), glb::math::Vector(sx, sy, sz));
        com->SetDepthTestEnable(true);
        ent->AddComponent(com);
    } else {
        printf("Wrong entity id\n");
        assert(false);
    }
}

void EntityAddCameraCom(int id, float px, float py, float pz, float tx, float ty, float tz) {
    entity::Entity* ent = entity::EntityMgr::GetEntity(id);
    if (ent != NULL) {
        entity::Component* com = new entity::CameraCom(ent, glb::math::Vector(px, py, pz), glb::math::Vector(tx, ty, tz));
        ent->AddComponent(com);
    } else {
        printf("Wrong entity id\n");
        assert(false);
    }
}

void EntityAddScriptCom(int id, const char* script) {
    entity::Entity* ent = entity::EntityMgr::GetEntity(id);
    if (ent != NULL) {
        entity::ScriptCom* com = new entity::ScriptCom(ent, script);
        ent->AddComponent(com);
    } else {
        printf("Wrong entity id\n");
        assert(false);
    }
}

void EntityAddRoleCom(int id, int mt, int st) {
    entity::Entity* ent = entity::EntityMgr::GetEntity(id);
    if (ent != NULL) {
        entity::RoleCom* com = new entity::RoleCom(ent, static_cast<entity::RoleMainType>(mt), static_cast<entity::RoleSubType>(st));
        ent->AddComponent(com);
    } else {
        printf("Wrong entity id\n");
        assert(false);
    }
}

void EntityAddArsenalCom(int id) {
    entity::Entity* ent = entity::EntityMgr::GetEntity(id);
    if (ent != NULL) {
        entity::ArsenalCom* com = new entity::ArsenalCom(ent);
        ent->AddComponent(com);
    } else {
        printf("Wrong entity id\n");
        assert(false);
    }
}

void EntityAddDataCom(int id) {
    entity::Entity* ent = entity::EntityMgr::GetEntity(id);
    if (ent != NULL) {
        entity::DataCom* com = new entity::DataCom(ent);
        ent->AddComponent(com);
    } else {
        printf("Wrong entity id\n");
        assert(false);
    }
}

void EntityAddCollisionCom(int id) {
    entity::Entity* ent = entity::EntityMgr::GetEntity(id);
    if (ent != NULL) {
        entity::CollisionCom* com = new entity::CollisionCom(ent);
        ent->AddComponent(com);
    } else {
        printf("Wrong entity id\n");
        assert(false);
    }
}

float EntityGetPosX(int id) {
    float x = 0.0f;

    entity::Entity* ent = entity::EntityMgr::GetEntity(id);
    if (ent != NULL) {
        entity::TransformCom* com = reinterpret_cast<entity::TransformCom*>(ent->GetComponent(entity::CT_TRANSFORM));
        if (com != NULL) {
            x = com->GetPos().x;
        }
    } else {
        printf("Wrong entity id\n");
        assert(false);
    }

    return x;
}

float EntityGetPosY(int id) {
    float y = 0.0f;

    entity::Entity* ent = entity::EntityMgr::GetEntity(id);
    if (ent != NULL) {
        entity::TransformCom* com = reinterpret_cast<entity::TransformCom*>(ent->GetComponent(entity::CT_TRANSFORM));
        if (com != NULL) {
            y = com->GetPos().y;
        }
    } else {
        printf("Wrong entity id\n");
        assert(false);
    }

    return y;
}

float EntityGetPosZ(int id) {
    float z = 0.0f;

    entity::Entity* ent = entity::EntityMgr::GetEntity(id);
    if (ent != NULL) {
        entity::TransformCom* com = reinterpret_cast<entity::TransformCom*>(ent->GetComponent(entity::CT_TRANSFORM));
        if (com != NULL) {
            z = com->GetPos().z;
        }
    } else {
        printf("Wrong entity id\n");
        assert(false);
    }

    return z;
}

void EntitySetPos(int id, float x, float y, float z) {
    entity::Entity* ent = entity::EntityMgr::GetEntity(id);
    if (ent != NULL) {
        entity::TransformCom* com = reinterpret_cast<entity::TransformCom*>(ent->GetComponent(entity::CT_TRANSFORM));
        if (com != NULL) {
            com->SetPos(glb::math::Vector(x, y, z));
        }
    } else {
        printf("Wrong entity id\n");
        assert(false);
    }
}

float EntityGetScaleX(int id) {
    float x = 1.0f;
    entity::Entity* ent = entity::EntityMgr::GetEntity(id);
    if (ent != NULL) {
        entity::TransformCom* com = reinterpret_cast<entity::TransformCom*>(ent->GetComponent(entity::CT_TRANSFORM));
        if (com != NULL) {
            x = com->GetScale().x;
        }
    } else {
        printf("Wrong entity id\n");
        assert(false);
    }

    return x;
}

float EntityGetScaleY(int id) {
    float y = 1.0f;
    entity::Entity* ent = entity::EntityMgr::GetEntity(id);
    if (ent != NULL) {
        entity::TransformCom* com = reinterpret_cast<entity::TransformCom*>(ent->GetComponent(entity::CT_TRANSFORM));
        if (com != NULL) {
            y = com->GetScale().y;
        }
    } else {
        printf("Wrong entity id\n");
        assert(false);
    }

    return y;
}

float EntityGetScaleZ(int id) {
    float z = 1.0f;
    entity::Entity* ent = entity::EntityMgr::GetEntity(id);
    if (ent != NULL) {
        entity::TransformCom* com = reinterpret_cast<entity::TransformCom*>(ent->GetComponent(entity::CT_TRANSFORM));
        if (com != NULL) {
            z = com->GetScale().z;
        }
    } else {
        printf("Wrong entity id\n");
        assert(false);
    }

    return z;
}

void EntitySetScale(int id, float x, float y, float z) {
    entity::Entity* ent = entity::EntityMgr::GetEntity(id);
    if (ent != NULL) {
        entity::TransformCom* com = reinterpret_cast<entity::TransformCom*>(ent->GetComponent(entity::CT_TRANSFORM));
        if (com != NULL) {
            com->SetScale(glb::math::Vector(x, y, z));
        }
    } else {
        printf("Wrong entity id\n");
        assert(false);
    }
}

void EntityAddWeapon(int id, int wt, const char* script) {
    entity::Entity* ent = entity::EntityMgr::GetEntity(id);
    if (ent != NULL) {
        entity::ArsenalCom* com = reinterpret_cast<entity::ArsenalCom*>(ent->GetComponent(entity::CT_ARSENAL));
        if (com != NULL) {
            entity::Weapon* weapon = new entity::Weapon(static_cast<entity::WeaponType>(wt), com, script);
            com->AddWeapon(weapon);
        }
    } else {
        printf("Wrong entity id\n");
        assert(false);
    }
}

void EntityActiveWeapon(int id, int wt) {
    entity::Entity* ent = entity::EntityMgr::GetEntity(id);
    if (ent != NULL) {
        entity::ArsenalCom* com = reinterpret_cast<entity::ArsenalCom*>(ent->GetComponent(entity::CT_ARSENAL));
        if (com != NULL) {
            com->ActiveWeapon(static_cast<entity::WeaponType>(wt));
        }
    } else {
        printf("Wrong entity id\n");
        assert(false);
    }
}

int EntityGetWeaponBulletNum(int id) {
    int num = 0;

    entity::Entity* ent = entity::EntityMgr::GetEntity(id);
    if (ent != NULL) {
        entity::ArsenalCom* com = reinterpret_cast<entity::ArsenalCom*>(ent->GetComponent(entity::CT_ARSENAL));
        if (com != NULL) {
            entity::Weapon* weapon = com->GetActiveWeapon();
            num = weapon->GetBulletNum();
        }
    } else {
        printf("Wrong entity id\n");
        assert(false);
    }

    return num;
}

void EntitySetWeaponBulletNum(int id, int num) {
    entity::Entity* ent = entity::EntityMgr::GetEntity(id);
    if (ent != NULL) {
        entity::ArsenalCom* com = reinterpret_cast<entity::ArsenalCom*>(ent->GetComponent(entity::CT_ARSENAL));
        if (com != NULL) {
            entity::Weapon* weapon = com->GetActiveWeapon();
            weapon->SetBulletNum(num);
        }
    } else {
        printf("Wrong entity id\n");
        assert(false);
    }
}

void EntityMoveToTarget(int id, float speed, float tx, float ty, float tz) {
    entity::Entity* ent = entity::EntityMgr::GetEntity(id);
    if (ent != NULL) {
        entity::TransformCom* transform = reinterpret_cast<entity::TransformCom*>(ent->GetComponent(entity::CT_TRANSFORM));
        glb::math::Vector pos = transform->GetPos();
        glb::math::Vector target(tx, ty, tz);
        glb::math::Vector dir = target - pos;
        dir.Normalize();
        pos = pos + dir * speed * td::GameTimer::GetFrameSpeed();
        transform->SetPos(pos);
    } else {
        printf("Wrong entity id\n");
        assert(false);
    }
}

int EntityFindCloestEnemy(int id) {
    int result = -1;

    entity::Entity* ent = entity::EntityMgr::GetEntity(id);
    if (ent != NULL) {
        std::vector<void*> args;
        args.push_back(ent);
        std::vector<entity::Entity*> ents = entity::EntityMgr::FindEntities(entity::ClosestEnemyFilter, args);
        if (ents[0] != NULL) {
            result = ents[0]->GetID();
        }
    } else {
        printf("Wrong entity id\n");
        assert(false);
    }

    return result;
}

void EntityShoot(int id) {
    entity::Entity* player = entity::EntityMgr::GetEntity(id);
    if (player != NULL) {
        entity::ArsenalCom* com = reinterpret_cast<entity::ArsenalCom*>(player->GetComponent(entity::CT_ARSENAL));
        if (com != NULL) {
            entity::Weapon* weapon = com->GetActiveWeapon();
            weapon->Shoot();
        } else {
            assert(false);  // No arsenal component
        }
    } else {
        printf("Wrong entity id\n");
        assert(false);
    }
}

void EntityAddIntData(int id, const char* name, int data) {
    entity::Entity* ent = entity::EntityMgr::GetEntity(id);
    if (ent != NULL) {
        entity::DataCom* com = reinterpret_cast<entity::DataCom*>(ent->GetComponent(entity::CT_DATA));
        if (com != NULL) {
            com->AddData(name, entity::DataPack(data, 0.0f));
        } else {
            assert(false);  // No data component
        }
    } else {
        printf("Wrong entity id\n");
        assert(false);
    }
}

void EntityAddFloatData(int id, const char* name, float data) {
    entity::Entity* ent = entity::EntityMgr::GetEntity(id);
    if (ent != NULL) {
        entity::DataCom* com = reinterpret_cast<entity::DataCom*>(ent->GetComponent(entity::CT_DATA));
        if (com != NULL) {
            com->AddData(name, entity::DataPack(0, data));
        } else {
            assert(false);  // No data component
        }
    } else {
        printf("Wrong entity id\n");
        assert(false);
    }
}

int EntityGetIntData(int id, const char* name) {
    int result = 0;

    entity::Entity* ent = entity::EntityMgr::GetEntity(id);
    if (ent != NULL) {
        entity::DataCom* com = reinterpret_cast<entity::DataCom*>(ent->GetComponent(entity::CT_DATA));
        if (com != NULL) {
            entity::DataPack* pack = com->GetData(name);
            if (pack != NULL) {
                result = pack->GetInt();
            }
        } else {
            assert(false);  // No data component
        }
    } else {
        printf("Wrong entity id\n");
        assert(false);
    }

    return result;
}

float EntityGetFloatData(int id, const char* name) {
    float result = 0;

    entity::Entity* ent = entity::EntityMgr::GetEntity(id);
    if (ent != NULL) {
        entity::DataCom* com = reinterpret_cast<entity::DataCom*>(ent->GetComponent(entity::CT_DATA));
        if (com != NULL) {
            entity::DataPack* pack = com->GetData(name);
            if (pack != NULL) {
                result = pack->GetFloat();
            }
        } else {
            assert(false);  // No data component
        }
    } else {
        printf("Wrong entity id\n");
        assert(false);
    }

    return result;
}

void EntitySetIntData(int id, const char* name, int data) {
    entity::Entity* ent = entity::EntityMgr::GetEntity(id);
    if (ent != NULL) {
        entity::DataCom* com = reinterpret_cast<entity::DataCom*>(ent->GetComponent(entity::CT_DATA));
        if (com != NULL) {
            entity::DataPack* pack = com->GetData(name);
            if (pack != NULL) {
                pack->SetInt(data);
            }
        } else {
            assert(false);  // No data component
        }
    } else {
        printf("Wrong entity id\n");
        assert(false);
    }
}

void EntitySetFloatData(int id, const char* name, float data) {
    entity::Entity* ent = entity::EntityMgr::GetEntity(id);
    if (ent != NULL) {
        entity::DataCom* com = reinterpret_cast<entity::DataCom*>(ent->GetComponent(entity::CT_DATA));
        if (com != NULL) {
            entity::DataPack* pack = com->GetData(name);
            if (pack != NULL) {
                pack->SetFloat(data);
            }
        } else {
            assert(false);  // No data component
        }
    } else {
        printf("Wrong entity id\n");
        assert(false);
    }
}

void EntitySetRotateByLookVec(int id, float x, float y, float z) {
    entity::Entity* ent = entity::EntityMgr::GetEntity(id);
    if (ent != NULL) {
        entity::TransformCom* com = reinterpret_cast<entity::TransformCom*>(ent->GetComponent(entity::CT_TRANSFORM));
        if (com != NULL) {
            glb::math::Vector look(x, y, z);
            look.Normalize();
            glb::math::Vector rot = glb::math::CalcRotateByLookVecZXY(look);
            com->SetRotate(rot);
        } else {
            assert(false);  // No transform component
        }
    } else {
        printf("Wrong entity id\n");
        assert(false);
    }
}

void EntitySetScaleByLookVec(int id, float x, float y, float z) {
    entity::Entity* ent = entity::EntityMgr::GetEntity(id);
    if (ent != NULL) {
        entity::TransformCom* com = reinterpret_cast<entity::TransformCom*>(ent->GetComponent(entity::CT_TRANSFORM));
        if (com != NULL) {
            glb::math::Vector look(x, y, z);
            float length = look.Length();
            com->SetScale(glb::math::Vector(1.0f, 1.0f, length));
        } else {
            assert(false);  // No transform component
        }
    } else {
        printf("Wrong entity id\n");
        assert(false);
    }
}

float EntityCalcDistBetweenEntities(int e0, int e1) {
    float dist = 0.0f;

    entity::Entity* ent0 = entity::EntityMgr::GetEntity(e0);
    entity::Entity* ent1 = entity::EntityMgr::GetEntity(e1);
    if (ent0 != NULL && ent1 != NULL) {
        entity::TransformCom* com0 = reinterpret_cast<entity::TransformCom*>(ent0->GetComponent(entity::CT_TRANSFORM));
        entity::TransformCom* com1 = reinterpret_cast<entity::TransformCom*>(ent1->GetComponent(entity::CT_TRANSFORM));
        if (com0 != NULL && com1 != NULL) {
            glb::math::Vector v0 = com0->GetPos();
            glb::math::Vector v1 = com1->GetPos();
            dist = (v0 - v1).Length();
        } else {
            assert(false);
        }
    } else {
        printf("Wrong entity id\n");
        assert(false);
    }

    return dist;
}

void EntitySetDrawEnable(int id, int enable) {
    entity::Entity* ent = entity::EntityMgr::GetEntity(id);
    if (ent != NULL) {
        entity::RenderCom* com = reinterpret_cast<entity::RenderCom*>(ent->GetComponent(entity::CT_RENDER));
        if (com != NULL) {
            if (enable != 0) {
                com->SetDrawEnable(true);
            } else {
                com->SetDrawEnable(false);
            }
        } else {
            printf("No render component\n");
            assert(false);
        }
    } else {
        printf("Wrong entity id\n");
        assert(false);
    }
}

void EntityBeginIterate() {
    entity::EntityMgr::BeginIterate();
}

int EntityIterate() {
    return entity::EntityMgr::Iterate();
}

void EntityEndIterate() {
    entity::EntityMgr::EndIterate();
}

int EntityIsMainType(int id, int main_type) {
    int result = 0;

    entity::Entity* ent = entity::EntityMgr::GetEntity(id);
    if (ent != NULL) {
        entity::RoleCom* com = reinterpret_cast<entity::RoleCom*>(ent->GetComponent(entity::CT_ROLETYPE));
        if (com != NULL) {
            if (com->GetMainType() == main_type) {
                result = 1;
            }
        } // else {
        //    assert(false);  // No role type component
        //}
        // TODO: Some entities do not have role type now
    } else {
        printf("Wrong entity id\n");
        assert(false);
    }

    return result;
}

int EntityIsSubType(int id, int sub_type) {
    int result = 0;

    entity::Entity* ent = entity::EntityMgr::GetEntity(id);
    if (ent != NULL) {
        entity::RoleCom* com = reinterpret_cast<entity::RoleCom*>(ent->GetComponent(entity::CT_ROLETYPE));
        if (com != NULL) {
            if (com->GetSubType() == sub_type) {
                result = 1;
            }
        } // else {
        //    assert(false);  // No role type component
        //}
        // TODO: Some entities do not have role type now
    } else {
        printf("Wrong entity id\n");
        assert(false);
    }

    return result;
}

void EntityUpdateCollision(int id) {
    entity::Entity* ent = entity::EntityMgr::GetEntity(id);
    if (ent != NULL) {
        entity::CollisionCom* com = reinterpret_cast<entity::CollisionCom*>(ent->GetComponent(entity::CT_COLLISION));
        if (com != NULL) {
            com->Update();
        }
    } else {
        printf("Wrong entity id\n");
        assert(false);
    }
}

void EntityCheckCollision(int id) {
    entity::Entity* ent = entity::EntityMgr::GetEntity(id);
    if (ent != NULL) {
        entity::CollisionCom* com = reinterpret_cast<entity::CollisionCom*>(ent->GetComponent(entity::CT_COLLISION));
        if (com != NULL) {
            com->CheckCollision();
        }
    } else {
        printf("Wrong entity id\n");
        assert(false);
    }
}

void EntityCollisionBeginIterate(int id) {
    entity::Entity* ent = entity::EntityMgr::GetEntity(id);
    if (ent != NULL) {
        entity::CollisionCom* com = reinterpret_cast<entity::CollisionCom*>(ent->GetComponent(entity::CT_COLLISION));
        if (com != NULL) {
            com->BeginIterate();
        }
    } else {
        printf("Wrong entity id\n");
        assert(false);
    }
}

int EntityCollisionIterate(int id) {
    int result = -1;

    entity::Entity* ent = entity::EntityMgr::GetEntity(id);
    if (ent != NULL) {
        entity::CollisionCom* com = reinterpret_cast<entity::CollisionCom*>(ent->GetComponent(entity::CT_COLLISION));
        if (com != NULL) {
            result = com->Iterate();
        }
    } else {
        printf("Wrong entity id\n");
        assert(false);
    }

    return result;
}

void EntityCollisionEndIterate(int id) {
    entity::Entity* ent = entity::EntityMgr::GetEntity(id);
    if (ent != NULL) {
        entity::CollisionCom* com = reinterpret_cast<entity::CollisionCom*>(ent->GetComponent(entity::CT_COLLISION));
        if (com != NULL) {
            com->EndIterate();
        }
    } else {
        printf("Wrong entity id\n");
        assert(false);
    }
}

float EntityGetCollisionWidth(int id) {
    float result = 0.0f;

    entity::Entity* ent = entity::EntityMgr::GetEntity(id);
    if (ent != NULL) {
        entity::CollisionCom* com = reinterpret_cast<entity::CollisionCom*>(ent->GetComponent(entity::CT_COLLISION));
        if (com != NULL) {
            com->EndIterate();
            result = com->GetWidth();
        }
    } else {
        printf("Wrong entity id\n");
        assert(false);
    }

    return result;
}

float EntityGetCollisionLength(int id) {
    float result = 0.0f;

    entity::Entity* ent = entity::EntityMgr::GetEntity(id);
    if (ent != NULL) {
        entity::CollisionCom* com = reinterpret_cast<entity::CollisionCom*>(ent->GetComponent(entity::CT_COLLISION));
        if (com != NULL) {
            com->EndIterate();
            result = com->GetLength();
        }
    } else {
        printf("Wrong entity id\n");
        assert(false);
    }

    return result;
}

//-----------------------------------------------------------------
// Time
float TimeGetPrevGameTime() {
    return td::GameTimer::GetPrevGameTime();
}

float TimeGetCurGameTime() {
    return td::GameTimer::GetCurGameTime();
}

float TimeGetGameFrameSpeed() {
    return td::GameTimer::GetFrameSpeed();
}

//-----------------------------------------------------------------
// Input
int InputGetPointAtEntity() {
    return 0;
}

float InputGetCursorPosX() {
    return glb::Input::GetMousePosX();
}

float InputGetCursorPosY() {
    return glb::Input::GetMousePosY();
}

int InputHasKeyPressed(int key) {
    int pressed = 0;

    if (glb::Input::IsKeyboardButtonPressed(static_cast<glb::ButtonKey>(key))) {
        pressed = 1;
    }

    return pressed;
}

int InputHasMousePressed(int mouse) {
    int pressed = 0;

    if (glb::Input::IsMouseButtonPressed(static_cast<glb::ButtonMouse>(mouse))) {
        pressed = 1;
    }

    return pressed;
}