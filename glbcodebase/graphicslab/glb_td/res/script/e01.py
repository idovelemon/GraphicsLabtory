#!/usr/env/bin python32

"""
Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
Author: i_dovelemon[1322600812@qq.com]
Date: 2017/05/26
Brief: Test level emey 1 AI
"""

from config import *
from host_api import *

E01_STATE_RUN = 0
E01_STATE_PREPARE_ATTACK = 1
E01_STATE_ATTACK = 2
E01_STATE_FINISH_ATTACK = 3

def run(id):
    speed = EntityGetFloatData(id, "speed")
    target = EntityGetIntData(id, "target")
    tx = EntityGetPosX(target)
    ty = EntityGetPosY(target)
    tz = EntityGetPosZ(target)
    EntityMoveToTarget(id, speed, tx, ty, tz)

def can_attack_player(id):
    target = EntityGetIntData(id, "target")
    dist = EntityCalcDistBetweenEntities(id, target)
    if dist < E01_ATTACK_RADIUS:
        return True
    else:
        return False

def prepare_attack(id):
    is_end_prepare = False
    prepare_attack_time = EntityGetFloatData(id, "prepare_attack_time")
    prepare_attack_time = prepare_attack_time + TimeGetGameFrameSpeed()
    if prepare_attack_time > E01_TOTAL_PREPARE_ATTACK_TIME:
        prepare_attack_time = 0.0
        is_end_prepare = True
    EntitySetFloatData(id, "prepare_attack_time", prepare_attack_time)
    return is_end_prepare

def attack(id):
    is_end_attack = False
    attack_time = EntityGetFloatData(id, "attack_time")
    attack_time = attack_time + TimeGetGameFrameSpeed()
    if attack_time > E01_TOTAL_ATTACK_TIME:
        attack_time = 0
        is_end_attack = True
    EntitySetFloatData(id, "attack_time", attack_time)

    has_attack = EntityGetIntData(id, "has_attack")
    if has_attack == 0:
        target = EntityGetIntData(id, "target")
        dist = EntityCalcDistBetweenEntities(id, target)
        if dist < E01_ATTACK_RADIUS:
            hp = EntityGetFloatData(target, "hp")
            hp = hp - E01_ATTACK
            EntitySetFloatData(target, "hp", hp)
            EntitySetIntData(id, "has_attack", 1)
    return is_end_attack

def finish_attack(id):
    is_finish_attack = False
    finish_attack_time = EntityGetFloatData(id, "finish_attack_time")
    finish_attack_time = finish_attack_time + TimeGetGameFrameSpeed()
    if finish_attack_time > E01_TOTAL_FINISH_TIME:
        finish_attack_time = 0.0
        is_finish_attack = True
    EntitySetFloatData(id, "finish_attack_time", finish_attack_time)
    return is_finish_attack

def main(id):
    # State machine
    state = EntityGetIntData(id, "state")
    if state == E01_STATE_RUN:
        run(id)
        if can_attack_player(id) is True:
            state = E01_STATE_PREPARE_ATTACK
    elif state == E01_STATE_PREPARE_ATTACK:
        if prepare_attack(id) is True:
            state = E01_STATE_ATTACK
    elif state == E01_STATE_ATTACK:
        if attack(id) is True:
            state = E01_STATE_FINISH_ATTACK
    elif state == E01_STATE_FINISH_ATTACK:
        if finish_attack(id) is True:
            state = E01_STATE_RUN
    EntitySetIntData(id, "state", state)

    # Check if dead?
    hp = EntityGetFloatData(id, "hp")
    if hp < 0.0:
        EntitySetDead(id, 1)