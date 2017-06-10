#!/usr/env/bin python32

"""
Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
Author: i_dovelemon[1322600812@qq.com]
Date: 2017/05/29
Brief: Test level bullet laser AI
"""

from config import *
from host_api import *

def main(id):
    is_dead = 0

    attacker = EntityGetIntData(id, "attacker")
    enemy = EntityGetIntData(id, "target")
    if EntityIsDead(enemy) == 0:  # Enemy is not dead
        # Update transform
        cur_px = EntityGetPosX(attacker)
        cur_py = EntityGetPosY(attacker)
        cur_pz = EntityGetPosZ(attacker)   
        EntitySetPos(id, cur_px, cur_py, cur_pz)
        enemy_px = EntityGetPosX(enemy)
        enemy_py = EntityGetPosY(enemy)
        enemy_pz = EntityGetPosZ(enemy)
        EntitySetRotateByLookVec(id, enemy_px - cur_px, enemy_py - cur_py, enemy_pz - cur_pz)
        EntitySetScaleByLookVec(id, enemy_px - cur_px, enemy_py - cur_py, enemy_pz - cur_pz)

        # Damage
        damage = EntityGetFloatData(id, "damage")
        enemy_hp = EntityGetFloatData(enemy, "hp")
        enemy_hp = enemy_hp - damage * TimeGetGameFrameSpeed()
        EntitySetFloatData(enemy, "hp", enemy_hp)
        if enemy_hp < 0.0:
            enemy_coin = EntityGetIntData(enemy, "coin")
            attacker_coin = EntityGetIntData(attacker, "coin")
            attacker_coin = attacker_coin + enemy_coin
            EntitySetIntData(attacker, "coin", attacker_coin)
            is_dead = 1

        # Check if out of range
        dist = EntityCalcDistBetweenEntities(enemy, attacker)
        if dist > LASER_ATTACK_DIST:
            is_dead = 1
    else:
        is_dead = 1

    # Dead?
    if is_dead != 0:
        EntitySetDead(id, is_dead)
        bullet_num = EntityGetWeaponBulletNum(attacker)
        bullet_num = bullet_num - 1
        EntitySetWeaponBulletNum(attacker, bullet_num)
        EntityDecRefCount(attacker)
        EntityDecRefCount(enemy)