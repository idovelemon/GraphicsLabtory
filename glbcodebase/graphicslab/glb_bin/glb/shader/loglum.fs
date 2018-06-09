//-----------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017 / 01 / 30
// Brief: Log lumiance shader
//-----------------------------------------------------------
#version 330

in vec2 vs_texcoord;
out vec4 color;

uniform sampler2D glb_HDRSceneTex;

void main() {
	vec3 hdr_color = textureLod(glb_HDRSceneTex, vs_texcoord, 1).xyz;
	float lum = hdr_color.x * 0.27 + hdr_color.y * 0.67 + hdr_color.z * 0.06;
	float log_lum = log(lum + 0.0001);
	color = vec4(log_lum, log_lum, log_lum, 1.0);
}