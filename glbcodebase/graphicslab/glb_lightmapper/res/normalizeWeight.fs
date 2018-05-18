#version 450

in vec2 vsTexCoord;

out vec3 oColor;

uniform sampler2D glb_WeightMap;
uniform vec3 glb_TotalColor;

void main() {
    vec3 color = texture(glb_WeightMap, vsTexCoord).rgb;
    oColor = color / glb_TotalColor;
}