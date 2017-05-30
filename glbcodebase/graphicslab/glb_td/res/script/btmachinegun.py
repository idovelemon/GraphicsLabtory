#!/usr/env/bin python32

"""
Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
Author: i_dovelemon[1322600812@qq.com]
Date: 2017/05/30
Brief: Test level bullet laser AI
"""

from config import *
from host_api import *

def main(id):
    attacker = EntityGetIntData(id, "attacker")
    enemy = EntityGetIntData(id, "target")
    delta = EntityGetFloatData(id, "delta")

    need_destroy = False
    if EntityIsDead(enemy) == 0:
        attack_x = EntityGetPosX(attacker)
        attack_y = EntityGetPosY(attacker)
        attack_z = EntityGetPosZ(attacker)
        enemy_x = EntityGetPosX(enemy)
        enemy_y = EntityGetPosY(enemy)
        enemy_z = EntityGetPosZ(enemy)
        ratio = delta / MACHINE_GUN_TOTAL_DELTA
        cur_x = attack_x + ratio * (enemy_x - attack_x)
        cur_y = attack_y + ratio * (enemy_y - attack_y)
        cur_z = attack_z + ratio * (enemy_z - attack_z)
        EntitySetPos(id, cur_x, cur_y, cur_z)

        delta = delta + TimeGetGameFrameSpeed()
        EntitySetFloatData(id, "delta", delta)

        if delta > MACHINE_GUN_TOTAL_DELTA:
            enemy_hp = EntityGetFloatData(enemy, "hp")
            enemy_hp = enemy_hp - MACHINE_GUN_DAMAGE * TimeGetGameFrameSpeed()
            EntitySetFloatData(enemy, "hp", enemy_hp)
            need_destroy = True
    else:
        need_destroy = True

    if need_destroy is True:
        EntitySetDead(id, 1)
        EntityDecRefCount(attacker)
        EntityDecRefCount(enemy)