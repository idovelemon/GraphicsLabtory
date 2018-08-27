#version 330

in vec2 vs_TexCoord;
in vec4 vs_Vertex;

out vec3 oColor;

uniform sampler2D glb_AlbedoTex;
uniform sampler2D glb_DecalTex;
uniform mat4 glb_DecalWVP;

void main() {
    vec3 albedo = texture(glb_AlbedoTex, vs_TexCoord).xyz;

    vec4 decalTexcoord = glb_DecalWVP * vs_Vertex;
    decalTexcoord.xyz /= 2.0;
    decalTexcoord.xyz += 0.5;
    decalTexcoord.xyz /= decalTexcoord.w;

    vec4 decal = texture(glb_DecalTex, decalTexcoord.xy);

    oColor = albedo * (1.0 - decal.w) + decal.xyz * decal.w;
}