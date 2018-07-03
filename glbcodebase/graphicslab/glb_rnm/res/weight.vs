#version 450

in vec3 glb_attr_Pos;
in vec2 glb_attr_TexCoord;

out vec2 vsTexCoord;

void main() {
    gl_Position = vec4(glb_attr_Pos, 1.0);
    vsTexCoord = glb_attr_TexCoord;
}