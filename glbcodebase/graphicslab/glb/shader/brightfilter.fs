//-----------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016 / 07 / 24
// Brief: Get high light
// Update[2019-5-25]: Support sampler2DMS
//-----------------------------------------------------------
#version 450

in vec2 vs_texcoord;
out vec4 oColor;

uniform sampler2DMS glb_unif_HDRSceneTex;
uniform float glb_unif_HighLightBase;

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
	vec3 color = msaaResolve(glb_unif_HDRSceneTex, vs_texcoord).xyz;
	float lum = color.x * 0.27 + color.y * 0.67 + color.z * 0.06;
	if (lum > glb_unif_HighLightBase) {
		oColor = vec4(color, 1.0);
	} else {
		oColor = vec4(0.0, 0.0, 0.0, 1.0);
	}
}