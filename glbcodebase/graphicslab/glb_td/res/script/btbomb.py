#!/usr/env/bin python32

"""
Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
Author: i_dovelemon[1322600812@qq.com]
Date: 2017/05/29
Brief: Test level bullet bomb AI
"""

from config import *
from host_api import *

def main(id):
    # Expand
    time = TimeGetGameFrameSpeed()
    cur_scalex = EntityGetScaleX(id)
    cur_scaley = EntityGetScaleY(id)
    cur_scalez = EntityGetScaleZ(id)
    cur_scalex = cur_scalex + BOMB_SCALE_SPEED * time
    cur_scalez = cur_scalez + BOMB_SCALE_SPEED * time
    EntitySetScale(id, cur_scalex, cur_scaley, cur_scalez)

    # Destroy enemy
    EntityBeginIterate()
    while True:
        enemy_id = EntityIterate()
        if enemy_id is -1:
            break
        if EntityIsMainType(enemy_id, RMT_ENEMY):
            dist = EntityCalcDistBetweenEntities(enemy_id, id)
            if dist < cur_scalex * 0.5:
                EntitySetDead(enemy_id, 1)
    EntityEndIterate()

    # Is Dead?
    if cur_scalex > BOMB_MAX_SCALE or cur_scalez > BOMB_MAX_SCALE:
        EntitySetDead(id, 1)
    