//-------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016 / 09 / 04
// Brief: Define some structures that used to hold the glb data
//-------------------------------------------------------
#ifndef GLB_EDITOR_GLB_H_
#define GLB_EDITOR_GLB_H_

#include <stdint.h>

namespace glb {

struct CommMaterial {
    char material_name[128];
    int32_t id;
    float ambient_x;
    float ambient_y;
    float ambient_z;
    float diffuse_x;
    float diffuse_y;
    float diffuse_z;
    float specular_x;
    float specular_y;
    float specular_z;
    float emission_x;
    float emission_y;
    float emission_z;
    float pow;
};

};  // namespace glb

#endif  // GLB_EDITOR_GLB_H_
