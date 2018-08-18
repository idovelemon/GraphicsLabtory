//--------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017/05/28
// Brief: Entity Filter used to find specific entities
//--------------------------------------------------------------------
#include "entityfilter.h"

#include "datacom.h"
#include "entity.h"
#include "transformcom.h"

namespace entity {

std::vector<Entity*> TagNameEntityFilter(Entity** entity, int32_t num, std::vector<void*>& args) {
    std::vector<Entity*> result;

    const char* tag = reinterpret_cast<const char*>(args[0]);

    for (int32_t i = 0; i < num; i++) {
        if (entity[i] != NULL) {
            DataCom* data = reinterpret_cast<DataCom*>(entity[i]->GetComponent(CT_DATA));
            if (data == NULL) continue;

            DataPack* pack = data->GetData("Tag");
            if (pack != NULL && !pack->GetString().compare(tag)) {
                result.push_back(entity[i]);
            }
        }
    }

    return result;
}

};  // namespace entity