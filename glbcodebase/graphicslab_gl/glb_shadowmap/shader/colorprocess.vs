//-------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[13226000812@qq.com]
// Date: 2016 / 09 / 12
// Brief: Color process's pass through vertex shader
//-------------------------------------------------------

in vec2 glb_Pos;
in vec2 glb_TexCoord;
out vec2 glb_ps_tex_coord;

void main() {
	gl_Position =  vec4(glb_Pos, 0.0, 1.0);
	glb_ps_tex_coord = glb_TexCoord;
}