#version 450

in vec3 glb_Pos;
in vec2 glb_TexCoord;
in vec3 glb_Tangent;
in vec3 glb_Binormal;

out vec2 vsTexCoord;
out vec3 vsVertex;
out vec3 vsNormal;
out vec3 vsTangent;
out vec3 vsBinormal;

uniform mat4 glb_MVP;
uniform mat4 glb_TransInvWorld;

void main() {
    gl_Position = glb_MVP * vec4(glb_Pos, 1.0);
    vsVertex = gl_Position.xyz / gl_Position.w;
    vsTexCoord = glb_TexCoord;

    vsNormal = cross(glb_Tangent, glb_Binormal);
    vsNormal = (glb_TransInvWorld * vec4(vsNormal, 0.0)).xyz;

    vsTangent = (glb_TransInvWorld * vec4(glb_Tangent, 0.0)).xyz;
    vsBinormal = (glb_TransInvWorld * vec4(glb_Binormal, 0.0)).xyz;
}