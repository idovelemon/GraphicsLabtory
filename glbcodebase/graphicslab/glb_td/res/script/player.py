#!/usr/env/bin python32

"""
Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
Author: i_dovelemon[1322600812@qq.com]
Date: 2017/05/30
Brief: Test level player AI
"""

from config import *
from common import *
from host_api import *

def create_bomb(id, px, py, pz):
    bomb = EntityCreate()
    EntityAddTransformCom(bomb, px, py, pz, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0)
    EntityAddRoleCom(bomb, RMT_PLAYER, RST_BULLET_BOMB)
    EntityAddRenderCom(bomb, "res\model\Bullet\TD_Bullet_Bomb.obj", px, py, pz, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0)
    EntityAddScriptCom(bomb, "btbomb")
    DebugPrint("Throw a bomb!\n")

def col_with_crystal(id, crystal):
    px = EntityGetPosX(id)
    py = EntityGetPosY(id)
    pz = EntityGetPosZ(id)
    pw = EntityGetCollisionWidth(id)
    pl = EntityGetCollisionLength(id)
    cx = EntityGetPosX(crystal)
    cy = EntityGetPosY(crystal)
    cz = EntityGetPosZ(crystal)
    cw = EntityGetCollisionWidth(crystal)
    cl = EntityGetCollisionLength(crystal)
    direction = query_collision_direction(cx, cz, px, pz, pw, pl)
    if direction is COLLISION_DIRECTION_LEFT:
        px = cx - cw / 2.0 - pw / 2.0 - COLLISION_RESPONSE_EP
    elif direction is COLLISION_DIRECTION_RIGHT:
        px = cx + cw / 2.0 + pw / 2.0 + COLLISION_RESPONSE_EP
    elif direction is COLLISION_DIRECTION_UP:
        pz = cz - cl / 2.0 - pl / 2.0 - COLLISION_RESPONSE_EP
    elif direction is COLLISION_DIRECTION_DOWN:
        pz = cz + cl / 2.0 + pl / 2.0 + COLLISION_RESPONSE_EP
    EntitySetPos(id, px, py, pz)
    EntityUpdateCollision(id)
    DebugPrint("coll craystal\n")

def collision_response(id):
    EntityCollisionBeginIterate(id)
    while True:
        coll_id = EntityCollisionIterate(id)
        if coll_id == -1:
            break
        if EntityIsMainType(coll_id, RMT_PLAYER):
            if EntityIsSubType(coll_id, RST_CRYSTAL):
                col_with_crystal(id, coll_id)
                break
    EntityCollisionEndIterate(id)

def main(id):
    # Normal Attack
    if InputHasKeyPressed(BK_Z):
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