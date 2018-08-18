//--------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017/05/28
// Brief: Entity Filter used to find specific entities
//--------------------------------------------------------------------
#ifndef ENTITY_ENTITYFILTER_H_
#define ENTITY_ENTITYFILTER_H_

#include <vector>

namespace entity {

//--------------------------------------------------------------
// Pre Declaration
class Entity;

//--------------------------------------------------------------
// Function Declaration
std::vector<Entity*> TagNameEntityFilter(Entity** entity, int num, std::vector<void*>& args);

};  // namespace entity

#endif  // ENTITY_ENTITYFILTER_H_