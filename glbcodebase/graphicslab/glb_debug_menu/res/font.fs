#version 330

in vec3 vs_Color;
in vec2 vs_TexCoord;

out vec4 oColor;

uniform sampler2D glb_FontMap;

void main() {
    vec4 fontTexColor = texture(glb_FontMap, vs_TexCoord);
    oColor.xyz = fontTexColor.xyz * vs_Color;
    oColor.w = fontTexColor.w;
}