#version 450

in vec2 vsTexCoord;
in vec3 vsVertex;

out vec3 oColor;

uniform sampler2D glb_NormalizeWeightMap;
uniform sampler2D glb_LightMap[3];
uniform sampler2D glb_AlbedoMap;
uniform sampler2D glb_NormalMap;
uniform int glb_Face;

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
    if (glb_Face == 0 && vsVertex.x <= 0.0) {  // Right face
        oColor = calc_radiosity_normal_map_color(vsTexCoord);
    } else if (glb_Face == 1 && vsVertex.x >= 0.0) {  // Left face
        oColor = calc_radiosity_normal_map_color(vsTexCoord);
    } else if (glb_Face == 2 && vsVertex.y <= 0.0) { // Up face
        oColor = calc_radiosity_normal_map_color(vsTexCoord);
    } else if (glb_Face == 3 && vsVertex.y >= 0.0) { // Down face
        oColor = calc_radiosity_normal_map_color(vsTexCoord);
    } else if (glb_Face == 4) {  // Front face
        oColor = calc_radiosity_normal_map_color(vsTexCoord);
    } else {
        oColor = vec3(0.0, 0.0, 0.0);
    }

    vec2 uv = (vec2(vsVertex.x, vsVertex.y) + vec2(1.0, 1.0)) / 2.0;
    oColor = oColor * texture(glb_AlbedoMap, vsTexCoord).rgb * texture(glb_NormalizeWeightMap, uv).rgb;
}