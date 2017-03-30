//-----------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017 / 01 / 30
// Brief: Log lumiance pass through shader
//-----------------------------------------------------------
#version 330

in vec2 glb_Pos;
in vec2 glb_TexCoord;
out vec2 vs_texcoord;

void main() {
	gl_Position = vec4(glb_Pos, 0.0, 1.0);
	vs_texcoord = glb_TexCoord;
}