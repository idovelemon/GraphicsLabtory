#version 330

#include "../../glb/shader/glb.gsh"

// Input attributes
in vec4 vs_Vertex;
in vec3 vs_Normal;
in vec3 vs_Tangent;
in vec3 vs_Binormal;
in vec2 vs_TexCoord;

// Output color
out vec4 oColor;

// Uniform
uniform sampler2D glb_unif_AlbedoTex;
uniform sampler2D glb_unif_RoughnessTex;
uniform sampler2D glb_unif_MetallicTex;
uniform sampler2D glb_unif_NormalTex;
uniform sampler2D glb_unif_EmissionTex;
uniform samplerCube glb_unif_DiffusePFCTex;
uniform samplerCube glb_unif_SpecularPFCTex;

uniform vec3 glb_unif_EmissionIntensity;

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

void calc_material(out vec3 albedo, out float roughness, out float metallic, out vec3 emission) {
	albedo = texture(glb_unif_AlbedoTex, vs_TexCoord).xyz;
	roughness = texture(glb_unif_RoughnessTex, vs_TexCoord).x;
	metallic = texture(glb_unif_MetallicTex, vs_TexCoord).x;
	emission = texture(glb_unif_EmissionTex, vs_TexCoord).xyz * glb_unif_EmissionIntensity;
}

vec3 calc_direct_light_color() {
	vec3 light = vec3(0.0, 0.0, 0.0);
	light = light + glb_unif_GlobalLight_Ambient;
	light = light + glb_unif_ParallelLight;

	return light;
}

void main() {
	oColor = vec4(0.0, 0.0, 0.0, 0.0);

	vec3 normalInWorld = vec3(0.0, 0.0, 0.0);
	vec3 normalInTangent = vec3(0.0, 0.0, 0.0);
    normalInWorld = glbCalculateWorldNormalFromTex(glb_unif_NormalTex, vs_TexCoord, vs_Tangent, vs_Binormal, vs_Normal);

	float shadow_factor = 1.0;

	vec3 view = calc_view();

	vec3 light = calc_light_dir();

	vec3 h = normalize(view + light);

	vec3 albedo = vec3(0.0, 0.0, 0.0);
	float roughness = 0.0;
	float metallic = 0.0;
	vec3 emission = vec3(0.0, 0.0, 0.0);
	calc_material(albedo, roughness, metallic, emission);

	vec3 direct_light_color = calc_direct_light_color();

	vec3 direct_color = glbCalculateDirectLightColor(normalInWorld, view, light, h, albedo, roughness, metallic, direct_light_color);

	vec3 sky_light_color = glbCalcSkyLightColor(normalInWorld, view, albedo, roughness, metallic);

	oColor.xyz = (direct_color + sky_light_color) * shadow_factor + emission;

	float alpha = 1.0;
	oColor.w = alpha;
}