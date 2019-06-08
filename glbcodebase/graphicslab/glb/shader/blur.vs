//--------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016 / 06 / 29
// Brief: Gauss blur pass through vertex shader
//--------------------------------------------------------------------
#version 330

in vec3 glb_attr_Pos;
in vec2 glb_attr_TexCoord;
out vec2 vs_texcoord;

void main() {
    gl_Position = vec4(glb_attr_Pos, 1.0);
    vs_texcoord = glb_attr_TexCoord;
}