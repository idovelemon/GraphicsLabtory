#version 450

in vec2 vsTexCoord;

out vec3 oColor;

uniform int glb_Face;
uniform int glb_LightPatchWidth;
uniform int glb_LightPatchHeight;

float calc_perspective_correction_factor(vec2 uv) {
    vec3 l = vec3((uv - vec2(0.5, 0.5)) * 2.0, 1.0);
    l = normalize(l);
    float vdotl = dot(vec3(0.0, 0.0, 1.0), l);
    vdotl = max(0.0, vdotl);
    return vdotl;
}

float calc_lambert_law_factor(vec2 uv, int face) {
    vec3 n = vec3(0.0, 0.0, -1.0);
    vec3 l = vec3(0.0, 0.0, 0.0);
    vec2 p = (uv - vec2(0.5, 0.5)) * 2.0;
    if (face == 0) {
        l = vec3(1.0, -p.y, p.x);
    } else if (face == 1) {
        l = vec3(-1.0, -p.y, -p.x);
    } else if (face == 2) {
        l = vec3(p.x, 1.0, p.y);
    } else if (face == 3) {
        l = vec3(p.x, -1.0, -p.y);
    } else if (face == 4) {
        l = vec3(p.x, -p.y, -1.0);
    }

    l = normalize(l);
    return max(0.0, dot(n, l));
}

float calc_area_factor(vec2 uv, int face, int w, int h) {
    float deltaArea = 4.0 / (1.0 * w * h);

    vec3 l = vec3(0.0, 0.0, 0.0);
    vec2 p = (uv - vec2(0.5, 0.5)) * 2.0;
    if (face == 0) {
        l = vec3(1.0, -p.y, p.x);
    } else if (face == 1) {
        l = vec3(-1.0, -p.y, -p.x);
    } else if (face == 2) {
        l = vec3(p.x, 1.0, p.y);
    } else if (face == 3) {
        l = vec3(p.x, -1.0, -p.y);
    } else if (face == 4) {
        l = vec3(p.x, -p.y, -1.0);
    }

    float r = length(l);

    return deltaArea / (3.1415926 * r * r);
}

void main() {
    float vdotl = calc_perspective_correction_factor(vsTexCoord);
    float ndotl = calc_lambert_law_factor(vsTexCoord, glb_Face);
    float area = calc_area_factor(vsTexCoord, glb_Face, glb_LightPatchWidth, glb_LightPatchHeight);
    oColor = vec3(vdotl * ndotl * area, vdotl * ndotl * area, vdotl * ndotl * area);
}