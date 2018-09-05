"""
Declaration: Copyright (c), by idovelemoen, 2018. All right reserved.
Author: idovelemon[1322600812@qq.com]
Date: 2018-09-05
Brief: Enemy relative method
"""

from host_api import *
import config

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