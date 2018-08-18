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
        EntityAddTransformCom(coreDummy, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0)
        EntityAddScriptCom(coreDummy, "EntityCoreDummyUpdater")
        EntityAddDataCom(coreDummy)
        EntityAddStringData(coreDummy, "Tag", "Player")
        EntityAddRelationshipCom(coreDummy)

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

        # Create Four Normal Emitter
        backNormalEmitter = EntityCreate()
        EntityAddTransformCom(backNormalEmitter, 0.0, 0.75, 1.8, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0)
        EntityAddRelationshipCom(backNormalEmitter)
        EntityBindRelationship(coreRot, backNormalEmitter)        
        EntityAddRenderCom(backNormalEmitter, "res/model/emitter/normal_Emitter.obj", 0.0, 0.75, 1.8, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, True, 4)
        EntityAddScriptCom(backNormalEmitter, "EntityNormalEmitterUpdater")
        EntityAddDataCom(backNormalEmitter)
        EntityAddFloatData(backNormalEmitter, "ShootDelta", 0.0)

        frontNormalEmitter = EntityCreate()
        EntityAddTransformCom(frontNormalEmitter, 0.0, 0.75, -1.8, 0.0, 180.0, 0.0, 1.0, 1.0, 1.0)
        EntityAddRelationshipCom(frontNormalEmitter)
        EntityBindRelationship(coreRot, frontNormalEmitter)        
        EntityAddRenderCom(frontNormalEmitter, "res/model/emitter/normal_Emitter.obj", 0.0, 0.75, -1.8, 0.0, 180.0, 0.0, 1.0, 1.0, 1.0, True, 4)
        EntityAddScriptCom(frontNormalEmitter, "EntityNormalEmitterUpdater")
        EntityAddDataCom(frontNormalEmitter)
        EntityAddFloatData(frontNormalEmitter, "ShootDelta", 0.0)        

        leftNormalEmitter = EntityCreate()
        EntityAddTransformCom(leftNormalEmitter, -1.8, 0.75, 0.0, 0.0, -90.0, 0.0, 1.0, 1.0, 1.0)
        EntityAddRelationshipCom(leftNormalEmitter)
        EntityBindRelationship(coreRot, leftNormalEmitter)                
        EntityAddRenderCom(leftNormalEmitter, "res/model/emitter/normal_Emitter.obj", -1.8, 0.75, 0.0, 0.0, -90.0, 0.0, 1.0, 1.0, 1.0, True, 4)
        EntityAddScriptCom(leftNormalEmitter, "EntityNormalEmitterUpdater")
        EntityAddDataCom(leftNormalEmitter)
        EntityAddFloatData(leftNormalEmitter, "ShootDelta", 0.0)          

        rightNormalEmitter = EntityCreate()
        EntityAddTransformCom(rightNormalEmitter, 1.8, 0.75, 0.0, 0.0, 90.0, 0.0, 1.0, 1.0, 1.0)
        EntityAddRelationshipCom(rightNormalEmitter)
        EntityBindRelationship(coreRot, rightNormalEmitter)
        EntityAddRenderCom(rightNormalEmitter, "res/model/emitter/normal_Emitter.obj", 1.8, 0.75, 0.0, 0.0, 90.0, 0.0, 1.0, 1.0, 1.0, True, 4)
        EntityAddScriptCom(rightNormalEmitter, "EntityNormalEmitterUpdater")
        EntityAddDataCom(rightNormalEmitter)
        EntityAddFloatData(rightNormalEmitter, "ShootDelta", 0.0)                  

    if LEVEL_COUNT == 20:
        enemy = EntityCreate()
        EntityAddTransformCom(enemy, -50.0, 0.0, 50.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0)
        EntityAddScriptCom(enemy, "EntityEnemy01Updater")
        EntityAddCollisionCom(enemy, -50.0, 0.0, 50.0, 1.8, 1.8, 1.8)
        EntitySetCollisionHandle(enemy, "EntityEnemyCollisionHandle")        
        EntitySetCollisionFilter(enemy, EnemyFilter, PlayerFilter)
        EntityAddDataCom(enemy)
        EntityAddFloatData(enemy, "HP", 5.0)        
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

    if LEVEL_COUNT == 40:
        enemy = EntityCreate()
        EntityAddTransformCom(enemy, 50.0, 0.0, 50.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0)
        EntityAddScriptCom(enemy, "EntityEnemy01Updater")
        EntityAddCollisionCom(enemy, 50.0, 0.0, 50.0, 1.8, 1.8, 1.8)
        EntitySetCollisionHandle(enemy, "EntityEnemyCollisionHandle")
        EntitySetCollisionFilter(enemy, EnemyFilter, PlayerFilter)
        EntityAddDataCom(enemy)
        EntityAddFloatData(enemy, "HP", 5.0)
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

    if LEVEL_COUNT == 60:
        enemy = EntityCreate()
        EntityAddTransformCom(enemy, 50.0, 0.0, -50.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0)
        EntityAddScriptCom(enemy, "EntityEnemy01Updater")
        EntityAddCollisionCom(enemy, 50.0, 0.0, -50.0, 1.8, 1.8, 1.8)
        EntitySetCollisionHandle(enemy, "EntityEnemyCollisionHandle")
        EntitySetCollisionFilter(enemy, EnemyFilter, PlayerFilter)
        EntityAddDataCom(enemy)
        EntityAddFloatData(enemy, "HP", 5.0)        
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

    if LEVEL_COUNT == 80:
        enemy = EntityCreate()
        EntityAddTransformCom(enemy, -50.0, 0.0, -50.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0)
        EntityAddScriptCom(enemy, "EntityEnemy01Updater")
        EntityAddCollisionCom(enemy, -50.0, 0.0, -50.0, 1.8, 1.8, 1.8)
        EntitySetCollisionHandle(enemy, "EntityEnemyCollisionHandle")
        EntitySetCollisionFilter(enemy, EnemyFilter, PlayerFilter)
        EntityAddDataCom(enemy)
        EntityAddFloatData(enemy, "HP", 5.0)
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

    if LEVEL_COUNT == 30:
        DebugPrint("TD Level0 Finish\n")