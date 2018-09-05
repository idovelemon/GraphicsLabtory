"""
Declaration: Copyright (c), by idovelemoen, 2018. All right reserved.
Author: idovelemon[1322600812@qq.com]
Date: 2018-09-05
Brief: Obstacle relative method
"""

from host_api import *
import config

def ObstacleCreateWall(x, y, z, rx, ry, rz, width, height, depth):
    obstacle = EntityCreate()
    EntityAddRenderCom(obstacle, "res/model/obstacle/wall.obj", 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, True, 10)
    EntityAddTransformCom(obstacle, x, y, z, rx, ry, rz, width / 2.0, height / 2.0, depth / 2.0)
    EntityAddCollisionCom(obstacle, x, y, z, 2.0, 2.0, 2.0)
    EntitySetCollisionFilter(obstacle, ObstacleFilter, PlayerFilter)
    EntityAddDataCom(obstacle)
    EntityAddStringData(obstacle, "Tag", "Wall")   

def ObstacleCreateSpike(x, y, z):
    obstacle = EntityCreate()
    EntityAddRenderCom(obstacle, "res/model/obstacle/spike.obj", 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, True, 10)
    EntityAddTransformCom(obstacle, x, y, z, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0)
    EntityAddScriptCom(obstacle, "EntityObstacleSpikeUpdater")
    EntityAddCollisionCom(obstacle, x, y, z, 2.0, 2.0, 2.0)
    EntitySetCollisionHandle(obstacle, "EntityObstacleSpikeCollisionHandle")
    EntitySetCollisionFilter(obstacle, ObstacleFilter, PlayerFilter | PlayerBulletFilter)
    EntityAddDataCom(obstacle)
    EntityAddStringData(obstacle, "State", "down")
    EntityAddFloatData(obstacle, "TotalTime", config.SPIKE_DOWN_TIME)
    EntityAddFloatData(obstacle, "CurTime", 0.0)
    EntityAddFloatData(obstacle, "StartY", -3.0)
    EntityAddFloatData(obstacle, "EndY", 0.0)
    EntityAddFloatData(obstacle, "ExtraSpeed", 1.0)
    EntityAddFloatData(obstacle, "ExtraSpeedTimer", 0.0)

def ObstacleCreatePushRock(x, y, z, rx, ry, rz):
    obstacle = EntityCreate()
    EntityAddRenderCom(obstacle, "res/model/obstacle/pushrock.obj", 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, True, 10)
    EntityAddTransformCom(obstacle, x, y, z, rx, ry, rz, 1.0, 1.0, 1.0)
    EntityAddScriptCom(obstacle, "EntityObstaclePushRockUpdater")
    EntityAddCollisionCom(obstacle, x, y, z, 5.0, 2.0, 8.0)
    EntitySetCollisionHandle(obstacle, "EntityObstaclePushRockCollisionHandle")
    EntitySetCollisionFilter(obstacle, ObstacleFilter, PlayerFilter | PlayerBulletFilter)
    EntityAddDataCom(obstacle)
    EntityAddStringData(obstacle, "State", "back")
    EntityAddFloatData(obstacle, "TotalTime", config.PUSHROCK_PUSH_TIME)
    EntityAddFloatData(obstacle, "CurTime", 0.0)
    EntityAddFloatData(obstacle, "StartZ", z - 10.0)
    EntityAddFloatData(obstacle, "EndZ", 0.0)
    EntityAddFloatData(obstacle, "OriX", x)
    EntityAddFloatData(obstacle, "OriY", y)
    EntityAddFloatData(obstacle, "OriZ", z)
    EntityAddFloatData(obstacle, "ExtraSpeed", 1.0)
    EntityAddFloatData(obstacle, "ExtraSpeedTimer", 0.0)

def ObstacleCreateRotateRock(x, y, z):
    obstacle = EntityCreate()
    EntityAddRenderCom(obstacle, "res/model/obstacle/rotaterock.obj", 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, True, 10)
    EntityAddTransformCom(obstacle, x, y, z, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0)
    EntityAddScriptCom(obstacle, "EntityObstacleRotateRockUpdater")
    EntityAddCollisionCom(obstacle, x, y, z, 2.0, 2.0, 15.0)
    EntitySetCollisionHandle(obstacle, "EntityObstacleRotateRockCollisionHandle")
    EntitySetCollisionFilter(obstacle, ObstacleFilter, PlayerFilter | PlayerBulletFilter)
    EntityAddDataCom(obstacle)
    EntityAddFloatData(obstacle, "ExtraSpeed", 1.0)
    EntityAddFloatData(obstacle, "ExtraSpeedTimer", 0.0)