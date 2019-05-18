#version 330

#include "glb.gsh"

// Input attributes
in vec4 vs_Vertex;
in vec3 vs_Normal;
in vec3 vs_Tangent;
in vec3 vs_Binormal;
in vec2 vs_TexCoord;

// Output color
out vec4 oColor;

// Uniform
uniform vec3 glb_unif_Albedo;

vec3 calc_view() {
	vec3 view = vec3(0.0, 0.0, 0.0);
	view = normalize(glb_unif_EyePos - vs_Vertex.xyz);
	return view;
}

vec3 calc_light_dir() {
	vec3 light_dir = vec3(0.0, 0.0, 0.0);
	light_dir = -glb_unif_ParallelLight_Dir;
	return light_dir;
}

vec3 calc_direct_light_color() {
	vec3 light = vec3(0.0, 0.0, 0.0);
	light = light + glb_unif_ParallelLight;

	return light;
}

void main() {
	oColor = vec4(0.0, 0.0, 0.0, 0.0);

	float shadow_factor = glbCalculateShadowFactor(vs_Vertex.xyz, glb_unif_EyePos, glb_unif_LookAt, 
                            glb_unif_ShadowSplit0, glb_unif_ShadowSplit1, glb_unif_ShadowSplit2,
                            glb_unif_ShadowM0, glb_unif_ShadowM1, glb_unif_ShadowM2, glb_unif_ShadowM3,
                            glb_unif_ShadowTex0, glb_unif_ShadowTex1, glb_unif_ShadowTex2, glb_unif_ShadowTex3);

	oColor.xyz = glb_unif_Albedo * shadow_factor + glb_unif_GlobalLight_Ambient * glb_unif_Albedo;

	float alpha = 1.0;
	oColor.w = alpha;
}