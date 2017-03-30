//--------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017 / 02 / 24
// Brief: Bilaterial Filter Blur V-Pass
// TODO: do not know how to combine depth coefficient now
//--------------------------------------------------------------------
#version 330

in vec2 vs_texcoord;
out vec3 color;

uniform sampler2D glb_BiBlurMap;
uniform float glb_ScreenHeight;

const float kGaussNum[2] = {
0.36166444368093026,
0.3191677781595349,
};

void main() {
    color = texture2D(glb_BiBlurMap, vs_texcoord).xyz * kGaussNum[0];
    float step = 1.0 / glb_ScreenHeight;

    for (int i = 1; i < 2; i++) {
        if (vs_texcoord.y - i * step >= 0.0) {
            color += texture2D(glb_BiBlurMap, vec2(vs_texcoord.x, vs_texcoord.y - i * step)).xyz * kGaussNum[i];
        }

        if (vs_texcoord.y + i * step <= 1.0) {
            color += texture2D(glb_BiBlurMap, vec2(vs_texcoord.x, vs_texcoord.y + i * step)).xyz * kGaussNum[i];
        }
    }
}