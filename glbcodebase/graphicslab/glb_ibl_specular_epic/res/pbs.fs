#version 330
#extension GL_NV_shadow_samplers_cube : enable

in vec3 vs_Vertex;
in vec3 vs_Normal;

uniform vec3 glb_Albedo;
uniform float glb_Roughness;
uniform float glb_Metalic;
uniform vec3 glb_EyePos;
uniform samplerCube glb_IrradianceMap;
uniform samplerCube glb_PerfilterEnvMap;
uniform sampler2D glb_IntegrateBRDFMap;

out vec3 oColor;

const float PI = 3.1415927;

vec3 filtering_cube_map(samplerCube cube, vec3 n) {
    n.yz = -n.yz;
    return texture(cube, n).xyz;
}

vec3 filtering_cube_map_lod(samplerCube cube, vec3 n, float lod) {
    n.yz = -n.yz;
    return textureLod(cube, n, lod).xyz;
}

vec3 calc_fresnel(vec3 n, vec3 v, vec3 F0) {
    float ndotv = max(dot(n, v), 0.0);
    return F0 + (vec3(1.0, 1.0, 1.0) - F0) * pow(1.0 - ndotv, 5.0);
}

vec3 calc_fresnel_roughness(vec3 n, vec3 v, vec3 F0, float roughness) {
    float ndotv = max(dot(n, v), 0.0);
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - ndotv, 5.0);
}

float calc_NDF_GGX(vec3 n, vec3 h, float roughness) {
    float a = roughness * roughness;
    float a2 = a * a;
    float ndoth = max(dot(n, h), 0.0);
    float ndoth2 = ndoth * ndoth;
    float t = ndoth2 * (a2 - 1.0) + 1.0;
    float t2 = t * t;
    return a2 / (PI * t2);
}

float calc_Geometry_GGX(float costheta, float roughness) {
    float a = roughness;
    float r = a + 1.0;
    float r2 = r * r;
    float k = r2 / 8.0;

    float t = costheta * (1.0 - k) + k;

    return costheta / t;
}

float calc_Geometry_Smith(vec3 n, vec3 v, vec3 l, float roughness) {
    float ndotv = max(dot(n, v), 0.0);
    float ndotl = max(dot(n, l), 0.0);
    float ggx1 = calc_Geometry_GGX(ndotv, roughness);
    float ggx2 = calc_Geometry_GGX(ndotl, roughness);
    return ggx1 * ggx2;
}

vec3 calc_lighting_direct(vec3 n, vec3 v, vec3 l, vec3 h, vec3 albedo, float roughness, float metalic, vec3 light) {
    vec3 F0 = mix(vec3(0.04, 0.04, 0.04), albedo, metalic);
    vec3 F = calc_fresnel(h, v, F0);

    vec3 T = vec3(1.0, 1.0, 1.0) - F;
    vec3 kD = T * (1.0 - metalic);

    float D = calc_NDF_GGX(n, h, roughness);

    float G = calc_Geometry_Smith(n, v, l, roughness);

    vec3 Diffuse = kD * albedo * vec3(1.0 / PI, 1.0 / PI, 1.0 / PI);
    float t = 4.0 * max(dot(n, v), 0.0) * max(dot(n, l), 0.0) + 0.001;
    vec3 Specular = D * F * G * vec3(1.0 / t, 1.0 / t, 1.0 / t);

    float ndotl = max(dot(n, l), 0.0);
    return (Diffuse + Specular) * light * vec3(ndotl, ndotl, ndotl);
}

vec3 calc_ibl(vec3 n, vec3 v, vec3 albedo, float roughness, float metalic) {
    vec3 F0 = mix(vec3(0.04, 0.04, 0.04), albedo, metalic);
    vec3 F = calc_fresnel_roughness(n, v, F0, roughness);

    // Diffuse part
    vec3 T = vec3(1.0, 1.0, 1.0) - F;
    vec3 kD = T * (1.0 - metalic);

    vec3 irradiance = filtering_cube_map(glb_IrradianceMap, n);
    vec3 diffuse = kD * albedo * irradiance;

    // Specular part
    float ndotv = max(0.0, dot(n, v));
    vec3 r = 2.0 * ndotv * n - v;
    vec3 ld = filtering_cube_map_lod(glb_PerfilterEnvMap, r, roughness * 9.0);
    vec2 dfg = textureLod(glb_IntegrateBRDFMap, vec2(ndotv, roughness), 0.0).xy;
    vec3 specular = ld * (F0 * dfg.x + dfg.y);

    return diffuse + specular;
}

void main() {
    vec3 view = glb_EyePos - vs_Vertex;
    view = normalize(view);

    vec3 lightPos = vec3(0.0, 0.0, 200.0);
    vec3 light = lightPos - vs_Vertex;
    light = normalize(light);

    vec3 half = normalize(view + light);

    vec3 colorDirect = calc_lighting_direct(normalize(vs_Normal), view, light, half, glb_Albedo, glb_Roughness, glb_Metalic, vec3(2.5, 2.5, 2.5));
    vec3 colorIBL = calc_ibl(normalize(vs_Normal), view, glb_Albedo, glb_Roughness, glb_Metalic);
    //vec3 color = colorDirect + colorIBL;
    vec3 color = colorIBL;

    // base tone mapping
    color = color / (color + vec3(1.0, 1.0, 1.0));

    // gamma correction
    color = pow(color, vec3(1.0 / 2.2, 1.0 / 2.2, 1.0 / 2.2));

    oColor = color;
}