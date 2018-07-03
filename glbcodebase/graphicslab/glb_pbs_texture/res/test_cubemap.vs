#version 330

in vec3 glb_attr_Pos;
in vec3 glb_attr_Normal;

uniform mat4 glb_WVP;

out vec3 vs_Normal;

void main() {
    gl_Position = (glb_WVP * vec4(glb_attr_Pos, 1.0));
    vs_Normal = glb_attr_Normal;
}