//--------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016 / 07 / 24
// Brief: Tone mapping the HDR scene
// Update[2019-5-25]: Support sampler2DMS
//--------------------------------------------------------
#version 450

in vec2 vs_texcoord;
out vec4 color;

uniform sampler2DMS glb_unif_Tex;
uniform sampler2D glb_unif_BloomTex[4];
uniform float glb_unif_BloomWeight[4];

const float kGamma = 2.2;
const float A = 0.22;
const float B = 0.30;
const float C = 0.10;
const float D = 0.20;
const float E = 0.01;
const float F = 0.30;
const float W = 1.5;

vec3 Uncharted2Tonemap(vec3 x)
{
   return ((x*(A*x+C*B)+D*E)/(x*(A*x+B)+D*F))-E/F;
}

vec4 msaaResolve(sampler2DMS msTex, vec2 uv) {
	ivec2 texSize = textureSize(msTex);
	int samplers = textureSamples(msTex);
    ivec2 coord = ivec2(texSize * uv);

	vec4 color = vec4(0.0, 0.0, 0.0, 0.0);
	for (int i = 0; i < samplers; i++) {
		color = color + texelFetch(msTex, coord, i);
	}

	color /= samplers;

	return color;
}

void main() {
	vec3 hdrColor = msaaResolve(glb_unif_Tex, vs_texcoord).xyz;
	vec3 bloom0Color = texture2D(glb_unif_BloomTex[0], vs_texcoord).xyz;
	vec3 bloom1Color = texture2D(glb_unif_BloomTex[1], vs_texcoord).xyz;
	vec3 bloom2Color = texture2D(glb_unif_BloomTex[2], vs_texcoord).xyz;
	vec3 bloom3Color = texture2D(glb_unif_BloomTex[3], vs_texcoord).xyz;

	hdrColor = hdrColor + bloom0Color * glb_unif_BloomWeight[0] + bloom1Color * glb_unif_BloomWeight[1] + bloom2Color * glb_unif_BloomWeight[2] + bloom3Color * glb_unif_BloomWeight[3];

    // base tone mapping
	hdrColor = Uncharted2Tonemap(hdrColor);

    // gamma correction
    hdrColor = pow(hdrColor, vec3(1.0 / 2.2, 1.0 / 2.2, 1.0 / 2.2));

	color.xyz = hdrColor;
	color.w = 1.0;
}