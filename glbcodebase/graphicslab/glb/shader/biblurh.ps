//--------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017 / 02 / 24
// Brief: Bilaterial Filter Blur H-Pass
// TODO: do not know how to combine depth coefficient now
//--------------------------------------------------------------------
#version 330

in vec2 vs_texcoord;
out vec3 color;

uniform sampler2D glb_DepthMap;
uniform sampler2D glb_AOMap;
uniform float glb_ScreenWidth;
uniform float glb_FarClip;

const float kGaussNum[2] = {
0.36166444368093026,
0.3191677781595349,
};

void main() {
    float cur_pixel_depth = texture2D(glb_DepthMap, vs_texcoord).r * glb_FarClip;
    color = texture2D(glb_AOMap, vs_texcoord).xyz * kGaussNum[0] * 1.0;
    float step = 1.0 / glb_ScreenWidth;

    for (int i = 1; i < 2; i++) {
        if (vs_texcoord.x - i * step >= 0.0) {
            float temp_depth = texture2D(glb_DepthMap, vec2(vs_texcoord.x - i * step, vs_texcoord.y)).r * glb_FarClip;
            float cof = 1.0 / (0.01 + abs(cur_pixel_depth - temp_depth));
            //color += texture2D(glb_AOMap, vec2(vs_texcoord.x - i * step, vs_texcoord.y)).xyz * kGaussNum[i] * cof;
            color += texture2D(glb_AOMap, vec2(vs_texcoord.x - i * step, vs_texcoord.y)).xyz * kGaussNum[i];
        }

        if (vs_texcoord.x + i * step <= 1.0) {
            float temp_depth = texture2D(glb_DepthMap, vec2(vs_texcoord.x + i * step, vs_texcoord.y)).r * glb_FarClip;
            float cof = 1.0 / (0.01 + abs(cur_pixel_depth - temp_depth));        
            //color += texture2D(glb_AOMap, vec2(vs_texcoord.x + i * step, vs_texcoord.y)).xyz * kGaussNum[i] * cof;
            color += texture2D(glb_AOMap, vec2(vs_texcoord.x + i * step, vs_texcoord.y)).xyz * kGaussNum[i];
        }
    }
}