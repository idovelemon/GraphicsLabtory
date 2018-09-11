//-----------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2018. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2018 / 07 / 03
// Brief: Draw debug color line
//-----------------------------------------------------------
#version 450

in vec3 vs_Color;
in vec4 vs_Vertex;
out vec3 oColor;

uniform sampler2D glb_unif_DepthMap;

void main() {
    vec3 ndc = vs_Vertex.xyz / vs_Vertex.w;
    ndc = (ndc.xyz + vec3(1.0, 1.0, 1.0)) / 2.0;
    vec2 uv = ndc.xy;
    float gameDepth = texture(glb_unif_DepthMap, uv).r;
    if (gameDepth <= ndc.z) discard;

	oColor = vs_Color;
}