"""
Declaration: Copyright (c), by idovelemoen, 2018. All right reserved.
Author: idovelemon[1322600812@qq.com]
Date: 2018-09-05
Brief: Player relative method
"""

from host_api import *
import config

def PlayerCreate(x, y, z):
    # Create CoreDummy Entity
    coreDummy = EntityCreate()
    EntityAddTransformCom(coreDummy, x, y, z, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0)
    EntityAddScriptCom(coreDummy, "EntityCoreDummyUpdater")
    EntityAddDataCom(coreDummy)
    EntityAddStringData(coreDummy, "Tag", "Player")
    EntityAddRelationshipCom(coreDummy)
    EntityAddCollisionCom(coreDummy, 0.0, 0.0, 0.0, 1.8, 1.8, 1.8)
    EntitySetCollisionHandle(coreDummy, "EntityPlayerCollisionHandle")
    EntitySetCollisionFilter(coreDummy, PlayerFilter, EnemyFilter | EnemyBulletFilter | ObstacleFilter)

    # Create Camera Entity
    camera = EntityCreate()
    EntityAddCameraCom(camera, 0.0, 20.0, 20.0, 0.0, 0.0, 0.0)
    EntityAddTransformCom(camera, 0.0, 20.0, 20.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0)
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
    rightNormalEmitter = EntityCreate()
    EntityAddTransformCom(rightNormalEmitter, 1.8, 0.75, 0.0, 0.0, 90.0, 0.0, 1.0, 1.0, 1.0)
    EntityAddRelationshipCom(rightNormalEmitter)
    EntityBindRelationship(coreRot, rightNormalEmitter)
    EntityAddRenderCom(rightNormalEmitter, "res/model/emitter/normal_Emitter.obj", 1.8, 0.75, 0.0, 0.0, 90.0, 0.0, 1.0, 1.0, 1.0, True, 4)
    EntityAddScriptCom(rightNormalEmitter, "EntityNormalEmitterUpdater")
    EntityAddDataCom(rightNormalEmitter)
    EntityAddFloatData(rightNormalEmitter, "ShootAccelerateBulletDelta", 0.0)
    EntityAddFloatData(rightNormalEmitter, "ShootDeaccelerateBulletDelta", 0.0)