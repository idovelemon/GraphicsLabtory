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
const float A = 0.15;
const float B = 0.50;
const float C = 0.10;
const float D = 0.20;
const float E = 0.02;
const float F = 0.30;
const float W = 11.2;

vec3 Uncharted2Tonemap(vec3 x)
{
   return ((x*(A*x+C*B)+D*E)/(x*(A*x+B)+D*F))-E/F;
}

void main() {
	vec4 hdr_color = texture2D(glb_HDRSceneTex, vs_texcoord);
	vec4 bloom_color = texture2D(glb_LogLumTex, vs_texcoord);
	hdr_color = hdr_color + bloom_color;
	hdr_color *= glb_ExposureLevel;

	// Filmic Tone mapping
	hdr_color.xyz = Uncharted2Tonemap(hdr_color.xyz);
	vec3 whiteScale = vec3(1.0, 1.0, 1.0) / Uncharted2Tonemap(vec3(W, W, W));
	color.xyz = whiteScale * hdr_color.xyz;

	// Gamma correction
	color.x = pow(color.x, 1.0 / kGamma);
	color.y = pow(color.y, 1.0 / kGamma);
	color.z = pow(color.z, 1.0 / kGamma);
	color.w = 1.0;
}