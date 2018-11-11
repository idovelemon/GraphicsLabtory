#version 330

in vec3 glb_attr_Pos;
in vec3 glb_attr_Color;
in vec2 glb_attr_TexCoord;

out vec3 vs_Color;
out vec2 vs_TexCoord;

void main() {
    gl_Position = vec4(glb_attr_Pos, 1.0);
    vs_Color = glb_attr_Color;
    vs_TexCoord = glb_attr_TexCoord;
}