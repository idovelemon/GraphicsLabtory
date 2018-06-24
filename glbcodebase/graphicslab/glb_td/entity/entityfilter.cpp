//--------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017/05/28
// Brief: Entity Filter used to find specific entities
//--------------------------------------------------------------------
#include "entityfilter.h"

#include "datacom.h"
#include "entity.h"
#include "rolecom.h"
#include "transformcom.h"

namespace entity {

std::vector<Entity*> ClosestEnemyFilter(Entity** entity, int32_t num, std::vector<void*>& args) {
    std::vector<Entity*> result;
    result.push_back(NULL);

    Entity* ent = reinterpret_cast<Entity*>(args[0]);
    if (ent == NULL) return result;
    TransformCom* com = reinterpret_cast<TransformCom*>(ent->GetComponent(CT_TRANSFORM));
    if (com == NULL) return result;

    glb::math::Vector pos = com->GetPos();

    float min_len = FLT_MAX;
    Entity* closest_entity = NULL;

    for (int32_t i = 0; i < num; i++) {
        if (entity[i] != NULL) {
            RoleCom* role_com = reinterpret_cast<RoleCom*>(entity[i]->GetComponent(CT_ROLETYPE));
            if (role_com == NULL) continue;
            if (role_com->GetMainType() != RMT_ENEMY) continue;

            com = reinterpret_cast<TransformCom*>(entity[i]->GetComponent(CT_TRANSFORM));
            if (com == NULL) continue;

            glb::math::Vector pos2 = com->GetPos();
            float l = (pos2 - pos).SqureLength();
            if (l < min_len) {
                min_len = l;
                closest_entity = entity[i];
            }
        }
    }

    result[0] = closest_entity;
    return result;
}

std::vector<Entity*> MatchTypeEntityFilter(Entity** entity, int32_t num, std::vector<void*>& args) {
    std::vector<Entity*> result;

    int main_type = reinterpret_cast<int>(args[0]);
    int sub_type = reinterpret_cast<int>(args[1]);

    for (int32_t i = 0; i < num; i++) {
        if (entity[i] != NULL) {
            RoleCom* role_com = reinterpret_cast<RoleCom*>(entity[i]->GetComponent(CT_ROLETYPE));
            if (role_com == NULL) continue;
            if (role_com->GetMainType() == main_type && role_com->GetSubType() == sub_type) {
                result.push_back(entity[i]);
            }
        }
    }

    return result;
}

std::vector<Entity*> TagNameEntityFilter(Entity** entity, int32_t num, std::vector<void*>& args) {
    std::vector<Entity*> result;

    const char* tag = reinterpret_cast<const char*>(args[0]);

    for (int32_t i = 0; i < num; i++) {
        if (entity[i] != NULL) {
            DataCom* data = reinterpret_cast<DataCom*>(entity[i]->GetComponent(CT_DATA));
            if (data == NULL) continue;

            DataPack* pack = data->GetData("Tag");
            if (pack && !pack->GetString().compare(tag)) {
                result.push_back(entity[i]);
            }
        }
    }

    return result;
}

};  // namespace entity