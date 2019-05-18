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

// Sky light setting
uniform samplerCube glb_unif_DiffuseSkyCubeMap;
uniform samplerCube glb_unif_SpecularSkyCubeMap;
uniform float glb_unif_SpecularSkyPFCLOD;
uniform vec3 glb_unif_SkyLight;

// Image based light settting
uniform sampler2D glb_unif_BRDFPFTTex;
uniform float glb_unif_SpecularPFCLOD;

// Shadow map setting
uniform sampler2D glb_unif_ShadowTex0;
uniform sampler2D glb_unif_ShadowTex1;
uniform sampler2D glb_unif_ShadowTex2;
uniform sampler2D glb_unif_ShadowTex3;
uniform mat4 glb_unif_ShadowM0;
uniform mat4 glb_unif_ShadowM1;
uniform mat4 glb_unif_ShadowM2;
uniform mat4 glb_unif_ShadowM3;
uniform float glb_unif_ShadowSplit0;
uniform float glb_unif_ShadowSplit1;
uniform float glb_unif_ShadowSplit2;
uniform int glb_unif_ShadowMapWidth;
uniform int glb_unif_ShadowMapHeight;

// Decal map setting
uniform mat4 glb_unif_DecalViewM;
uniform mat4 glb_unif_DecalProjM;
uniform sampler2D glb_unif_DecalTex;

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
// Calculate pixel's sky light color
// n: Normal vector
// v: View vector
// albedo: Material base color
// roughness: Material roughness value
// metallic: Material metallic value
// return: sky light color
//----------------------------------------------------
vec3 glbCalcSkyLightColor(vec3 n, vec3 v, vec3 albedo, float roughness, float metalic) {
	vec3 result = vec3(0.0, 0.0, 0.0);	

    vec3 F0 = mix(vec3(0.04, 0.04, 0.04), albedo, metalic);
    vec3 F = glbCalculateFresnelRoughness(n, v, F0, roughness);

    // Diffuse part
    vec3 T = vec3(1.0, 1.0, 1.0) - F;
    vec3 kD = T * (1.0 - metalic);

    vec3 irradiance = glbFetechCubeMap(glb_unif_DiffuseSkyCubeMap, n);
    vec3 diffuse = kD * albedo * irradiance;

    // Specular part
    float ndotv = max(0.0, dot(n, v));
    vec3 r = 2.0 * ndotv * n - v;
    vec3 ld = glbFetechCubeMapLOD(glb_unif_SpecularSkyCubeMap, r, roughness * glb_unif_SpecularSkyPFCLOD);
    vec2 dfg = textureLod(glb_unif_BRDFPFTTex, vec2(ndotv, roughness), 0.0).xy;
    vec3 specular = ld * (F0 * dfg.x + dfg.y);

    result = (diffuse + specular) * glb_unif_SkyLight;

	return result;
}

//----------------------------------------------------
// end: Method about light calculating
//----------------------------------------------------

//----------------------------------------------------
// begin: Method about shadow calculating
//----------------------------------------------------

//----------------------------------------------------
// Calculate shadow map index
// pos: The vertex position
// eyePos: Camera position
// lookAt: Camera look at vector
// split0-2: PSSM split value 0,1,2
// return: Shadow map index
//----------------------------------------------------
int glbCalculateShadowIndex(vec3 pos, vec3 eyePos, vec3 lookAt, float sp0, float sp1, float sp2) {
    int index = -1;

    vec3 toVtx = eyePos - pos;
    float z = dot(toVtx, lookAt);
    if (z < sp0) {
        index = 0;
    } else if (sp0 < z && z < sp1) {
        index = 1;
    } else if (sp1 < z && z < sp2) {
        index = 2;
    } else if (z > sp2) {
        index = 3;
    }

    return index;
}

//----------------------------------------------------
// Calculate pixel shadow factor
// pos: The vertex position
// eyePos: Camera position
// lookAt: Camera look at vector
// split0-2: PSSM split value 0,1,2
// shadowM0-3: PSSM shadow matrix 0,1,2,3
// shadowMap0-3: PSSM shadow map 0,1,2,3
// return: Pixel shadow factor [0 or 1]
//----------------------------------------------------
float glbCalculateShadowFactor(vec3 pos, vec3 eyePos, vec3 lookAt, 
                            float split0, float split1, float split2,
                            mat4 shadowM0, mat4 shadowM1, mat4 shadow2, mat4 shadow3,
                            sampler2D shadowMap0, sampler2D shadowMap1, sampler2D shadowMap2, sampler2D shadowMap3) {
	float shadowFactor = 1.0;

	int index = glbCalculateShadowIndex(pos, eyePos, lookAt, split0, split1, split2);
	vec4 lightSpacePos;

	if (index == 0) {
		lightSpacePos = shadowM0 * vec4(pos, 1.0);
		lightSpacePos.xyz /= lightSpacePos.w;
		lightSpacePos.xyz /= 2.0f;
		lightSpacePos.xyz += 0.5f;
		shadowFactor = texture2D(shadowMap0, lightSpacePos.xy).z;
	} else if (index == 1) {
		lightSpacePos = shadowM1 * vec4(pos, 1.0);
		lightSpacePos.xyz /= lightSpacePos.w;
		lightSpacePos.xyz /= 2.0f;
		lightSpacePos.xyz += 0.5f;	
		shadowFactor = texture2D(shadowMap1, lightSpacePos.xy).z;
	} else if (index == 2) {
		lightSpacePos = shadow2 * vec4(pos, 1.0);
		lightSpacePos.xyz /= lightSpacePos.w;
		lightSpacePos.xyz /= 2.0f;
		lightSpacePos.xyz += 0.5f;				
		shadowFactor = texture2D(shadowMap2, lightSpacePos.xy).z;
	} else {
		lightSpacePos = shadow3 * vec4(pos, 1.0);
		lightSpacePos.xyz /= lightSpacePos.w;
		lightSpacePos.xyz /= 2.0f;
		lightSpacePos.xyz += 0.5f;
		shadowFactor = texture2D(shadowMap3, lightSpacePos.xy).z;
	}

	if (shadowFactor < lightSpacePos.z) {
		// In shadow
		shadowFactor = 0.0;
	} else {
		// Out of shadow
		shadowFactor = 1.0;
	}
	if (lightSpacePos.x < 0.0 || 
		lightSpacePos.x > 1.0 ||
		lightSpacePos.y < 0.0 ||
		lightSpacePos.y > 1.0) {
		// Out of shadow
		shadowFactor = 1.0;
	}

	return shadowFactor;
}

//----------------------------------------------------
// Calculate pixel shadow factor using PCF(Percent Closet Filter)
// pos: The vertex position
// eyePos: Camera position
// lookAt: Camera look at vector
// split0-2: PSSM split value 0,1,2
// shadowM0-3: PSSM shadow matrix 0,1,2,3
// shadowMap0-3: PSSM shadow map 0,1,2,3
// return: Pixel shadow factor [0 - 1]
//----------------------------------------------------
float glbCalculatePCFShadowFactor(vec3 pos, vec3 eyePos, vec3 lookAt, 
                            float split0, float split1, float split2,
                            mat4 shadowM0, mat4 shadowM1, mat4 shadow2, mat4 shadow3,
                            sampler2D shadowMap0, sampler2D shadowMap1, sampler2D shadowMap2, sampler2D shadowMap3) {
	float shadowFactor = 1.0;

	int index = glbCalculateShadowIndex(pos, eyePos, lookAt, split0, split1, split2);
	vec4 lightSpacePos;
	vec2 offset = vec2(1.0 / glb_unif_ShadowMapWidth, 1.0 / glb_unif_ShadowMapHeight);

	if (index == 0) {
		lightSpacePos = shadowM0 * vec4(pos, 1.0);
		lightSpacePos.xyz /= lightSpacePos.w;
		lightSpacePos.xyz /= 2.0f;
		lightSpacePos.xyz += 0.5f;
		float singleShadowFactor = 0.0;
		for (float i = -1.5; i <= 1.5; i = i + 1.0) {
			for (float j = -1.5; j <= 1.5; j = j + 1.0) {
				vec2 newLightSpacePos = lightSpacePos.xy + vec2(offset.x * i, offset.y * j);
				if (newLightSpacePos.x < 0.0 || 
					newLightSpacePos.x > 1.0 ||
					newLightSpacePos.y < 0.0 ||
					newLightSpacePos.y > 1.0) {
					// Out of shadow
					continue;
				}

				float shadowDepth = texture2D(shadowMap0, newLightSpacePos).z;
				if (shadowDepth < lightSpacePos.z) {
					// In shadow
					singleShadowFactor = singleShadowFactor + 1.0;
				}				
			}
		}
		shadowFactor = 1.0 - singleShadowFactor / 16.0;
	} else if (index == 1) {
		lightSpacePos = shadowM1 * vec4(pos, 1.0);
		lightSpacePos.xyz /= lightSpacePos.w;
		lightSpacePos.xyz /= 2.0f;
		lightSpacePos.xyz += 0.5f;
		float singleShadowFactor = 0.0;
		for (float i = -1.5; i <= 1.5; i = i + 1.0) {
			for (float j = -1.5; j <= 1.5; j = j + 1.0) {
				vec2 newLightSpacePos = lightSpacePos.xy + vec2(offset.x * i, offset.y * j);
				if (newLightSpacePos.x < 0.0 || 
					newLightSpacePos.x > 1.0 ||
					newLightSpacePos.y < 0.0 ||
					newLightSpacePos.y > 1.0) {
					// Out of shadow
					continue;
				}

				float shadowDepth = texture2D(shadowMap1, newLightSpacePos).z;
				if (shadowDepth < lightSpacePos.z) {
					// In shadow
					singleShadowFactor = singleShadowFactor + 1.0;
				}				
			}
		}
		shadowFactor = 1.0 - singleShadowFactor / 16.0;
	} else if (index == 2) {
		lightSpacePos = shadow2 * vec4(pos, 1.0);
		lightSpacePos.xyz /= lightSpacePos.w;
		lightSpacePos.xyz /= 2.0f;
		lightSpacePos.xyz += 0.5f;
		float singleShadowFactor = 0.0;
		for (float i = -1.5; i <= 1.5; i = i + 1.0) {
			for (float j = -1.5; j <= 1.5; j = j + 1.0) {
				vec2 newLightSpacePos = lightSpacePos.xy + vec2(offset.x * i, offset.y * j);
				if (newLightSpacePos.x < 0.0 || 
					newLightSpacePos.x > 1.0 ||
					newLightSpacePos.y < 0.0 ||
					newLightSpacePos.y > 1.0) {
					// Out of shadow
					continue;
				}

				float shadowDepth = texture2D(shadowMap2, newLightSpacePos).z;
				if (shadowDepth < lightSpacePos.z) {
					// In shadow
					singleShadowFactor = singleShadowFactor + 1.0;
				}				
			}
		}
		shadowFactor = 1.0 - singleShadowFactor / 16.0;
	} else {
		lightSpacePos = shadow3 * vec4(pos, 1.0);
		lightSpacePos.xyz /= lightSpacePos.w;
		lightSpacePos.xyz /= 2.0f;
		lightSpacePos.xyz += 0.5f;
		float singleShadowFactor = 0.0;
		for (float i = -1.5; i <= 1.5; i = i + 1.0) {
			for (float j = -1.5; j <= 1.5; j = j + 1.0) {
				vec2 newLightSpacePos = lightSpacePos.xy + vec2(offset.x * i, offset.y * j);
				if (newLightSpacePos.x < 0.0 || 
					newLightSpacePos.x > 1.0 ||
					newLightSpacePos.y < 0.0 ||
					newLightSpacePos.y > 1.0) {
					// Out of shadow
					continue;
				}

				float shadowDepth = texture2D(shadowMap3, newLightSpacePos).z;
				if (shadowDepth < lightSpacePos.z) {
					// In shadow
					singleShadowFactor = singleShadowFactor + 1.0;
				}				
			}
		}
		shadowFactor = 1.0 - singleShadowFactor / 16.0;
	}

	return shadowFactor;
}

//----------------------------------------------------
// end: Method about shadow calculating
//----------------------------------------------------

//----------------------------------------------------
// begin: Method about decal
//----------------------------------------------------

//----------------------------------------------------
// Calculate pixel's decal color
// decalProj: Decal pass projection matrix
// decalView: Decal pass view matrix
// pos: The world position
// decalTex: The texture hold all decals already
// return: Pixel's decal color with alpha
//----------------------------------------------------
vec4 glbCalculateDecalColor(mat4 decalProj, mat4 decalView, vec4 pos, sampler2D decalTex) {
  	vec4 decalTexcoord = decalProj * decalView * pos;
    decalTexcoord.xyz /= 2.0;
    decalTexcoord.xyz += 0.5;
    decalTexcoord.xyz /= decalTexcoord.w;

    return texture(decalTex, decalTexcoord.xy);
}

//----------------------------------------------------
// end: Method about decal
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