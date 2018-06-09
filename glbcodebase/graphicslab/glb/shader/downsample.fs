//-----------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2018. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2018 / 06 / 05
// Brief: Down-sample
//-----------------------------------------------------------
#version 330

in vec2 vs_texcoord;

uniform sampler2D glb_Tex;

out vec3 oColor;

void main() {
    oColor = texture(glb_Tex, vs_texcoord).xyz;
}