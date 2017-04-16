//----------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016 / 10 / 15
// Brief: This is a uber shader, all the light calculation, texture mapping
// and some sort of that will be implemented in this shader.
//----------------------------------------------------
// Input attributes
in vec3 vs_Vertex;

#ifdef GLB_COLOR_IN_VERTEX
in vec3 vs_Color;
#endif

#ifdef GLB_NORMAL_IN_VERTEX
in vec3 vs_Normal;
#endif

#ifdef GLB_TANGENT_IN_VERTEX
in vec3 vs_Tangent;
#endif

#ifdef GLB_BINORMAL_IN_VERTEX
in vec3 vs_Binormal;
#endif

#ifdef GLB_TANGENT_IN_VERTEX
#ifdef GLB_BINORMAL_IN_VERTEX
in vec3 vs_NT;
#endif
#endif

#ifdef GLB_TEXCOORD_IN_VERTEX
in vec2 vs_TexCoord;
#endif

// Output color
out vec4 oColor;

// Uniform
#ifdef GLB_TEXCOORD_IN_VERTEX

	#ifdef GLB_ENABLE_DIFFUSE_TEX
	uniform sampler2D glb_DiffuseTex;
	#endif

	#ifdef GLB_ENABLE_ALPHA_TEX
	uniform sampler2D glb_AlphaTex;
	#endif

	#ifdef GLB_ENABLE_NORMAL_TEX
	uniform sampler2D glb_NormalTex;

		#ifdef GLB_TANGENT_IN_VERTEX
		#ifdef GLB_BINORMAL_IN_VERTEX
			#define GLB_ENABLE_NORMAL_MAPPING
		#endif
		#endif
	#endif

	#ifdef GLB_ENABLE_REFLECT_TEX
		uniform samplerCube glb_ReflectTex;
	#endif

#endif

#ifdef GLB_ENABLE_SHADOW
uniform sampler2D glb_ShadowTex0;
uniform sampler2D glb_ShadowTex1;
uniform sampler2D glb_ShadowTex2;
uniform sampler2D glb_ShadowTex3;
uniform mat4 glb_ShadowM0;
uniform mat4 glb_ShadowM1;
uniform mat4 glb_ShadowM2;
uniform mat4 glb_ShadowM3;
uniform float glb_ShadowSplit0;
uniform float glb_ShadowSplit1;
uniform float glb_ShadowSplit2;

int calc_current_shadow_index(vec3 vtx, vec3 eye_pos, vec3 look_at, float sp0, float sp1, float sp2) {
	int index = -1;
	vec3 to_vtx = eye_pos - vtx;
	float z_value = dot(to_vtx, look_at);
	if (z_value < sp0) {
		index = 0;
	} else if (sp0 < z_value && z_value < sp1) {
		index = 1;
	} else if (sp1 < z_value && z_value < sp2) {
		index = 2;
	} else if (z_value > sp2) {
		index = 3;
	}

	return index;
}
#endif

#ifdef GLB_ENABLE_AO
uniform float glb_ScreenWidth;
uniform float glb_ScreenHeight;
uniform sampler2D glb_AOMap;
#endif

#ifdef GLB_ENABLE_LIGHTING
	#define GLB_ENABLE_EYE_POS
#endif

#ifdef GLB_ENABLE_REFLECT_TEX
	#define GLB_ENABLE_EYE_POS
#endif

#ifdef GLB_ENABLE_EYE_POS
uniform vec3 glb_EyePos;
uniform vec3 glb_LookAt;
#endif

uniform vec3 glb_Material_Emission;

#ifdef GLB_ENABLE_LIGHTING
uniform vec3 glb_Material_Ambient;
uniform vec3 glb_Material_Diffuse;
uniform vec3 glb_Material_Specular;
uniform float glb_Material_Pow;

	#ifdef GLB_USE_PARALLEL_LIGHT
	uniform vec3 glb_ParallelLight_Dir;
	uniform vec3 glb_ParallelLight_Ambient;
	uniform vec3 glb_ParallelLight_Diffuse;
	uniform vec3 glb_ParallelLight_Specular;

	vec3 calc_diffuse(vec3 light_vec, vec3 normal, vec3 diffuse_mat, vec3 light_color) {
		float ratio = dot(light_vec, normal);
		ratio = max(ratio, 0.0);
		return light_color * diffuse_mat * ratio;
	}

	vec3 calc_specular(vec3 light_vec, vec3 normal, vec3 view_vec, vec3 spec_mat, vec3 light_color, float pow_value) {
		/*******************************************
		* reflect(incident light vector, surface normal)
		* incident_light_vec = -light_vec
		********************************************/
		vec3 ref_light_vec = reflect(-light_vec, normal);
		float ratio = dot(ref_light_vec, view_vec);
		ratio = max(ratio, 0.0);
		ratio = pow(ratio, pow_value);

		return light_color * spec_mat * ratio;
	}
	#else
		#error No light source specified
	#endif

#endif  // GLB_ENABLE_LIGHTING

//-----------------------------------------------------------------------------------------------
/*******************************************
* Control how to combine all colors
********************************************/
#ifdef GLB_ENABLE_LIGHTING
#ifndef GLB_COLOR_IN_VERTEX
#ifndef GLB_TEXCOORD_IN_VERTEX

#define GLB_COMBINE_COLOR_SUPPORT
vec3 combine_color(vec3 light, vec3 reflect_light, vec3 diffuse_color, vec3 vtx_color) {
	return light;
}

#endif
#endif
#endif

#ifdef GLB_ENABLE_LIGHTING
#ifdef GLB_TEXCOORD_IN_VERTEX
#ifndef GLB_COLOR_IN_VERTEX
#ifndef GLB_ENABLE_DIFFUSE_TEX

#define GLB_COMBINE_COLOR_SUPPORT
vec3 combine_color(vec3 light, vec3 reflect_light, vec3 diffuse_color, vec3 vtx_color) {
	return light;
}

#endif
#endif
#endif
#endif

#ifdef GLB_ENABLE_LIGHTING
#ifndef GLB_COLOR_IN_VERTEX
#ifdef GLB_ENABLE_DIFFUSE_TEX
#ifndef GLB_ENABLE_REFLECT_TEX

#define GLB_COMBINE_COLOR_SUPPORT
vec3 combine_color(vec3 light, vec3 reflect_light, vec3 diffuse_color, vec3 vtx_color) {
	return light * diffuse_color;
}

#endif
#endif
#endif
#endif

#ifdef GLB_ENABLE_REFLECT_TEX
#ifndef GLB_ENABLE_DIFFUSE_TEX
#ifndef GLB_ENABLE_LIGHTING
#endif
#endif
#endif

#ifdef GLB_COLOR_IN_VERTEX
#ifndef GLB_ENABLE_LIGHTING
#ifndef GLB_TEXCOORD_IN_VERTEX

#define GLB_COMBINE_COLOR_SUPPORT
vec3 combine_color(vec3 light, vec3 reflect_light, vec3 diffuse_color, vec3 vtx_color) {
	return vtx_color;
}

#endif
#endif
#endif

#ifdef GLB_TEXCOORD_IN_VERTEX
#ifdef GLB_ENABLE_DIFFUSE_TEX
#ifndef GLB_ENABLE_LIGHTING
#ifndef GLB_COLOR_IN_VERTEX
#ifndef GLB_ENABLE_REFLECT_TEX

#define GLB_COMBINE_COLOR_SUPPORT
vec3 combine_color(vec3 light, vec3 reflect_light, vec3 diffuse_color, vec3 vtx_color) {
	return diffuse_color;
}

#endif
#endif
#endif
#endif
#endif

#ifndef GLB_ENABLE_LIGHTING
#ifndef GLB_TEXCOORD_IN_VERTEX
#ifndef GLB_COLOR_IN_VERTEX

#define GLB_COMBINE_COLOR_SUPPORT
vec3 combine_color(vec3 light, vec3 reflect_light, vec3 diffuse_color, vec3 vtx_color) {
	return vec3(0.0, 0.0, 0.0);
}

#endif
#endif
#endif

#ifdef GLB_TEXCOORD_IN_VERTEX
#ifndef GLB_ENABLE_LIGHTING
#ifndef GLB_COLOR_IN_VERTEX
#ifndef GLB_ENABLE_DIFFUSE_TEX
#ifndef GLB_ENABLE_REFLECT_TEX

#define GLB_COMBINE_COLOR_SUPPORT
vec3 combine_color(vec3 light, vec3 reflect_light, vec3 diffuse_color, vec3 vtx_color) {
	return vec3(0.0, 0.0, 0.0);
}

#endif
#endif
#endif
#endif
#endif

#ifdef GLB_TEXCOORD_IN_VERTEX
#ifdef GLB_ENABLE_REFLECT_TEX
#ifndef GLB_ENABLE_LIGHTING
#ifndef GLB_ENABLE_DIFFUSE_TEX
#ifndef GLB_COLOR_IN_VERTEX

#define GLB_COMBINE_COLOR_SUPPORT
vec3 combine_color(vec3 light, vec3 reflect_light, vec3 diffuse_color, vec3 vtx_color) {
	return reflect_light;
}

#endif
#endif
#endif
#endif
#endif

#ifndef GLB_COMBINE_COLOR_SUPPORT
	#error No support color combine method
#endif
//-----------------------------------------------------------------------------------------------

void main() {
	oColor = vec4(0.0, 0.0, 0.0, 1.0);

	vec3 diffuse_light = vec3(0.0, 0.0, 0.0);
	vec3 specular_light = vec3(0.0, 0.0, 0.0);
	vec3 ambient_light = vec3(0.0, 0.0, 0.0);
	vec3 reflect_light = vec3(0.0, 0.0, 0.0);
	vec3 emii_light = glb_Material_Emission;

	float is_in_shadow = 0.0;
#ifdef GLB_ENABLE_SHADOW
	vec2 shadow_coord;
	int index = calc_current_shadow_index(vs_Vertex, glb_EyePos, glb_LookAt, glb_ShadowSplit0, glb_ShadowSplit1, glb_ShadowSplit2);
	vec4 light_space_pos;
	float factor;
	if (index == 0) {
		light_space_pos = glb_ShadowM0 * vec4(vs_Vertex, 1.0);
		light_space_pos.xyz /= 2.0f;
		light_space_pos.xyz += 0.5f;
		light_space_pos.xyz /= light_space_pos.w;
		factor = texture2D(glb_ShadowTex0, light_space_pos.xy).z;
	} else if (index == 1) {
		light_space_pos = glb_ShadowM1 * vec4(vs_Vertex, 1.0);
		light_space_pos.xyz /= 2.0f;
		light_space_pos.xyz += 0.5f;
		light_space_pos.xyz /= light_space_pos.w;	
		factor = texture2D(glb_ShadowTex1, light_space_pos.xy).z;
	} else if (index == 2) {
		light_space_pos = glb_ShadowM2 * vec4(vs_Vertex, 1.0);
		light_space_pos.xyz /= 2.0f;
		light_space_pos.xyz += 0.5f;
		light_space_pos.xyz /= light_space_pos.w;				
		factor = texture2D(glb_ShadowTex2, light_space_pos.xy).z;
	} else {
		light_space_pos = glb_ShadowM3 * vec4(vs_Vertex, 1.0);
		light_space_pos.xyz /= 2.0f;
		light_space_pos.xyz += 0.5f;
		light_space_pos.xyz /= light_space_pos.w;
		factor = texture2D(glb_ShadowTex3, light_space_pos.xy).z;
	}
	if (factor < light_space_pos.z) {
		is_in_shadow = 1.0;
	}
	if (light_space_pos.x < 0.0 || 
		light_space_pos.x > 1.0 ||
		light_space_pos.y < 0.0 ||
		light_space_pos.y > 1.0) {
		is_in_shadow = 0.0;
	}
	shadow_coord = light_space_pos.xy;
#endif

	vec3 normal = vec3(0.0, 0.0, 0.0);
#ifdef GLB_NORMAL_IN_VERTEX
	normal = normalize(vs_Normal);
#endif

#ifdef GLB_ENABLE_NORMAL_MAPPING
	normal = texture2D(glb_NormalTex, vs_TexCoord).xyz;
	normal -= vec3(0.5, 0.5, 0.5);
	normal *= 2.0;

	mat3 tbn = mat3(vs_Tangent, vs_Binormal, vs_NT);
	normal = tbn * normal;
	normalize(normal);
#endif

#ifdef GLB_ENABLE_EYE_POS
	vec3 view_vec = normalize(glb_EyePos - vs_Vertex);
#endif	

#ifdef GLB_ENABLE_LIGHTING
	if (is_in_shadow <= 0.0) {
		#ifdef GLB_USE_PARALLEL_LIGHT
			vec3 light_vec = -glb_ParallelLight_Dir;
			ambient_light = glb_Material_Ambient * glb_ParallelLight_Ambient;
			diffuse_light = calc_diffuse(light_vec, normal, glb_Material_Diffuse, glb_ParallelLight_Diffuse);
			specular_light = calc_specular(light_vec, normal, view_vec, glb_Material_Specular, glb_ParallelLight_Specular, glb_Material_Pow);
			emii_light = glb_Material_Emission;
		#endif
	} else {
		ambient_light = glb_Material_Ambient * glb_ParallelLight_Ambient;
		emii_light = glb_Material_Emission;
	}
#else
	if (is_in_shadow >= 1.0) {
		oColor.xyz = 0.5 * oColor.xyz;
	}
#endif

#ifdef GLB_ENABLE_AO
	vec2 ao_coord = vec2(gl_FragCoord.x / glb_ScreenWidth, gl_FragCoord.y / glb_ScreenHeight);
	ambient_light *= texture2D(glb_AOMap, ao_coord).rgb;
#endif

	vec3 light = ambient_light + specular_light + diffuse_light;

#ifdef GLB_ENABLE_REFLECT_TEX
	vec3 refl = reflect(-view_vec, normal);
	refl = normalize(refl);
	refl.yz = -refl.yz;
	reflect_light = textureCube(glb_ReflectTex, refl).xyz;
#endif

	vec3 brdf = vec3(0.0, 0.0, 0.0);
	vec3 vtxc = vec3(0.0, 0.0, 0.0);
#ifdef GLB_COLOR_IN_VERTEX
	vtxc = vs_Color;
#endif

#ifdef GLB_TEXCOORD_IN_VERTEX
	#ifdef GLB_ENABLE_DIFFUSE_TEX
		brdf.xyz = texture2D(glb_DiffuseTex, vs_TexCoord).xyz;
	#endif
#endif

	oColor.xyz = combine_color(light, reflect_light, brdf, vtxc);
	oColor.xyz += emii_light;
	
#ifdef GLB_TEXCOORD_IN_VERTEX
	#ifdef GLB_ENABLE_ALPHA_TEX
		oColor.w = texture2D(glb_AlphaTex, vs_TexCoord).w;
	#endif
#endif
}