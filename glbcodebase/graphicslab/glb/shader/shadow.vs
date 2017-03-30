//----------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016 / 12 / 01
// Brief: Cast Shadow
//----------------------------------------------------
#version 330

in vec3 glb_Pos;
in vec3 glb_Normal;

uniform mat4 glb_ShadowM;
uniform mat4 glb_WorldM;
uniform mat4 glb_Trans_Inv_WorldM;

out vec3 vs_Vertex;
out vec3 vs_Normal;

void main() {
	gl_Position = glb_ShadowM * glb_WorldM * vec4(glb_Pos, 1.0);
	vs_Vertex = vec3(gl_Position.xyz);
	vs_Normal = (glb_Trans_Inv_WorldM * vec4(glb_Normal, 0.0)).xyz;
}