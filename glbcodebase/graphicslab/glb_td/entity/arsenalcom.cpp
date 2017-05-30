//-------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017/05/29
// Brief: Arsenal hold all the weapons that an entity have
//-------------------------------------------------------------
#include "arsenalcom.h"

#include "component.h"
#include "entity.h"
#include "../pyscript/pyscriptmgr.h"

namespace entity {

//-------------------------------------------------------------------
Weapon::Weapon(WeaponType wt, Component* owner, const char* script)
: m_Type(wt)
, m_Owner(owner)
, m_BulletNum(0) {
    int32_t len = strlen(script);
    len = (len > kScriptFileNameMaxLen - 1) ? (kScriptFileNameMaxLen - 1) : len;
    memcpy(m_Script, script, len);
    m_Script[len] = '\0';

    pyscript::PyScriptMgr::LoadScript(m_Script);
}

Weapon::~Weapon() {
    m_Owner = NULL;
}

WeaponType Weapon::GetType() const {
    return m_Type;
}

void Weapon::Shoot() {
    pyscript::PyScriptMgr::RunScript(m_Script, m_Owner->GetOwner()->GetID());
}

int Weapon::GetBulletNum() const {
    return m_BulletNum;
}

void Weapon::SetBulletNum(int num) {
    m_BulletNum = num;
}

//-------------------------------------------------------------------
ArsenalCom::ArsenalCom(Entity* entity)
: Component(CT_ARSENAL, entity)
, m_ActiveWeapon(WT_NONE) {
}

ArsenalCom::~ArsenalCom() {
    int32_t size = m_Arsenal.size();
    for (int32_t i = 0; i < size; i++) {
        delete m_Arsenal[i];
        m_Arsenal[i] = NULL;
    }
    m_Arsenal.clear();
}

void ArsenalCom::AddWeapon(Weapon* weapon) {
    if (weapon != NULL) {
        m_Arsenal.push_back(weapon);
    }
}

void ArsenalCom::ActiveWeapon(WeaponType wt) {
    m_ActiveWeapon = wt;
}

Weapon* ArsenalCom::GetActiveWeapon() {
    Weapon* weapon = NULL;

    int32_t size = m_Arsenal.size();
    for (int32_t i = 0; i < size; i++) {
        if (m_Arsenal[i]->GetType() == m_ActiveWeapon) {
            weapon = m_Arsenal[i];
            break;
        }
    }

    return weapon;
}

};  // namespace entity