#!/usr/env/bin python32

"""
Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
Author: i_dovelemon[1322600812@qq.com]
Date: 2017/05/26
Brief: Test level
"""

from config import *
from host_api import *

def main():
    DebugPrint("-----------------testlevel Start-----------------\n")

    # Create camera entity
    cam = EntityCreate()
    EntityAddCameraCom(cam, 0.0, 11.0, 0.1, 0.0, 0.0, 0.0)
    DebugPrint("Create Camera Entity: OK\n")

    # Create ground entity
    # ground = EntityCreate()
    # EntityAddTransformCom(ground, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0)
    # EntityAddRenderCom(ground, "res\model\TD_L1\Ground\TD_L1_Ground.obj")
    # DebugPrint("Create Ground Entity: OK\n")

    # Create ground block entity
    # TEST: This is only for test, in the final game, you can not use too many entities just for ground
    sx = -1.0 * GAME_WORLD_WIDTH / 2 + 0.5
    sy = 0.0
    sz = 1.0 * GAME_WORLD_DEPTH / 2 - 0.5
    white_block = "res\model\TD_L1\Ground\TD_L1_Ground_Block_White.obj"
    gray_block = "res\model\TD_L1\Ground\TD_L1_Ground_Block_Gray.obj"
    for i in range(GAME_WORLD_DEPTH):
        for j in range(GAME_WORLD_WIDTH):
            block = EntityCreate()
            EntityAddTransformCom(block, sx + j * 1.0, sy, sz - i * 1.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0)
            if ((i + j) % 2) == 0:
                EntityAddRenderCom(block, white_block)
            else:
                EntityAddRenderCom(block, gray_block)
    DebugPrint("Create Ground Blocks Entity: OK\n")

    # Create main tower
    main_tower = EntityCreate()
    EntityAddTransformCom(main_tower, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0)
    EntityAddRenderCom(main_tower, "res\model\Tower\TD_Main_Tower.obj")
    EntityAddRoleCom(main_tower, RMT_PLAYER, RST_MAINTOWER)
    EntityAddScriptCom(main_tower, "maintower")
    EntityAddDataCom(main_tower)
    EntityAddIntData(main_tower, "coin", 0)
    EntityAddFloatData(main_tower, "speed", MAIN_TOWER_MOVE_SPEED)
    EntityAddFloatData(main_tower, "press_delta", MAIN_TOWER_PRESS_DELTA)
    EntityAddFloatData(main_tower, "cur_press_delta", 0.0)
    EntityAddArsenalCom(main_tower)
    EntityAddWeapon(main_tower, WT_LASER, "wplaser")
    EntityActiveWeapon(main_tower, WT_LASER)
    DebugPrint("Create Ground Main Tower: OK\n")

    # Create placer entity
    # placer = EntityCreate()
    # EntityAddScriptCom(placer, "placer")
    # EntityAddRoleCom(placer, RMT_OPERATOR, RST_PLACER)
    # EntityAddTransformCom(placer, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0)
    # EntityAddRenderCom(placer, "res\model\TD_Placer.obj")
    # EntityAddDataCom(placer)
    # EntityAddIntData(placer, "maintower", main_tower)
    # EntityAddIntData(placer, "enable", 0)
    # EntityAddRefCount(main_tower)
    # # EntitySetDrawEnable(placer, 0)    

    DebugPrint("-----------------testlevel End-----------------\n")