//-----------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016 / 07 / 24
// Brief: Get high light
//-----------------------------------------------------------
#version 330

in vec2 vs_texcoord;
out vec4 oColor;

uniform sampler2D glb_unif_HDRSceneTex;
uniform float glb_unif_HighLightBase;

void main() {
	vec3 color = texture(glb_unif_HDRSceneTex, vs_texcoord).xyz;
	float lum = color.x * 0.27 + color.y * 0.67 + color.z * 0.06;
	if (lum > glb_unif_HighLightBase) {
		oColor = vec4(color, 1.0);
	} else {
		oColor = vec4(0.0, 0.0, 0.0, 1.0);
	}
}