#version 330

// Input attributes
layout (location = 0) in vec3 glb_attr_Pos;
layout (location = 2) in vec3 glb_attr_Normal;
layout (location = 3) in vec3 glb_attr_Tangent;
layout (location = 4) in vec3 glb_attr_Binormal;
layout (location = 5) in vec2 glb_attr_TexCoord;

// Output attributes
out vec4 vs_Vertex;
out vec3 vs_Normal;
out vec3 vs_Tangent;
out vec3 vs_Binormal;
out vec2 vs_TexCoord;

uniform mat4 glb_unif_ProjM;
uniform mat4 glb_unif_ViewM;

uniform mat4 glb_unif_WorldM;
uniform mat4 glb_unif_Trans_Inv_WorldM;

void main() {
	gl_Position = glb_unif_ProjM * glb_unif_ViewM * glb_unif_WorldM * vec4(glb_attr_Pos, 1.0);
	vs_Vertex = (glb_unif_WorldM * vec4(glb_attr_Pos, 1.0));

    vs_Normal = (glb_unif_Trans_Inv_WorldM * vec4(glb_attr_Normal, 0.0)).xyz;
    vs_Tangent = (glb_unif_Trans_Inv_WorldM * vec4(glb_attr_Tangent, 0.0)).xyz;
    vs_Binormal = (glb_unif_Trans_Inv_WorldM * vec4(glb_attr_Binormal, 0.0)).xyz;

	vs_TexCoord = glb_attr_TexCoord;
}