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

def CreateTestEnemy():
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
    EntityAddRenderCom(enemy, "res\model\Enemy\TD_EnemyTest.obj")
    EntityAddScriptCom(enemy, "testenemy")
    EntityAddRoleCom(enemy, RMT_ENEMY, RST_E01)
    EntityAddDataCom(enemy)
    EntityAddFloatData(enemy, "hp", E01_HP)
    EntityAddIntData(enemy, "coin", E01_COIN)

def main():
    pre = TimeGetPrevGameTime()
    cur = TimeGetCurGameTime()
    pre = math.floor(pre / 30.0)
    cur = math.floor(cur / 30.0)
    if pre != cur:
        CreateTestEnemy()
        CreateTestEnemy()
        CreateTestEnemy()
        CreateTestEnemy()