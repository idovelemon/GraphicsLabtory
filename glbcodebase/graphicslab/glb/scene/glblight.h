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
    PARALLEL_LIGHT,
    POINT_LIGHT,
};

class Light {
public:
    Light(int32_t type)
    : ambient(0.0f, 0.0f, 0.0f)
    , diffuse(0.0f, 0.0f, 0.0f)
    , specular(0.0f, 0.0f, 0.0f)
    , pos(0.0f, 0.0f, 0.0f)
    , dir(0.0f, 0.0f, 0.0f)
    , pow(0.0f) {
        this->type = type;
    }

    Light()
    :type(UNKNOWN_LIGHT)
    , ambient(0.0f, 0.0f, 0.0f)
    , diffuse(0.0f, 0.0f, 0.0f)
    , specular(0.0f, 0.0f, 0.0f)
    , pos(0.0f, 0.0f, 0.0f)
    , dir(0.0f, 0.0f, 0.0f)
    , pow(0.0f) {
    }

    virtual ~Light() {
    }

public:
    int32_t         type;
    math::Vector    ambient;
    math::Vector    diffuse;
    math::Vector    specular;
    math::Vector    pos;
    math::Vector    dir;
    float           pow;
};

};  // namespace scene

};  // namespace glb

#endif  // GLB_GLBLIGHT_H_