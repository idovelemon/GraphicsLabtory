#version 450

in vec3 glb_Pos;
in vec2 glb_TexCoord;

out vec2 vsTexCoord;

uniform mat4 glb_MVP;

void main() {
    gl_Position = glb_MVP * vec4(glb_Pos, 1.0);
    vsTexCoord = glb_TexCoord;
}