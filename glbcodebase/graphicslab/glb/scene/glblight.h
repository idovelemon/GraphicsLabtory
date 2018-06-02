//---------------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016 / 06 / 25
// Brief: Lights that used in graphics labtory
//---------------------------------------------------------------------------
#ifndef GLB_GLBLIGHT_H_
#define GLB_GLBLIGHT_H_

#include "math/glbvector.h"

namespace glb {

namespace scene {

enum {
    UNKNOWN_LIGHT = 0,
    AMBIENT_LIGHT,
    PARALLEL_LIGHT,
    POINT_LIGHT,
};

class Light {
public:
    Light(int32_t type)
    : color(0.0f, 0.0f, 0.0f)
    , dir(0.0f, 0.0f, 0.0f) {
        this->type = type;
    }

    Light()
    : type(UNKNOWN_LIGHT)
    , color(0.0f, 0.0f, 0.0f)
    , dir(0.0f, 0.0f, 0.0f) {
    }

    virtual ~Light() {
    }

public:
    int32_t         type;
    math::Vector    color;
    math::Vector    dir;
};

};  // namespace scene

};  // namespace glb

#endif  // GLB_GLBLIGHT_H_