//----------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017 / 02 / 18
// Brief: SSAO
//----------------------------------------------------
#version 330

// Input
in vec2 vs_TexCoord;

// uniform
uniform sampler2D glb_unif_RandRotateMap;
uniform sampler2D glb_unif_DepthMap;
uniform float glb_unif_ScreenWidth;
uniform float glb_unif_ScreenHeight;
uniform float glb_unif_FarClip;

// Output
out vec3 oColor;

// const vec3 kCubeCorner[8] = vec3[]
// (
//     vec3(1.0, 1.0, 1.0),
//     vec3(1.0, 1.0, -1.0),
//     vec3(1.0, -1.0, -1.0),
//     vec3(1.0, -1.0, 1.0),
//     vec3(-1.0, 1.0, 1.0),
//     vec3(-1.0, 1.0, -1.0),
//     vec3(-1.0, -1.0, 1.0),
//     vec3(-1.0, -1.0, -1.0)
// );

void main() {
    // Calculate rotate matrix
    vec2 rotate_tex_coord = vs_TexCoord * vec2(glb_unif_ScreenWidth, glb_unif_ScreenHeight) / 4.0;
    vec2 rotate = texture2D(glb_unif_RandRotateMap, rotate_tex_coord).rg;
    mat4 rot_z;
    rot_z[0][0] = rotate.y;rot_z[0][1] = -rotate.x;rot_z[0][2] = 0.0;rot_z[0][3] = 0.0;
    rot_z[1][0] = rotate.x;rot_z[1][1] = rotate.y;rot_z[1][2] = 0.0;rot_z[1][3] = 0.0;
    rot_z[2][0] = 0.0;rot_z[2][1] = 0.0;rot_z[2][2] = 1.0;rot_z[2][3] = 0.0;
    rot_z[3][0] = 0.0;rot_z[3][1] = 0.0;rot_z[3][2] = 0.0;rot_z[3][3] = 1.0;
    
    mat4 rot_y;
    rot_y[0][0] = rotate.y;rot_y[0][1] = 0.0;rot_y[0][2] = rotate.x;rot_y[0][3] = 0.0;
    rot_y[1][0] = 0.0;rot_y[1][1] = 1.0;rot_y[1][2] = 0.0;rot_y[1][3] = 0.0;
    rot_y[2][0] = -rotate.x;rot_y[2][1] = 0.0;rot_y[2][2] = rotate.y;rot_y[2][3] = 0.0;
    rot_y[3][0] = 0.0;rot_y[3][1] = 0.0;rot_y[3][2] = 0.0;rot_y[3][3] = 1.0;
    
    mat4 rot_x;
    rot_x[0][0] = 1.0;rot_x[0][1] = 0.0;rot_x[0][2] = 0.0;rot_x[0][3] = 0.0;
    rot_x[1][0] = 0.0;rot_x[1][1] = rotate.y;rot_x[1][2] = -rotate.x;rot_x[1][3] = 0.0;
    rot_x[2][0] = 0.0;rot_x[2][1] = rotate.x;rot_x[2][2] = rotate.y;rot_x[2][3] = 0.0;
    rot_x[3][0] = 0.0;rot_x[3][1] = 0.0;rot_x[3][2] = 0.0;rot_x[3][3] = 1.0;
    
    mat4 rot_zxy = rot_y * rot_x * rot_z;
    
    // Get current pixel's depth value in view space
    float depth = texture2D(glb_unif_DepthMap, vs_TexCoord).r * glb_unif_FarClip;
    
    // Accumulate the accessibility
    float offset = 0.01f;
    const float offset_scale = 1.0 + 2.4 / 16;
    float accessibility = 0.0;

    vec3 kCubeCorner[8] = vec3[]
    (
        vec3(1.0, 1.0, 1.0),
        vec3(1.0, 1.0, -1.0),
        vec3(1.0, -1.0, -1.0),
        vec3(1.0, -1.0, 1.0),
        vec3(-1.0, 1.0, 1.0),
        vec3(-1.0, 1.0, -1.0),
        vec3(-1.0, -1.0, 1.0),
        vec3(-1.0, -1.0, -1.0)
    );
    
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 8; j++) {
            //vec3 sampler_v = kCubeCorner[j];
            vec3 sampler_v = (rot_zxy * vec4(kCubeCorner[j], 0.0)).xyz;
            sampler_v = normalize(sampler_v);
            sampler_v *= offset;
            offset *= offset_scale;
            sampler_v.xy += vs_TexCoord;
            float sampler_depth = texture2D(glb_unif_DepthMap, sampler_v.xy).r * glb_unif_FarClip;
            if (sampler_v.z * glb_FarClip + depth > sampler_depth) {  // Invalid
                continue;
            }
            
            accessibility += 1.0;
        }
    }
    
    accessibility /= 16.0;
    
    oColor = vec3(accessibility, accessibility, accessibility);
}