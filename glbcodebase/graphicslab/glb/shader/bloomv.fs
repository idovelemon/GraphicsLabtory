//--------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016 / 06 / 29
// Brief: Gauss blur vertical pass shader
//--------------------------------------------------------------------
#version 330

in vec2 vs_texcoord;
out vec3 color;

uniform sampler2D glb_unif_BloomTex;
uniform float glb_unif_BloomTexHeight;

// const float kGaussNum[10] = {
// 0.19947139242361153,
// 0.17603290076919906,
// 0.12098555593111537,
// 0.06475892872301567,
// 0.02699555370849171,
// 0.008764179751319668,
// 0.002215933715394386,
// 0.000436343696989634,
// 6.691555758028252e-05,
// 7.991935088595893e-06,
// };
float kGaussNum[10] = float[]
(
0.19947139242361153,
0.17603290076919906,
0.12098555593111537,
0.06475892872301567,
0.02699555370849171,
0.008764179751319668,
0.002215933715394386,
0.000436343696989634,
6.691555758028252e-05,
7.991935088595893e-06
);

void main() {
    color = texture(glb_unif_BloomTex, vs_texcoord).xyz * kGaussNum[0];
    float step = 1.0 / glb_unif_BloomTexHeight;

    for (int i = 1; i <= 5; i++) {
        if (vs_texcoord.y - i * step >= 0.0) {
            color += texture2D(glb_unif_BloomTex, vec2(vs_texcoord.x, vs_texcoord.y - i * step)).xyz * kGaussNum[i];
        }

        if (vs_texcoord.y + i * step <= 1.0) {
            color += texture2D(glb_unif_BloomTex, vec2(vs_texcoord.x, vs_texcoord.y + i * step)).xyz * kGaussNum[i];
        }
    }
}