#version 450

in vec2 vsTexCoord;

out vec3 oColor;

uniform sampler2D glb_LightMap;

void main() {
    //oColor = texture(glb_LightMap, vsTexCoord, -2.0).rgb;
    oColor = texture(glb_LightMap, vsTexCoord).rgb;
    //oColor = oColor / (vec3(1.0, 1.0, 1.0) + oColor);
    oColor = pow(oColor, vec3(1.0 / 2.2, 1.0 / 2.2, 1.0 / 2.2));
    //oColor = vec3(0.2, 0.2, 0.2) + texture(glb_LightMap, vsTexCoord).rgb;
    //oColor = vec3(1.0, 0.0, 0.0);
}