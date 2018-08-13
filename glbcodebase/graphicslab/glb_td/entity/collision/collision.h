//-------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2018. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2018/08/13
// Brief: All entity's collision handle method
//-------------------------------------------------------------
#ifndef ENTITY_COLLISION_HANDLE_H_
#define ENTITY_COLLISION_HANDLE_H_

#include "../../dynamic/dynamicobject.h"

namespace entity {

//-------------------------------------------------------------
class Entity;
class DynamicObject;


//-------------------------------------------------------------
#ifdef COLLISION_HANDLE_DEF
#undef COLLISION_HANDLE_DEF
#endif

#define COLLISION_HANDLE_DEF(_name_) void _name_(dynamic::DynamicObject*, dynamic::DynamicObject*);
#include "collision.def"

#ifdef COLLISION_HANDLE_DEF
#undef COLLISION_HANDLE_DEF
#endif

#define COLLISION_HANDLE_DEF(_name_) {#_name_, _name_},

static const struct {
    char name[128];
    dynamic::DynamicObject::CollisionEventHandle f;
} sEntityCollisionHandleTbl[] = {
#include "collision.def"
};

};  // namespace entity

#endif  // ENTITY_UPDATER_H_