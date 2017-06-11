#!/usr/env/bin python32

"""
Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
Author: i_dovelemon[1322600812@qq.com]
Date: 2017/05/26
Brief: Test level emey 2 AI
"""

from config import *
from host_api import *

def main(id):
    speed = EntityGetFloatData(id, "speed")
    target = EntityGetIntData(id, "target")
    tx = EntityGetPosX(target)
    ty = EntityGetPosY(target)
    tz = EntityGetPosZ(target)
    EntityMoveToTarget(id, speed, tx, ty, tz)

    # Check if hurt player?

    # Check if dead?
    hp = EntityGetFloatData(id, "hp")
    if hp < 0.0:
        EntitySetDead(id, 1)