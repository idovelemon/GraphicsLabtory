//-------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017/05/29
// Brief: Arsenal hold all the weapons that an entity have
//-------------------------------------------------------------
#ifndef ENTITY_ARSENALCOM_H_
#define ENTITY_ARSENALCOM_H_

#include "component.h"

#include <stdint.h>
#include <vector>

namespace entity {

//-------------------------------------------------------------
// Pre Declaration
class Weapon;

//-------------------------------------------------------------
// Type Declaration

//-------------------------------------------------------------
enum WeaponType {
    WT_NONE = -1,
    WT_LASER,
    WT_MACHINEGUN,
    WT_MAX,
};

//-------------------------------------------------------------
class Weapon {
public:
    Weapon(WeaponType wt, Component* owner, const char* script);
    virtual ~Weapon();

public:
    virtual WeaponType GetType() const;
    virtual void Shoot();
    virtual int GetBulletNum() const;
    virtual void SetBulletNum(int num);

protected:
    static const int32_t kScriptFileNameMaxLen = 128;
    WeaponType  m_Type;
    Component*  m_Owner;
    int         m_BulletNum;
    char        m_Script[kScriptFileNameMaxLen];
};

//-------------------------------------------------------------
class ArsenalCom : public Component {
public:
    ArsenalCom(Entity* entity);
    virtual ~ArsenalCom();

public:
    void AddWeapon(Weapon* weapon);
    void ActiveWeapon(WeaponType wt);
    Weapon* GetActiveWeapon();

protected:
    std::vector<Weapon*>        m_Arsenal;
    WeaponType                  m_ActiveWeapon;
};

};  // namespace entity

#endif  // ENTITY_ARSENALCOM_H_