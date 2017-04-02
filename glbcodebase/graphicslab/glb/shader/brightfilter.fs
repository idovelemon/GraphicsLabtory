//-----------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016 / 07 / 24
// Brief: Get high light
//-----------------------------------------------------------
#version 330

in vec2 vs_texcoord;
out vec4 oColor;

uniform float glb_AverageLum;
uniform sampler2D glb_HDRSceneTex;
uniform float glb_HighLightBase;

void main() {
	vec3 hdr_color = textureLod(glb_HDRSceneTex, vs_texcoord, 1).xyz;
	float lum = hdr_color.x * 0.27 + hdr_color.y * 0.67 + hdr_color.z * 0.06;
	float lum_after_tonemapping = lum / glb_AverageLum;
	lum_after_tonemapping = lum_after_tonemapping / (1.0 + lum_after_tonemapping);
	hdr_color =  hdr_color * lum_after_tonemapping;
	if (lum_after_tonemapping > glb_HighLightBase) {
		oColor = vec4(hdr_color, 1.0);
	} else {
		oColor = vec4(0.0, 0.0, 0.0, 1.0);
	}
}