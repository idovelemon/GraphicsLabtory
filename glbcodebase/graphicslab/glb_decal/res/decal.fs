#version 330

in vec2 vs_TexCoord;

out vec4 oColor;

uniform sampler2D glb_DecalTex;

void main() {
    oColor = texture(glb_DecalTex, vs_TexCoord);
}