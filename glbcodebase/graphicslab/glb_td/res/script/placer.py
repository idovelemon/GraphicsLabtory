#!/usr/env/bin python32

"""
Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
Author: i_dovelemon[1322600812@qq.com]
Date: 2017/05/30
Brief: Test level placer's AI
"""

import math
from config import *
from host_api import *

def screen_to_world(x, y):
    step = SCREEN_WIDTH * 1.0 / GAME_WORLD_WIDTH
    wx = math.floor(x / step) - GAME_WORLD_WIDTH / 2
    wy = math.floor((y - SCREEN_HEIGHT * 1.0 / 2) / step)
    return wx + 0.5, 0.01, wy + 0.5

def create_sub_tower(x, y, z):
    sub_tower = EntityCreate()
    EntityAddTransformCom(sub_tower, x, y, z, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0)
    EntityAddRenderCom(sub_tower, "res\model\Tower\TD_Sub_Tower.obj")
    EntityAddScriptCom(sub_tower, "subtower")
    EntityAddRoleCom(sub_tower, RMT_PLAYER, RST_SUBTOWER)
    EntityAddArsenalCom(sub_tower)
    EntityAddWeapon(sub_tower, WT_MACHINEGUN, "wpmachinegun")
    EntityActiveWeapon(sub_tower, WT_MACHINEGUN)
    EntityAddDataCom(sub_tower)
    EntityAddFloatData(sub_tower, "shootdelta", 0.0)

def main(id):
    main_tower = EntityGetIntData(id, "maintower")
    cur_coin = EntityGetIntData(main_tower, "coin")

    enable = EntityGetIntData(id, "enable")
    if cur_coin > SUBTOWER_COST_COIN:
        if InputHasKeyPressed(BK_1):
            if enable != 0:
                enable = 0
                EntitySetDrawEnable(id, 0)
            else:
                enable = 1
                EntitySetDrawEnable(id, 1)
            EntitySetIntData(id, "enable", enable)

    if enable != 0:
        cursor_x = InputGetCursorPosX()
        cursor_y = InputGetCursorPosY()
        wx,wy,wz = screen_to_world(cursor_x, cursor_y)
        EntitySetPos(id, wx, wy, wz)

        if InputHasMousePressed(BM_LEFT):
            create_sub_tower(wx, wy, wz)
            cur_coin = cur_coin - SUBTOWER_COST_COIN
            enable = 0
            EntitySetDrawEnable(id, 0)
            EntitySetIntData(id, "enable", enable)
            EntitySetIntData(main_tower, "coin", cur_coin)