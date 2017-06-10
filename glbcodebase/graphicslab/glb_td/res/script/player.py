#!/usr/env/bin python32

"""
Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
Author: i_dovelemon[1322600812@qq.com]
Date: 2017/05/30
Brief: Test level player AI
"""

from config import *
from host_api import *

def create_bomb(id, px, py, pz):
    bomb = EntityCreate()
    EntityAddTransformCom(bomb, px, py, pz, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0)
    EntityAddRoleCom(bomb, RMT_PLAYER, RST_BULLET_BOMB)
    EntityAddRenderCom(bomb, "res\model\Bullet\TD_Bullet_Bomb.obj")
    EntityAddScriptCom(bomb, "btbomb")
    DebugPrint("Throw a bomb!\n")

def collision_response(id):
    EntityCollisionBeginIterate(id)
    while True:
        coll_id = EntityCollisionIterate(id)
        if coll_id == -1:
            break
        if EntityIsMainType(coll_id, RMT_PLAYER):
            if EntityIsSubType(coll_id, RST_CRYSTAL):
                DebugPrint("Collision With Crystal\n")
                break
    EntityCollisionEndIterate(id)

def main(id):
    # Normal Attack(Auto)
    EntityShoot(id)

    # Movement
    px = EntityGetPosX(id)
    py = EntityGetPosY(id)
    pz = EntityGetPosZ(id)
    speed = EntityGetFloatData(id, "speed")
    time = TimeGetGameFrameSpeed()
    if InputHasKeyPressed(BK_UP):
        pz = pz - speed * time
    elif InputHasKeyPressed(BK_DOWN):
        pz = pz + speed * time
    if InputHasKeyPressed(BK_LEFT):
        px = px - speed * time
    elif InputHasKeyPressed(BK_RIGHT):
        px = px + speed * time
    EntitySetPos(id, px, py, pz)

    # Collision
    EntityUpdateCollision(id)
    EntityCheckCollision(id)
    collision_response(id)

    # Bomb
    delta = EntityGetFloatData(id, "press_delta")
    cur_delta = EntityGetFloatData(id, "cur_press_delta")
    if InputHasKeyPressed(BK_1) and cur_delta > delta:
        create_bomb(id, px, py + 0.1, pz)
        cur_delta = 0.0
    cur_delta = cur_delta + time
    EntitySetFloatData(id, "cur_press_delta", cur_delta)