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
uniform float glb_unif_Roughness;
uniform float glb_unif_Metallic;
uniform samplerCube glb_unif_DiffusePFC;
uniform samplerCube glb_unif_SpecularPFC;

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

	vec3 normalInWorld = vec3(0.0, 0.0, 0.0);
	vec3 normalInTangent = vec3(0.0, 0.0, 0.0);
    normalInWorld = normalize(vs_Normal);

	float shadow_factor = glbCalculateShadowFactor(vs_Vertex.xyz, glb_unif_EyePos, glb_unif_LookAt, 
                            glb_unif_ShadowSplit0, glb_unif_ShadowSplit1, glb_unif_ShadowSplit2,
                            glb_unif_ShadowM0, glb_unif_ShadowM1, glb_unif_ShadowM2, glb_unif_ShadowM3,
                            glb_unif_ShadowTex0, glb_unif_ShadowTex1, glb_unif_ShadowTex2, glb_unif_ShadowTex3);

	vec3 view = calc_view();

	vec3 light = calc_light_dir();

	vec3 h = normalize(view + light);
	float ndotl = dot(normalInWorld, light);
	if (ndotl < 0.0) shadow_factor = 0.0;

	vec3 albedo = glb_unif_Albedo;
	float roughness = glb_unif_Roughness;
	float metallic = glb_unif_Metallic;
	vec3 emission = vec3(0.0, 0.0, 0.0);

	vec3 direct_light_color = calc_direct_light_color();

	vec3 direct_color = glbCalculateDirectLightColor(normalInWorld, view, light, h, albedo, roughness, metallic, direct_light_color);
	vec3 sky_color = glbCalcSkyLightColor(normalInWorld, view, albedo, roughness, metallic);

	oColor.xyz = (direct_color) * shadow_factor + glb_unif_GlobalLight_Ambient * albedo + sky_color + emission;

	float alpha = 1.0;
	oColor.w = alpha;
}