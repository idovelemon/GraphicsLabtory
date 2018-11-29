#version 330

in vec3 glb_attr_Pos;
in vec4 glb_attr_Color;

out vec4 vs_Color;
out vec2 vs_TexCoord;

void main() {
    gl_Position = vec4(glb_attr_Pos, 1.0);
    vs_Color = glb_attr_Color;
}