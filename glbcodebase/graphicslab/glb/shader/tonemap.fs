//--------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016 / 07 / 24
// Brief: Tone mapping the HDR scene
//--------------------------------------------------------
#version 330

in vec2 vs_texcoord;
out vec4 color;

uniform float glb_AverageLum;
uniform sampler2D glb_LogLumTex;
uniform sampler2D glb_HDRSceneTex;
uniform float glb_ExposureLevel;

const float kGamma = 2.2;

void main() {
	vec4 hdr_color = texture2D(glb_HDRSceneTex, vs_texcoord);
	vec4 bloom_color = texture2D(glb_LogLumTex, vs_texcoord);
	float lum = hdr_color.x * 0.27 + hdr_color.y * 0.67 + hdr_color.z * 0.06;
	float lum_after_tonemapping = (glb_ExposureLevel * lum) / (glb_AverageLum + 0.0001);
	color.xyz = hdr_color.xyz * lum_after_tonemapping / (1 + hdr_color.xyz);
	color.xyz = color.xyz + bloom_color.xyz * 0.1;

	// Gamma correction
	color.x = pow(color.x, 1.0 / kGamma);
	color.y = pow(color.y, 1.0 / kGamma);
	color.z = pow(color.z, 1.0 / kGamma);
	color.w = 1.0;
}