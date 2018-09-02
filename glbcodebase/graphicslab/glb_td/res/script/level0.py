"""
Declaration: Copyright (c), by idovelemoen, 2018. All right reserved.
Author: idovelemon[1322600812@qq.com]
Date: 2018-06-21
Brief: Level 0
"""

from sys import *
from host_api import *
import config

LEVEL_COUNT = 0

def EnemyCreateE01(x, y, z):
    enemy = EntityCreate()
    EntityAddTransformCom(enemy, x, y, z, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0)
    EntityAddScriptCom(enemy, "EntityEnemy01Updater")
    EntityAddCollisionCom(enemy, x, y, z, 1.8, 1.8, 1.8)
    EntitySetCollisionHandle(enemy, "EntityEnemy01CollisionHandle")
    EntitySetCollisionFilter(enemy, EnemyFilter, PlayerFilter | PlayerBulletFilter)
    EntityAddDataCom(enemy)
    EntityAddFloatData(enemy, "HP", 5.0)
    EntityAddStringData(enemy, "Tag", "Enemy")
    EntityAddRelationshipCom(enemy)

    enemyCockPit = EntityCreate()
    EntityAddRenderCom(enemyCockPit, "res/model/enemy/enemy_cockpit.obj", 0.0, 0.0, 0.0, 0.0, 0.0, 0., 1.0, 1.0, 1.0, True, 10)
    EntityAddTransformCom(enemyCockPit, 0.0, 0.0, 0.0, 0.0, 0.0, 0., 1.0, 1.0, 1.0)
    EntityAddRelationshipCom(enemyCockPit)
    EntityBindRelationship(enemy, enemyCockPit)

    enemyRotate = EntityCreate()
    EntityAddRenderCom(enemyRotate, "res/model/enemy/enemy_rotate.obj", 0.0, 0.0, 0.0, 0.0, 0.0, 0., 1.0, 1.0, 1.0, True, 10)
    EntityAddTransformCom(enemyRotate, 0.0, 0.0, 0.0, 0.0, 0.0, 0., 1.0, 1.0, 1.0)
    EntityAddRelationshipCom(enemyRotate)
    EntityBindRelationship(enemy, enemyRotate)
    EntityAddScriptCom(enemyRotate, "EntityEnemy01RotateUpdater")

def EnemyCreateE02(x, y, z):
    enemy = EntityCreate()
    EntityAddTransformCom(enemy, x, y, z, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0)
    EntityAddScriptCom(enemy, "EntityEnemy02Updater")
    EntityAddCollisionCom(enemy, x, y, z, 1.8, 1.8, 1.8)
    EntitySetCollisionHandle(enemy, "EntityEnemy01CollisionHandle")
    EntitySetCollisionFilter(enemy, EnemyFilter, PlayerFilter | PlayerBulletFilter)
    EntityAddDataCom(enemy)
    EntityAddFloatData(enemy, "HP", 10.0)
    EntityAddStringData(enemy, "Tag", "Enemy")
    EntityAddStringData(enemy, "State", "Idle")
    EntityAddFloatData(enemy, "StateTimer", config.ENTITY_ENEMY_02_IDLE_TIME)
    EntityAddFloatData(enemy, "RushDirX", 0.0)
    EntityAddFloatData(enemy, "RushDirY", 0.0)
    EntityAddFloatData(enemy, "RushDirZ", 0.0)
    EntityAddRelationshipCom(enemy)

    enemyCockPit = EntityCreate()
    EntityAddRenderCom(enemyCockPit, "res/model/enemy/enemy_cockpit.obj", 0.0, 0.0, 0.0, 0.0, 0.0, 0., 1.0, 1.0, 1.0, True, 10)
    EntityAddTransformCom(enemyCockPit, 0.0, 0.0, 0.0, 0.0, 0.0, 0., 1.0, 1.0, 1.0)
    EntityAddRelationshipCom(enemyCockPit)
    EntityBindRelationship(enemy, enemyCockPit)

    enemyRotate = EntityCreate()
    EntityAddRenderCom(enemyRotate, "res/model/enemy/enemy_rotate.obj", 0.0, 0.0, 0.0, 0.0, 0.0, 0., 1.0, 1.0, 1.0, True, 10)
    EntityAddTransformCom(enemyRotate, 0.0, 0.0, 0.0, 0.0, 0.0, 0., 1.0, 1.0, 1.0)
    EntityAddRelationshipCom(enemyRotate)
    EntityBindRelationship(enemy, enemyRotate)
    EntityAddScriptCom(enemyRotate, "EntityEnemy01RotateUpdater")    

def EnemyCreateE03(x, y, z):
    enemy = EntityCreate()
    EntityAddTransformCom(enemy, x, y, z, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0)
    EntityAddScriptCom(enemy, "EntityEnemy03Updater")
    EntityAddCollisionCom(enemy, x, y, z, 1.8, 1.8, 1.8)
    EntitySetCollisionHandle(enemy, "EntityEnemy01CollisionHandle")
    EntitySetCollisionFilter(enemy, EnemyFilter, PlayerFilter | PlayerBulletFilter)
    EntityAddDataCom(enemy)
    EntityAddFloatData(enemy, "HP", 10.0)
    EntityAddStringData(enemy, "Tag", "Enemy")
    EntityAddStringData(enemy, "State", "FindPlayer")
    EntityAddFloatData(enemy, "StateTimer", 0.0)
    EntityAddFloatData(enemy, "MoveDirX", 0.0)
    EntityAddFloatData(enemy, "MoveDirY", 0.0)
    EntityAddFloatData(enemy, "MoveDirZ", 0.0)
    EntityAddRelationshipCom(enemy)

    enemyCockPit = EntityCreate()
    EntityAddRenderCom(enemyCockPit, "res/model/enemy/enemy_cockpit.obj", 0.0, 0.0, 0.0, 0.0, 0.0, 0., 1.0, 1.0, 1.0, True, 10)
    EntityAddTransformCom(enemyCockPit, 0.0, 0.0, 0.0, 0.0, 0.0, 0., 1.0, 1.0, 1.0)
    EntityAddRelationshipCom(enemyCockPit)
    EntityBindRelationship(enemy, enemyCockPit)

    enemyRotate = EntityCreate()
    EntityAddRenderCom(enemyRotate, "res/model/enemy/enemy_rotate.obj", 0.0, 0.0, 0.0, 0.0, 0.0, 0., 1.0, 1.0, 1.0, True, 10)
    EntityAddTransformCom(enemyRotate, 0.0, 0.0, 0.0, 0.0, 0.0, 0., 1.0, 1.0, 1.0)
    EntityAddRelationshipCom(enemyRotate)
    EntityBindRelationship(enemy, enemyRotate)
    EntityAddScriptCom(enemyRotate, "EntityEnemy01RotateUpdater")

def ObstacleCreateSpike(x, y, z):
    obstacle = EntityCreate()
    EntityAddRenderCom(obstacle, "res/model/obstacle/spike.obj", 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, True, 10)
    EntityAddTransformCom(obstacle, x, y, z, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0)
    EntityAddScriptCom(obstacle, "EntityObstacleSpikeUpdater")
    EntityAddCollisionCom(obstacle, x, y, z, 2.0, 2.0, 2.0)
    EntitySetCollisionHandle(obstacle, "EntityObstacleSpikeCollisionHandle")
    EntitySetCollisionFilter(obstacle, ObstacleFilter, PlayerFilter)
    EntityAddDataCom(obstacle)
    EntityAddStringData(obstacle, "State", "down")
    EntityAddFloatData(obstacle, "TotalTime", config.SPIKE_DOWN_TIME)
    EntityAddFloatData(obstacle, "CurTime", 0.0)
    EntityAddFloatData(obstacle, "StartY", -3.0)
    EntityAddFloatData(obstacle, "EndY", 0.0)

def ObstacleCreatePushRock(x, y, z, rx, ry, rz):
    obstacle = EntityCreate()
    EntityAddRenderCom(obstacle, "res/model/obstacle/pushrock.obj", 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, True, 10)
    EntityAddTransformCom(obstacle, x, y, z, rx, ry, rz, 1.0, 1.0, 1.0)
    EntityAddScriptCom(obstacle, "EntityObstaclePushRockUpdater")
    EntityAddCollisionCom(obstacle, x, y, z, 5.0, 2.0, 8.0)
    EntitySetCollisionHandle(obstacle, "EntityObstaclePushRockCollisionHandle")
    EntitySetCollisionFilter(obstacle, ObstacleFilter, PlayerFilter)
    EntityAddDataCom(obstacle)
    EntityAddStringData(obstacle, "State", "back")
    EntityAddFloatData(obstacle, "TotalTime", config.PUSHROCK_PUSH_TIME)
    EntityAddFloatData(obstacle, "CurTime", 0.0)
    EntityAddFloatData(obstacle, "StartZ", z - 10.0)
    EntityAddFloatData(obstacle, "EndZ", 0.0)
    EntityAddFloatData(obstacle, "OriX", x)
    EntityAddFloatData(obstacle, "OriY", y)
    EntityAddFloatData(obstacle, "OriZ", z)

def ObstacleCreateRotateRock(x, y, z):
    obstacle = EntityCreate()
    EntityAddRenderCom(obstacle, "res/model/obstacle/rotaterock.obj", 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, True, 10)
    EntityAddTransformCom(obstacle, x, y, z, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0)
    EntityAddScriptCom(obstacle, "EntityObstacleRotateRockUpdater")
    EntityAddCollisionCom(obstacle, x, y, z, 2.0, 2.0, 15.0)
    EntitySetCollisionHandle(obstacle, "EntityObstacleRotateRockCollisionHandle")
    EntitySetCollisionFilter(obstacle, ObstacleFilter, PlayerFilter)

def main():
    global LEVEL_COUNT
    LEVEL_COUNT = LEVEL_COUNT + 1

    if LEVEL_COUNT == 1:
        DebugPrint("TD Level0 Start\n")

        # Create Level0 Entity
        level0 = EntityCreate()
        EntityAddRenderCom(level0, "res/model/level/level0.obj", 0.0, -1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, False, 0)

        # Create CoreDummy Entity
        coreDummy = EntityCreate()
        EntityAddTransformCom(coreDummy, -15.0, 0.0, -20.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0)
        EntityAddScriptCom(coreDummy, "EntityCoreDummyUpdater")
        EntityAddDataCom(coreDummy)
        EntityAddStringData(coreDummy, "Tag", "Player")
        EntityAddRelationshipCom(coreDummy)
        EntityAddCollisionCom(coreDummy, 0.0, 0.0, 0.0, 1.8, 1.8, 1.8)
        EntitySetCollisionHandle(coreDummy, "EntityPlayerCollisionHandle")
        EntitySetCollisionFilter(coreDummy, PlayerFilter, EnemyFilter | EnemyBulletFilter | ObstacleFilter)

        # Create Camera Entity
        camera = EntityCreate()
        EntityAddCameraCom(camera, 0.0, 10.0, 10.0, 0.0, 0.0, 0.0)
        EntityAddTransformCom(camera, 0.0, 10.0, 10.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0)
        EntityAddRelationshipCom(camera)
        EntityBindRelationship(coreDummy, camera)

        # Create Core Entity
        core = EntityCreate()
        EntityAddTransformCom(core, 0.0, 1.2, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0)
        EntityAddRelationshipCom(core)
        EntityBindRelationship(coreDummy, core)
        EntityAddRenderCom(core, "res/model/core/core.obj", 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, False, 0)
        EntityAddScriptCom(core, "EntityCoreUpdater")

        # Create Core Rot Entity
        coreRot = EntityCreate()
        EntityAddTransformCom(coreRot, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0)
        EntityAddRelationshipCom(coreRot)
        EntityBindRelationship(coreDummy, coreRot)
        EntityAddRenderCom(coreRot, "res/model/core/coreRot.obj", 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, False, 0)
        EntityAddScriptCom(coreRot, "EntityCoreRotUpdater")
        EntityAddDataCom(coreRot)
        EntityAddFloatData(coreRot, "CurRotateSpeed", config.ENTITY_CORE_ROT_ROTATE_MIN_SPEED)

        # Create Normal Emitter
        # rightNormalEmitter = EntityCreate()
        # EntityAddTransformCom(rightNormalEmitter, 1.8, 0.75, 0.0, 0.0, 90.0, 0.0, 1.0, 1.0, 1.0)
        # EntityAddRelationshipCom(rightNormalEmitter)
        # EntityBindRelationship(coreRot, rightNormalEmitter)
        # EntityAddRenderCom(rightNormalEmitter, "res/model/emitter/normal_Emitter.obj", 1.8, 0.75, 0.0, 0.0, 90.0, 0.0, 1.0, 1.0, 1.0, True, 4)
        # EntityAddScriptCom(rightNormalEmitter, "EntityNormalEmitterUpdater")
        # EntityAddDataCom(rightNormalEmitter)
        # EntityAddFloatData(rightNormalEmitter, "ShootDelta", 0.0)

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

    if LEVEL_COUNT == 3000:
        DebugPrint("TD Level0 Finish\n")