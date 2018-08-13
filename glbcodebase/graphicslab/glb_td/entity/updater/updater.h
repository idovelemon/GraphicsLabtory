//-------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2018. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2018/06/21
// Brief: All entity's updater method
//-------------------------------------------------------------
#ifndef ENTITY_UPDATER_H_
#define ENTITY_UPDATER_H_

namespace entity {

//-------------------------------------------------------------
class Entity;



//-------------------------------------------------------------
#ifdef UPDATER_DEF
#undef UPDATER_DEF
#endif

#define UPDATER_DEF(_name_) void _name_(Entity* entity);
#include "updater.def"

#ifdef UPDATER_DEF
#undef UPDATER_DEF
#endif

#define UPDATER_DEF(_name_) {#_name_, _name_},

typedef void (*update_func__)(Entity*);
static const struct {
    char name[128];
    update_func__ f;
} sEntityUpdaterTbl[] = {
#include "updater.def"
};

};  // namespace entity

#endif  // ENTITY_UPDATER_H_