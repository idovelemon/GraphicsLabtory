#version 450

in vec3 glb_Pos;
in vec2 glb_TexCoord;
in vec2 glb_LightMapTexCoord;

out vec2 vsTexCoord;
out vec2 vsLightMapTexCoord;
out vec3 vsVertex;
out vec3 vsPosW;

uniform mat4 glb_MVP;
uniform mat4 glb_World;

void main() {
    gl_Position = glb_MVP * vec4(glb_Pos, 1.0);
    vsVertex = gl_Position.xyz / gl_Position.w;
    vsTexCoord = glb_TexCoord;
    vsLightMapTexCoord = glb_LightMapTexCoord;
    vsPosW = (glb_World * vec4(glb_Pos, 1.0)).xyz;
}