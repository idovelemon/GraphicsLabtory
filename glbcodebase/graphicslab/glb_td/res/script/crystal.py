#!/usr/env/bin python32

"""
Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
Author: i_dovelemon[1322600812@qq.com]
Date: 2017/06/08
Brief: Test level crystal's AI
"""

from config import *
from host_api import *

def create_random_item():
    item = EntityCreate()
    EntityAddTransformCom(item, 0.0, 2.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0)
    EntityAddRoleCom(item, RMT_ITEM, RST_ITEM_BOMB)
    EntityAddRenderCom(item, "res\model\Item\TD_Item_Bomb.obj")
    EntityAddScriptCom(item, "itembomb")

def main(id):
    cur_delta = EntityGetFloatData(id, "delta")
    if cur_delta > CRYSTAL_GEN_ITEM_TIME:
        cur_delta = 0.0
        create_random_item()
    else:
        cur_delta = cur_delta + TimeGetGameFrameSpeed()
    EntitySetFloatData(id, "delta", cur_delta)