#version 450

in vec3 vs_Vertex;

out vec3 oColor;

uniform vec3 glb_LightColor;
uniform vec3 glb_SunDir;
uniform vec3 glb_EyePos;

void main() {
    vec3 d = vs_Vertex - glb_EyePos;
    d = normalize(d);

    if (dot(d, glb_SunDir) > 0.99999) {
        oColor = glb_LightColor;
    } else {
        oColor = vec3(0.0, 0.0, 0.0);
    }

    oColor = oColor / (vec3(1.0, 1.0, 1.0) + oColor);
    oColor = pow(oColor, vec3(1.0 / 2.2, 1.0 / 2.2, 1.0 / 2.2));
}