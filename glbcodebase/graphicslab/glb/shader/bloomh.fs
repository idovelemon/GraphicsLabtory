//--------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016 / 06 / 29
// Brief: Gauss blur horizontal pass shader
//--------------------------------------------------------------------
#version 330

in vec2 vs_texcoord;
out vec4 color;

uniform sampler2D glb_LogLumTex;
uniform float glb_BloomTexWidth;

const float kGaussNum[10] = {
0.19947139242361153,
0.17603290076919906,
0.12098555593111537,
0.06475892872301567,
0.02699555370849171,
0.008764179751319668,
0.002215933715394386,
0.000436343696989634,
6.691555758028252e-05,
7.991935088595893e-06,
};

void main() {
    color.xyz = texture2D(glb_LogLumTex, vs_texcoord).xyz * kGaussNum[0];
    float step = 1.0 / glb_BloomTexWidth;

    for (int i = 1; i < 10; i++) {
        if (vs_texcoord.x - i * step >= 0.0) {
            vec4 tex_color = texture2D(glb_LogLumTex, vec2(vs_texcoord.x - i * step, vs_texcoord.y));
            color.xyz += tex_color.xyz * kGaussNum[i];
            color.w += tex_color.w * kGaussNum[i];
        }

        if (vs_texcoord.x + i * step <= 1.0) {
            vec4 tex_color = texture2D(glb_LogLumTex, vec2(vs_texcoord.x + i * step, vs_texcoord.y));
            color.xyz += tex_color.xyz * kGaussNum[i];
            color.w += tex_color.w * kGaussNum[i];
        }
    }
}