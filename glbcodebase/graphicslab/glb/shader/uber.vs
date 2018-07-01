//----------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016 / 10 / 15
// Brief: This is a uber shader, all the light calculation, texture mapping
// and some sort of that will be implemented in this shader.
//----------------------------------------------------
// Input attributes
layout (location = 0) in vec3 glb_Pos;

#ifdef GLB_COLOR_IN_VERTEX
	layout (location = 1) in vec3 glb_Color;
#endif

#ifdef GLB_NORMAL_IN_VERTEX
	layout (location = 2) in vec3 glb_Normal;
#endif

#ifdef GLB_TANGENT_IN_VERTEX
	layout (location = 3) in vec3 glb_Tangent;
#endif

#ifdef GLB_BINORMAL_IN_VERTEX
	layout (location = 4) in vec3 glb_Binormal;
#endif

#ifdef GLB_TEXCOORD_IN_VERTEX
	layout (location = 5) in vec2 glb_TexCoord;
#endif

#ifdef GLB_LIGHT_TEXCOORD_IN_VERTEX
	layout (location = 6) in vec2 glb_LightMapTexCoord;
#endif

#ifdef GLB_ENABLE_INSTANCE_RENDERING
	layout (location = 7) in mat4 glb_attr_WorldMatrix;
	layout (location = 11) in mat4 glb_attr_TransInvWorldMatrix;
#endif

// Output attributes
out vec3 vs_Vertex;

#ifdef GLB_COLOR_IN_VERTEX
	out vec3 vs_Color;
#endif

#ifdef GLB_NORMAL_IN_VERTEX
	out vec3 vs_Normal;
#endif

#ifdef GLB_TANGENT_IN_VERTEX
	out vec3 vs_Tangent;
#endif

#ifdef GLB_BINORMAL_IN_VERTEX
	out vec3 vs_Binormal;
#endif

#ifdef GLB_TEXCOORD_IN_VERTEX
	out vec2 vs_TexCoord;
#endif

#ifdef GLB_LIGHT_TEXCOORD_IN_VERTEX
	out vec2 vs_LightMapTexCoord;
#endif

uniform mat4 glb_ProjM;
uniform mat4 glb_ViewM;

#ifndef GLB_ENABLE_INSTANCE_RENDERING
	uniform mat4 glb_WorldM;
	uniform mat4 glb_Trans_Inv_WorldM;
#endif

void main() {
#ifdef GLB_ENABLE_INSTANCE_RENDERING	
	gl_Position = glb_ProjM * glb_ViewM * glb_attr_WorldMatrix * vec4(glb_Pos, 1.0);
	vs_Vertex = (glb_attr_WorldMatrix * vec4(glb_Pos, 1.0)).xyz;
#else
	gl_Position = glb_ProjM * glb_ViewM * glb_WorldM * vec4(glb_Pos, 1.0);
	vs_Vertex = (glb_WorldM * vec4(glb_Pos, 1.0)).xyz;
#endif

#ifdef GLB_COLOR_IN_VERTEX
	vs_Color = glb_Color;
#endif

#ifdef GLB_NORMAL_IN_VERTEX
	#ifdef GLB_ENABLE_INSTANCE_RENDERING
		vs_Normal = (glb_attr_TransInvWorldMatrix * vec4(glb_Normal, 0.0)).xyz;
	#else
		vs_Normal = (glb_Trans_Inv_WorldM * vec4(glb_Normal, 0.0)).xyz;
	#endif
#endif

#ifdef GLB_TANGENT_IN_VERTEX
	#ifdef GLB_ENABLE_INSTANCE_RENDERING
		vs_Tangent = (glb_attr_TransInvWorldMatrix * vec4(glb_Tangent, 0.0)).xyz;
	#else
		vs_Tangent = (glb_Trans_Inv_WorldM * vec4(glb_Tangent, 0.0)).xyz;
	#endif
#endif

#ifdef GLB_BINORMAL_IN_VERTEX
	#ifdef GLB_ENABLE_INSTANCE_RENDERING
		vs_Binormal = (glb_attr_TransInvWorldMatrix * vec4(glb_Binormal, 0.0)).xyz;
	#else
		vs_Binormal = (glb_Trans_Inv_WorldM * vec4(glb_Binormal, 0.0)).xyz;
	#endif
#endif

#ifdef GLB_TEXCOORD_IN_VERTEX
	vs_TexCoord = glb_TexCoord;
#endif

#ifdef GLB_LIGHT_TEXCOORD_IN_VERTEX
	vs_LightMapTexCoord = glb_LightMapTexCoord;
#endif
}