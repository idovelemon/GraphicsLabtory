#version 330

#include "../../glb/shader/glb.gsh"

// Input attributes
in vec2 vs_TexCoord;

// Output color
out vec4 oColor;

// Uniform
uniform sampler2D glb_unif_AlbedoTex;

void main() {
    oColor = texture(glb_unif_AlbedoTex, vs_TexCoord);
    oColor.w = 1.0;
}