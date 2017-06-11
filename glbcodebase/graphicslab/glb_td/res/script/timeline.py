#!/usr/env/bin python32

"""
Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
Author: i_dovelemon[1322600812@qq.com]
Date: 2017/05/28
Brief: Test level time line
"""

import math
import random
from config import *
from host_api import *  

def IsTimeMatch(time):
    return TimeGetPrevGameTime() < time and time <= TimeGetCurGameTime()

def createE01():
    is_lr = True
    sx = 0.0
    sy = 0.0
    sz = 0.0
    if random.random() > 0.5:
        is_lr = False
    if is_lr is True:
        is_left = True
        if random.random() > 0.5:
            is_left = False
        if is_left is True:
            sx = -1.0 * GAME_WORLD_WIDTH / 2 - 0.5
        else:
            sx = 1.0 * GAME_WORLD_WIDTH / 2 + 0.5
        sz = -1.0 * GAME_WORLD_DEPTH / 2 + random.randint(0, GAME_WORLD_DEPTH) * 1.0
    else:
        is_up = True
        if random.random() > 0.5:
            is_up = False
        if is_up is True:
            sz = 1.0 * GAME_WORLD_DEPTH / 2 + 0.5
        else:
            sz = -1.0 * GAME_WORLD_DEPTH / 2 - 0.5
        sx = -1.0 * GAME_WORLD_WIDTH / 2 + random.randint(0, GAME_WORLD_WIDTH) * 1.0

    enemy = EntityCreate()
    EntityAddTransformCom(enemy, sx, sy, sz, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0)
    EntityAddRenderCom(enemy, "res\model\Enemy\TD_EnemyTest.obj", sx, sy, sz, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0)
    EntityAddScriptCom(enemy, "e01")
    EntityAddRoleCom(enemy, RMT_ENEMY, RST_E01)
    EntityAddDataCom(enemy)
    EntityAddFloatData(enemy, "hp", E01_HP)
    EntityAddIntData(enemy, "coin", E01_COIN)
    EntityAddFloatData(enemy, "speed", E01_SPEED)
    player = EntityFindEntity(RMT_PLAYER, RST_MAINTOWER)
    EntityAddIntData(enemy, "target", player)
    EntityAddIntData(enemy, "state", 0)
    EntityAddFloatData(enemy, "prepare_attack_time", 0.0)
    EntityAddFloatData(enemy, "attack_time", 0.0)
    EntityAddFloatData(enemy, "finish_attack_time", 0.0)
    EntityAddIntData(enemy, "has_attack", 0)

def createE02():
    is_lr = True
    sx = 0.0
    sy = 0.0
    sz = 0.0
    if random.random() > 0.5:
        is_lr = False
    if is_lr is True:
        is_left = True
        if random.random() > 0.5:
            is_left = False
        if is_left is True:
            sx = -1.0 * GAME_WORLD_WIDTH / 2 - 0.5
        else:
            sx = 1.0 * GAME_WORLD_WIDTH / 2 + 0.5
        sz = -1.0 * GAME_WORLD_DEPTH / 2 + random.randint(0, GAME_WORLD_DEPTH) * 1.0
    else:
        is_up = True
        if random.random() > 0.5:
            is_up = False
        if is_up is True:
            sz = 1.0 * GAME_WORLD_DEPTH / 2 + 0.5
        else:
            sz = -1.0 * GAME_WORLD_DEPTH / 2 - 0.5
        sx = -1.0 * GAME_WORLD_WIDTH / 2 + random.randint(0, GAME_WORLD_WIDTH) * 1.0

    enemy = EntityCreate()
    EntityAddTransformCom(enemy, sx, sy, sz, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0)
    EntityAddRenderCom(enemy, "res\model\Enemy\TD_E02.obj", sx, sy, sz, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0)
    EntityAddScriptCom(enemy, "e02")
    EntityAddRoleCom(enemy, RMT_ENEMY, RST_E02)
    EntityAddDataCom(enemy)
    EntityAddFloatData(enemy, "hp", E02_HP)
    EntityAddIntData(enemy, "coin", E02_COIN)
    EntityAddFloatData(enemy, "speed", E02_SPEED)
    player = EntityFindEntity(RMT_PLAYER, RST_MAINTOWER)
    EntityAddIntData(enemy, "target", player)    

def main():
    pre = TimeGetPrevGameTime()
    cur = TimeGetCurGameTime()
    pre = math.floor(pre / 60.0)
    cur = math.floor(cur / 60.0)
    if pre != cur:
        createE01()
        createE01()
        createE01()
        createE02()