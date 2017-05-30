#!/usr/env/bin python32

"""
Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
Author: i_dovelemon[1322600812@qq.com]
Date: 2017/05/26
Brief: Test level emey AI
"""

from config import *
from host_api import *

def main(id):
    EntityMoveToTarget(id, E01_SPEED, 0.0, 0.0, 0.0)

    # Check if dead
    hp = EntityGetFloatData(id, "hp")
    if hp < 0.0:
        EntitySetDead(id, 1)