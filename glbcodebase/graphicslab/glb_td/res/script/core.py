"""
Declaration: Copyright (c), by idovelemoen, 2018. All right reserved.
Author: idovelemon[1322600812@qq.com]
Date: 2018-06-21
Brief: Core's routine
"""

from sys import *
from host_api import *

def main(entity):
    rotX = EntityGetRotX(entity)
    rotY = EntityGetRotY(entity)
    rotZ = EntityGetRotZ(entity)
    rotY = rotY + 1.0 * TimeGetGameFrameSpeed()
    EntitySetRot(entity, rotX, rotY, rotZ)
