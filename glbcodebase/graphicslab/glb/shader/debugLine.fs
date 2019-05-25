//-----------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2018. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2018 / 07 / 03
// Brief: Draw debug color line
// Update[2019-5-25]: Support sampler2DMS
//-----------------------------------------------------------
#version 450

in vec3 vs_Color;
in vec4 vs_Vertex;
out vec3 oColor;

uniform sampler2DMS glb_unif_DepthMap;

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
    vec3 ndc = vs_Vertex.xyz / vs_Vertex.w;
    ndc = (ndc.xyz + vec3(1.0, 1.0, 1.0)) / 2.0;
    vec2 uv = ndc.xy;
    float gameDepth = msaaResolve(glb_unif_DepthMap, uv).r;
    if (gameDepth <= ndc.z) discard;

	oColor = vs_Color;
}