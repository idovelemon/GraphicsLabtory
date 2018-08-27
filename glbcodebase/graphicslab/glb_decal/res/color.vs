#version 330

in vec3 glb_attr_Pos;
in vec2 glb_attr_TexCoord;

uniform mat4 glb_WVP;

out vec2 vs_TexCoord;
out vec4 vs_Vertex;

void main() {
    gl_Position = (glb_WVP * vec4(glb_attr_Pos, 1.0));
    vs_TexCoord = glb_attr_TexCoord;
    vs_Vertex = vec4(glb_attr_Pos, 1.0);
}