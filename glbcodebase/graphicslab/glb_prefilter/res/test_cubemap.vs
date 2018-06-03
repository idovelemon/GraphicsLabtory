#version 330

in vec3 glb_Pos;
in vec3 glb_Normal;

uniform mat4 glb_WVP;

out vec3 vs_Normal;

void main() {
    gl_Position = (glb_WVP * vec4(glb_Pos, 1.0));
    vs_Normal = glb_Normal;
}