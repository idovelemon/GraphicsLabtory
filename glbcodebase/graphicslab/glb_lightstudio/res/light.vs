#version 450

in vec3 glb_Pos;

uniform mat4 glb_MVP;

void main() {
    gl_Position = glb_MVP * vec4(glb_Pos, 1.0);
}