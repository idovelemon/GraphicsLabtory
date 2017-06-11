#!/usr/env/bin python32

"""
Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
Author: i_dovelemon[1322600812@qq.com]
Date: 2017/05/28
Brief: Config the game
"""

# The size of screen
SCREEN_WIDTH = 800
SCREEN_HEIGHT = 600

# The size of the world
GAME_WORLD_WIDTH = 20
GAME_WORLD_DEPTH = 18

# TEST: Enemy move speed (Per Frame)
E01_SPEED = 0.15
E01_HP = 20.0
E01_COIN = 1
E01_TOTAL_PREPARE_ATTACK_TIME = 2.0
E01_ATTACK_RADIUS = 1.0
E01_ATTACK = 1.0
E01_TOTAL_ATTACK_TIME = 5.0
E01_TOTAL_FINISH_TIME = 2.0
E02_SPEED = 0.06
E02_HP = 100
E02_COIN = 2

# Laser
LASER_ATTACK_DIST = 6.0
LASER_DAMAGE = 3.0

# Machine gun
MACHINE_GUN_SHOOT_DELTA = 5.0
MACHINE_GUN_ATTACK_DIST = 5.0
MACHINE_GUN_DAMAGE = 2.0
MACHINE_GUN_TOTAL_DELTA = 5.0

# Subtower cost coin
SUBTOWER_COST_COIN = 5

# Main tower
MAIN_TOWER_MOVE_SPEED = 0.1
MAIN_TOWER_PRESS_DELTA = 10.0
MAIN_TOWER_HP = 5.0

# Bomb
BOMB_SCALE_SPEED = 0.5
BOMB_MAX_SCALE = 20.0

# Crystal
CRYSTAL_GEN_ITEM_TIME = 10 * 60

if __name__ == "__main__":
    pass