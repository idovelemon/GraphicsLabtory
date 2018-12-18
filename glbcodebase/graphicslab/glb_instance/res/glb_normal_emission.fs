#version 330


//----------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016 / 10 / 15
// Brief: This is a uber shader, all the light calculation, texture mapping
// and some sort of that will be implemented in this shader.
//----------------------------------------------------


//----------------------------------------------------
// begin: Build-in parameter uniforms
//----------------------------------------------------

// Camera setting
uniform vec3 glb_unif_EyePos;
uniform vec3 glb_unif_LookAt;

// Global light setting
uniform vec3 glb_unif_GlobalLight_Ambient;

// Parallel light setting
uniform vec3 glb_unif_ParallelLight_Dir;
uniform vec3 glb_unif_ParallelLight;

// Image based light settting
uniform sampler2D glb_unif_BRDFPFTTex;
uniform float glb_unif_SpecularPFCLOD;

//----------------------------------------------------
// end: Build-in parameter uniforms
//----------------------------------------------------

//----------------------------------------------------
// begin: Constant
//----------------------------------------------------

const float glbConstPI = 3.1415926;

//----------------------------------------------------
// end: Constant
//----------------------------------------------------

//----------------------------------------------------
// begin: Method about texture
//----------------------------------------------------

//----------------------------------------------------
// Fetech cube map texel
// cube: Cube map
// n: Sampler direction vector
// return: Feteched texel
//----------------------------------------------------
vec3 glbFetechCubeMap(samplerCube cube, vec3 n) {
    n.yz = -n.yz;
    return texture(cube, n).xyz;
}

//----------------------------------------------------
// Fetech cube map texel with lod
// cube: Cube map
// n: Sampler direction vector
// lod: Mip map level
// return: Feteched texel
//----------------------------------------------------
vec3 glbFetechCubeMapLOD(samplerCube cube, vec3 n, float lod) {
    n.yz = -n.yz;
    return textureLod(cube, n, lod).xyz;
}

//----------------------------------------------------
// end: Method about texture
//----------------------------------------------------

//----------------------------------------------------
// begin: Method about light calculating
//----------------------------------------------------

//----------------------------------------------------
// Calculate pixel's world space normal
// normalMap: The tangent space normal normalMap
// uv: Texture coordinate
// t: World space coordinate of tangent space base axis - Tangent
// b: World space coordinate of tangent space base axis - Binormal
// n: World space coordinate of tangent space base axis - Normal
// return: World space normal
//----------------------------------------------------
vec3 glbCalculateWorldNormalFromTex(sampler2D normalMap, vec2 uv, vec3 t, vec3 b, vec3 n) {
	vec3 nw = texture2D(normalMap, uv).xyz;
	nw -= vec3(0.5, 0.5, 0.5);
	nw *= 2.0;
	nw = normalize(nw);

	mat3 tbn = mat3(normalize(t), normalize(b), normalize(n));
	nw = tbn * nw;
	nw = normalize(nw);

    return nw;
}

//----------------------------------------------------
// Calculate fresnel reflection factor
// n: Normal vector
// v: View vector
// F0: Fresnel factor
// return: Fresnel reflection factor
//----------------------------------------------------
vec3 glbCalculateFresnel(vec3 n, vec3 v, vec3 F0) {
    float ndotv = max(dot(n, v), 0.0);
    return F0 + (vec3(1.0, 1.0, 1.0) - F0) * pow(1.0 - ndotv, 5.0);
}

//----------------------------------------------------
// Calculate normal distribution factor
// n: Normal vector
// h: Half vector
// roughness: Material roughness
// return: Normal distribution factor
//----------------------------------------------------
float glbCalculateNDFGGX(vec3 n, vec3 h, float roughness) {
    float a = roughness * roughness;
    float a2 = a * a;
    float ndoth = max(dot(n, h), 0.0);
    float ndoth2 = ndoth * ndoth;
    float t = ndoth2 * (a2 - 1.0) + 1.0;
    float t2 = t * t;
    return a2 / (glbConstPI * t2);
}

//----------------------------------------------------
// Calculate GGX shadow and mask factor
// return: Shadow and mask geometry factor
//----------------------------------------------------
float glbCalculateGeometryGGX(float costheta, float roughness) {
    float a = roughness;
    float r = a + 1.0;
    float r2 = r * r;
    float k = r2 / 8.0;

    float t = costheta * (1.0 - k) + k;

    return costheta / t;
}

//----------------------------------------------------
// Calculate smith shadow and mask factor
// n: Normal vector
// v: View vector
// l: Light vector
// roughness: Material roughness
// return: Shadow and mask geometry factor
//----------------------------------------------------
float glbCalculateGeometrySmith(vec3 n, vec3 v, vec3 l, float roughness) {
    float ndotv = max(dot(n, v), 0.0);
    float ndotl = max(dot(n, l), 0.0);
    float ggx1 = glbCalculateGeometryGGX(ndotv, roughness);
    float ggx2 = glbCalculateGeometryGGX(ndotl, roughness);
    return ggx1 * ggx2;
}

//----------------------------------------------------
// Calculate pixel's direct light color
// n: Normal vector
// v: View vector
// l: Light vector
// h: Half vector
// albedo: Material base color
// roughness: Material roughness value
// metallic: Material metallic value
// light: Light color
// return: Direct light color
//----------------------------------------------------
vec3 glbCalculateDirectLightColor(vec3 n, vec3 v, vec3 l, vec3 h, vec3 albedo, float roughness, float metalic, vec3 light) {
	vec3 result = vec3(0.0, 0.0, 0.0);

    vec3 F0 = mix(vec3(0.04, 0.04, 0.04), albedo, metalic);
    vec3 F = glbCalculateFresnel(h, v, F0);

    vec3 T = vec3(1.0, 1.0, 1.0) - F;
    vec3 kD = T * (1.0 - metalic);

    float D = glbCalculateNDFGGX(n, h, roughness);

    float G = glbCalculateGeometrySmith(n, v, l, roughness);

    vec3 Diffuse = kD * albedo * vec3(1.0 / glbConstPI, 1.0 / glbConstPI, 1.0 / glbConstPI);
    float t = 4.0 * max(dot(n, v), 0.0) * max(dot(n, l), 0.0) + 0.001;
    vec3 Specular = D * F * G * vec3(1.0 / t, 1.0 / t, 1.0 / t);

    float ndotl = max(dot(n, l), 0.0);
	result = (Diffuse + Specular) * light * (ndotl);

	return result;
}

//----------------------------------------------------
// Calculate fresnel reflection factor for IBL
// n: Normal vector
// v: View vector
// F0: Fresnel factor
// roughness: Material roughness
// return: Fresnel reflection factor
//----------------------------------------------------
vec3 glbCalculateFresnelRoughness(vec3 n, vec3 v, vec3 F0, float roughness) {
    float ndotv = max(dot(n, v), 0.0);
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - ndotv, 5.0);
}

//----------------------------------------------------
// Calculate pixel's indirect light color from image based light
// n: Normal vector
// v: View vector
// albedo: Material base color
// roughness: Material roughness value
// metallic: Material metallic value
// return: Indirect color from image based lighting
//----------------------------------------------------
vec3 glbCalcIBLColor(vec3 n, vec3 v, vec3 albedo, float roughness, float metalic, samplerCube diffusePFC, samplerCube specularPFC, sampler2D brdfLookup, float specularLOD) {
	vec3 result = vec3(0.0, 0.0, 0.0);	

    vec3 F0 = mix(vec3(0.04, 0.04, 0.04), albedo, metalic);
    vec3 F = glbCalculateFresnelRoughness(n, v, F0, roughness);

    // Diffuse part
    vec3 T = vec3(1.0, 1.0, 1.0) - F;
    vec3 kD = T * (1.0 - metalic);

    vec3 irradiance = glbFetechCubeMap(diffusePFC, n);
    vec3 diffuse = kD * albedo * irradiance;

    // Specular part
    float ndotv = max(0.0, dot(n, v));
    vec3 r = 2.0 * ndotv * n - v;
    vec3 ld = glbFetechCubeMapLOD(specularPFC, r, roughness * specularLOD);
    vec2 dfg = textureLod(brdfLookup, vec2(ndotv, roughness), 0.0).xy;
    vec3 specular = ld * (F0 * dfg.x + dfg.y);

    result = diffuse + specular;

	return result;
}

//----------------------------------------------------
// end: Method about light calculating
//----------------------------------------------------


// Input attributes
in vec4 vs_Vertex;
in vec3 vs_Normal;
in vec3 vs_Tangent;
in vec3 vs_Binormal;
in vec2 vs_TexCoord;

// Output color
out vec4 oColor;

// Uniform
uniform vec3 glb_unif_EmissionIntensity;
uniform sampler2D glb_unif_AlbedoTex;
uniform sampler2D glb_unif_RoughnessTex;
uniform sampler2D glb_unif_MetallicTex;
uniform sampler2D glb_unif_NormalTex;
uniform sampler2D glb_unif_EmissionTex;
uniform samplerCube glb_unif_DiffusePFCTex;
uniform samplerCube glb_unif_SpecularPFCTex;

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
	emission = texture(glb_unif_EmissionTex, vs_TexCoord).xyz;
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

	vec3 ibl_color = glbCalcIBLColor(normalInWorld, view, albedo, roughness, metallic, glb_unif_DiffusePFCTex, glb_unif_SpecularPFCTex, glb_unif_BRDFPFTTex, glb_unif_SpecularPFCLOD);

	oColor.xyz = (direct_color + ibl_color) * shadow_factor + glb_unif_EmissionIntensity * emission;

	float alpha = 1.0;
	oColor.w = alpha;
}