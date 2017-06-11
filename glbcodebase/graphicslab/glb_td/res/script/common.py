#!/usr/env/bin python32

"""
Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
Author: i_dovelemon[1322600812@qq.com]
Date: 2017/06/11
Brief: Common method
"""

COLLISION_RESPONSE_EP = 0.000001
COLLISION_DIRECTION_LEFT = 0
COLLISION_DIRECTION_RIGHT = 1
COLLISION_DIRECTION_UP = 2
COLLISION_DIRECTION_DOWN = 3

def query_collision_direction(x0,z0,x1,z1,w,l):
    direction = COLLISION_DIRECTION_LEFT
    dx = x0 - x1
    dz = z0 - z1
    if dz == 0.0:
        dz = COLLISION_RESPONSE_EP
    dr = abs(dx / dz)
    r = w / l
    if dr > r:
        if dx < 0.0:
            direction = COLLISION_DIRECTION_RIGHT
        else:
            direction = COLLISION_DIRECTION_LEFT
    else:
        if dz < 0.0:
            direction = COLLISION_DIRECTION_DOWN
        else:
            direction = COLLISION_DIRECTION_UP

    return direction