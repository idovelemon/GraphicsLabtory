#version 330
#extension GL_NV_shadow_samplers_cube : enable

in vec3 vs_Normal;

out vec3 oColor;

uniform samplerCube glb_CubeMap;

const float PI = 3.1415926536898;

void main() {
    vec3 dir = normalize(vs_Normal);
    dir.yz = -dir.yz;
    vec3 color = textureCube(glb_CubeMap, dir).xyz;

    // base tone mapping
    color = color / (color + vec3(1.0, 1.0, 1.0));

    // gamma correction
    color = pow(color, vec3(1.0 / 2.2, 1.0 / 2.2, 1.0 / 2.2));

    oColor = color;
}