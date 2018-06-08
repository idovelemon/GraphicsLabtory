#version 450

out vec3 oColor;

uniform vec3 glb_LightColor;
uniform int glb_EnableTonemapping;

void main() {
    oColor = glb_LightColor;
    if (glb_EnableTonemapping != 0) {
        oColor = oColor / (vec3(1.0, 1.0, 1.0) + oColor);
        oColor = pow(oColor, vec3(1.0 / 2.2, 1.0 / 2.2, 1.0 / 2.2));
    }
}