#version 450

in vec2 vsTexCoord;
in vec3 vsVertex;
in vec3 vsPosW;

out vec3 oColor;

uniform sampler2D glb_NormalizeWeightMap;
uniform int glb_Face;
uniform vec3 glb_EyePos;
uniform vec3 glb_SunDir;
uniform vec3 glb_LightColor;

vec3 calc_sun_color() {
    vec3 d = vsPosW - glb_EyePos;
    d = normalize(d);

    vec3 color = vec3(0.0, 0.0, 0.0);
    if (dot(d, glb_SunDir) > 0.999) {
        color = glb_LightColor;
    }

    return color;
}

void main() {
    if (glb_Face == 0 && vsVertex.x <= 0.0) {  // Right face
        oColor = calc_sun_color();
    } else if (glb_Face == 1 && vsVertex.x >= 0.0) {  // Left face
        oColor = calc_sun_color();
    } else if (glb_Face == 2 && vsVertex.y <= 0.0) { // Up face
        oColor = calc_sun_color();
    } else if (glb_Face == 3 && vsVertex.y >= 0.0) { // Down face
        oColor = calc_sun_color();
    } else if (glb_Face == 4) {  // Front face
        oColor = calc_sun_color();
    } else {
        oColor = vec3(0.0, 0.0, 0.0);
    }

    vec2 uv = (vec2(vsVertex.x, vsVertex.y) + vec2(1.0, 1.0)) / 2.0;
    oColor = oColor * texture(glb_NormalizeWeightMap, uv).rgb;
}