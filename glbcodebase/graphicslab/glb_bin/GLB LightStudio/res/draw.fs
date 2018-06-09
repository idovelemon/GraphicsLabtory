#version 450

in vec2 vsTexCoord;
in vec3 vsNormal;

uniform sampler2D glb_LightMap[3];
uniform sampler2D glb_AlbedoMap;
uniform sampler2D glb_NormalMap;

out vec3 oColor;

vec3 calc_radiosity_normal_map_color(vec2 uv) {
    // Calculate normal
    vec3 normal = texture(glb_NormalMap, uv).xyz;
    normal -= vec3(0.5, 0.5, 0.5);  
    normal *= 2.0;
    normalize(normal);

    // Calculate bais
    vec3 bais0 = vec3(sqrt(2.0 / 3.0), 0.0, sqrt(1.0 / 3.0));
    vec3 bais1 = vec3(-sqrt(1.0 / 6.0), sqrt(1.0 / 2.0), sqrt(1.0 / 3.0));
    vec3 bais2 = vec3(-sqrt(1.0 / 6.0), -sqrt(1.0 / 2.0), sqrt(1.0 / 3.0));
    normalize(bais0);
    normalize(bais1);
    normalize(bais2);

    vec3 lightMapColor0 = texture(glb_LightMap[0], uv).rgb;
    vec3 lightMapColor1 = texture(glb_LightMap[1], uv).rgb;
    vec3 lightMapColor2 = texture(glb_LightMap[2], uv).rgb;

    return lightMapColor0 * max(0.0, dot(bais0, normal)) + lightMapColor1 * max(0.0, dot(bais1, normal)) + lightMapColor2 * max(0.0, dot(bais2, normal));
}

void main() {
    oColor = (calc_radiosity_normal_map_color(vsTexCoord) + vec3(0.05, 0.05, 0.05)) * texture(glb_AlbedoMap, vsTexCoord).xyz;
    oColor = oColor / (vec3(1.0, 1.0, 1.0) + oColor);
    oColor = pow(oColor, vec3(1.0 / 2.2, 1.0 / 2.2, 1.0 / 2.2));
}