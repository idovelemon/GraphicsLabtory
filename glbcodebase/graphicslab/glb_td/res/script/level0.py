"""
Declaration: Copyright (c), by idovelemoen, 2018. All right reserved.
Author: idovelemon[1322600812@qq.com]
Date: 2018-06-21
Brief: Level 0
"""

from sys import *
from enemy import *
from host_api import *
from obstacle import *
from player import *
import config

LEVEL_COUNT = 0

def main():
    global LEVEL_COUNT
    LEVEL_COUNT = LEVEL_COUNT + 1

    if LEVEL_COUNT == 1:
        DebugPrint("TD Level0 Start\n")

        # Create Level0 Entity
        level0 = EntityCreate()
        EntityAddRenderCom(level0, "res/model/level/level0.obj", 0.0, -1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, False, 0)        

        PlayerCreate(-15.0, 0.0, -20.0)

    if LEVEL_COUNT == 2:
        ObstacleCreateSpike(0.0, 0.0, 0.0)
        ObstacleCreateSpike(2.5, 0.0, 0.0)
        ObstacleCreateSpike(-2.5, 0.0, 0.0)

    if LEVEL_COUNT == 3:
        ObstacleCreatePushRock(10.0, 0.0, -4.0, 0.0, 0.0, 0.0)
        ObstacleCreatePushRock(10.0, 0.0, 4.0, 0.0, 180.0, 0.0)
        ObstacleCreatePushRock(20.0, 3.0, 0.0, 90.0, 0.0, 0.0)

    if LEVEL_COUNT == 4:
        ObstacleCreateRotateRock(-15.0, 0.0, 0.0)

    if LEVEL_COUNT == 5:
        ObstacleCreateWall(-50.0, 0.0, 0.0, 0.0, 0.0, 0.0, 2.0, 2.0, 100.0)
        ObstacleCreateWall(50.0, 0.0, 0.0, 0.0, 0.0, 0.0, 2.0, 2.0, 100.0)
        ObstacleCreateWall(0.0, 0.0, -50.0, 0.0, 0.0, 0.0, 100.0, 2.0, 2.0)
        ObstacleCreateWall(0.0, 0.0, 50.0, 0.0, 0.0, 0.0, 100.0, 2.0, 2.0)

    if LEVEL_COUNT == 3000:
        DebugPrint("TD Level0 Finish\n")