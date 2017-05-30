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
    shoot_delta = EntityGetFloatData(id, "shootdelta")
    if shoot_delta > MACHINE_GUN_SHOOT_DELTA:
        shoot_delta = 0.0
        enemy = EntityFindCloestEnemy(id)
        dist = EntityCalcDistBetweenEntities(id, enemy)
        if dist < MACHINE_GUN_ATTACK_DIST:
            btmachinegun = EntityCreate()
            btposx = EntityGetPosX(id)
            btposy = EntityGetPosY(id)
            btposz = EntityGetPosZ(id)
            EntityAddTransformCom(btmachinegun, btposx, btposy, btposz, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0)
            EntityAddRoleCom(btmachinegun, RMT_PLAYER, RST_BULLET_MACHINEGUN)
            EntityAddRenderCom(btmachinegun, "res\model\Bullet\TD_Bullet_MachineGun.obj")
            EntityAddScriptCom(btmachinegun, "btmachinegun")
            EntityAddDataCom(btmachinegun)
            EntityAddIntData(btmachinegun, "attacker", id)
            EntityAddIntData(btmachinegun, "target", enemy)
            EntityAddFloatData(btmachinegun, "delta", 0.0)
            EntityAddRefCount(id)
            EntityAddRefCount(enemy)
    shoot_delta = shoot_delta + TimeGetGameFrameSpeed()
    EntitySetFloatData(id, "shootdelta", shoot_delta)