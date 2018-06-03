//----------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016 / 10 / 15
// Brief: This is a uber shader, all the light calculation, texture mapping
// and some sort of that will be implemented in this shader.
//----------------------------------------------------
// Input attributes
in vec3 glb_Pos;

#ifdef GLB_COLOR_IN_VERTEX
in vec3 glb_Color;
#endif

#ifdef GLB_NORMAL_IN_VERTEX
in vec3 glb_Normal;
#endif

#ifdef GLB_TANGENT_IN_VERTEX
in vec3 glb_Tangent;
#endif

#ifdef GLB_BINORMAL_IN_VERTEX
in vec3 glb_Binormal;
#endif

#ifdef GLB_TEXCOORD_IN_VERTEX
in vec2 glb_TexCoord;
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

uniform mat4 glb_ProjM;
uniform mat4 glb_ViewM;
uniform mat4 glb_WorldM;
uniform mat4 glb_Trans_Inv_WorldM;

void main() {
	gl_Position = glb_ProjM * glb_ViewM * glb_WorldM * vec4(glb_Pos, 1.0);
	vs_Vertex = (glb_WorldM * vec4(glb_Pos, 1.0)).xyz;

#ifdef GLB_COLOR_IN_VERTEX
	vs_Color = glb_Color;
#endif

#ifdef GLB_NORMAL_IN_VERTEX
	vs_Normal = (glb_Trans_Inv_WorldM * vec4(glb_Normal, 0.0)).xyz;
#endif

#ifdef GLB_TANGENT_IN_VERTEX
	vs_Tangent = (glb_Trans_Inv_WorldM * vec4(glb_Tangent, 0.0)).xyz;
#endif

#ifdef GLB_BINORMAL_IN_VERTEX
	vs_Binormal = (glb_Trans_Inv_WorldM * vec4(glb_Binormal, 0.0)).xyz;
#endif

#ifdef GLB_TEXCOORD_IN_VERTEX
	vs_TexCoord = glb_TexCoord;
#endif
}