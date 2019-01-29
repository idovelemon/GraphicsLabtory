//----------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2018. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2018 / 06 / 30
// Brief: Cast Shadow
//----------------------------------------------------
#version 330

layout (location = 0) in vec3 glb_attr_Pos;
layout (location = 2) in vec3 glb_attr_Normal;
layout (location = 7) in mat4 glb_attr_WorldMatrix;
layout (location = 11) in mat4 glb_attr_TransInvWorldMatrix;

uniform mat4 glb_unif_ShadowM0;
uniform mat4 glb_unif_ShadowM1;
uniform mat4 glb_unif_ShadowM2;
uniform mat4 glb_unif_ShadowM3;
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
	gl_Position = shadowM * glb_attr_WorldMatrix * vec4(glb_attr_Pos, 1.0);
	vs_Vertex = vec3(gl_Position.xyz);
	vs_Normal = (glb_attr_TransInvWorldMatrix * vec4(glb_attr_Normal, 0.0)).xyz;
}