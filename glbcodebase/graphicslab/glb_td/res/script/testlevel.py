#!/usr/env/bin python32

"""
Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
Author: i_dovelemon[1322600812@qq.com]
Date: 2017/05/26
Brief: Test level
"""

from host_api import *

def main():
    DebugPrint("HelloWorld\n")
    
    # Create camera entity
    cam = EntityCreate()
    EntityAddCameraCom(cam, 0.0, 10.0, -10.0, 0.0, 0.0, 0.0)

    # Create ground entity
    ground = EntityCreate()
    EntityAddTransformCom(ground, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0)
    EntityAddRenderCom(ground, "res\model\TD_L1\Ground\TD_L1_Ground.obj")
