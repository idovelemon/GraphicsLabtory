#!/usr/env/bin python32

"""
Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
Author: i_dovelemon[1322600812@qq.com]
Date: 2017/05/26
Brief: Test level laser weapon AI
"""

from config import *
from host_api import *

def main(id):
    bullet_num = EntityGetWeaponBulletNum(id)
    if bullet_num >= 1:
        return

    enemy = EntityFindCloestEnemy(id)
    if enemy != -1:
        dist = EntityCalcDistBetweenEntities(id, enemy)
        if dist < LASER_ATTACK_DIST:
            btlaser = EntityCreate()
            px = EntityGetPosX(id)
            py = EntityGetPosY(id)
            pz = EntityGetPosZ(id)
            EntityAddTransformCom(btlaser, px, py, pz, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0)
            EntityAddRenderCom(btlaser, "res\model\Bullet\TD_Bullet_Laser.obj", px, py, pz, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0)
            EntityAddRoleCom(btlaser, RMT_PLAYER, RST_BULLET_LASER)
            EntityAddScriptCom(btlaser, "btlaser")
            EntityAddDataCom(btlaser)
            EntityAddIntData(btlaser, "attacker", id)
            EntityAddRefCount(id)
            EntityAddIntData(btlaser, "target", enemy)
            EntityAddRefCount(enemy)
            EntityAddFloatData(btlaser, "damage", LASER_DAMAGE)
            EntitySetWeaponBulletNum(id, bullet_num + 1)
        