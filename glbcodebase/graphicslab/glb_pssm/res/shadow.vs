//----------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016 / 12 / 01
// Brief: Cast Shadow
//----------------------------------------------------
#version 330

in vec3 glb_attr_Pos;
in vec3 glb_attr_Normal;

uniform mat4 glb_unif_ShadowM0;
uniform mat4 glb_unif_ShadowM1;
uniform mat4 glb_unif_ShadowM2;
uniform mat4 glb_unif_ShadowM3;
uniform mat4 glb_unif_WorldM;
uniform mat4 glb_unif_Trans_Inv_WorldM;
uniform int glb_unif_ShadowMIndex;

out vec3 vs_Vertex;
out vec3 vs_Normal;

void main() {
	mat4 shadowM;
	if (glb_unif_ShadowMIndex == 0) {
		shadowM = glb_unif_ShadowM0;
	} else if (glb_unif_ShadowMIndex == 1) {
		shadowM = glb_unif_ShadowM1;
	} else if (glb_unif_ShadowMIndex == 2) {
		shadowM = glb_unif_ShadowM2;
	} else {
		shadowM = glb_unif_ShadowM3;
	}
	gl_Position = shadowM * glb_unif_WorldM * vec4(glb_attr_Pos, 1.0);
	vs_Vertex = vec3(gl_Position.xyz) / gl_Position.w;
	vs_Normal = (glb_unif_Trans_Inv_WorldM * vec4(glb_attr_Normal, 0.0)).xyz;
}