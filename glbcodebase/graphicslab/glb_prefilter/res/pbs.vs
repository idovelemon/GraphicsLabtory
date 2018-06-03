#version 330

in vec3 glb_Pos;
in vec2 glb_TexCoord;
in vec3 glb_Normal;
in vec3 glb_Tangent;
in vec3 glb_Binormal;

uniform mat4 glb_WVP;
uniform mat4 glb_InvTransWorldM;
uniform mat4 glb_World;

out vec3 vs_Vertex;
out vec2 vs_Texcoord;
out vec3 vs_Normal;
out vec3 vs_Tangent;
out vec3 vs_Binormal;

void main() {
    gl_Position = (glb_WVP * vec4(glb_Pos, 1.0));
    vs_Normal = cross(glb_Tangent, glb_Binormal);
	vs_Tangent = (glb_InvTransWorldM * vec4(glb_Tangent, 0.0)).xyz;
	vs_Binormal = (glb_InvTransWorldM * vec4(glb_Binormal, 0.0)).xyz;
	vs_Normal = (glb_InvTransWorldM * vec4(vs_Normal, 0.0)).xyz;
    vs_Vertex = (glb_World * vec4(glb_Pos, 1.0)).xyz;
    vs_Texcoord = glb_TexCoord;
}