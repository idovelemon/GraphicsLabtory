#version 450

in vec3 glb_Pos;

uniform mat4 glb_World;
uniform mat4 glb_MVP;

out vec3 vs_Vertex;

void main() {
    gl_Position = glb_MVP * vec4(glb_Pos, 1.0);
    vs_Vertex = (glb_World * vec4(glb_Pos, 1.0)).xyz;
}