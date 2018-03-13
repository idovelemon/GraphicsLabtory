#version 330
#extension GL_NV_shadow_samplers_cube : enable

in vec2 vs_TexCoord;

out vec3 oColor;

uniform samplerCube glb_CubeMap;
uniform int glb_FaceIndex;
uniform float glb_Roughness;

const float PI = 3.1415926536898;

vec3 filtering_cube_map(samplerCube cube, vec3 n) {
    n.yz = -n.yz;
    return texture(cube, n).xyz;
}

vec3 calc_normal(int face, vec2 uv) {
    // 6 Face(+X,-X,+Y,-Y,+Z,-Z) for [0,5]
    uv = (uv - 0.5) * 2.0;  // Convert range [0, 1] to [-1, 1]

    vec3 n = vec3(0.0, 0.0, 0.0);
    if (face == 0) {
        // +X face
        n.x = 1.0;
        n.zy = uv;
    } else if (face == 1) {
        // -X face
        n.x = -1.0;
        n.z = -uv.x;
        n.y = uv.y;
    } else if (face == 2) {
        // +Y face
        n.y = 1.0;
        n.xz = uv;
    } else if (face == 3) {
        // -Y face
        n.y = -1.0;
        n.x = uv.x;
        n.z = -uv.y;
    } else if (face == 4) {
        // +Z face
        n.z = 1.0;
        n.x = -uv.x;
        n.y = uv.y;
    } else if (face == 5) {
        // -Z face
        n.z = -1.0;
        n.xy = uv;
    }

    return n;
}

float radical_inverse( uint bits ) {
    bits = ( bits << 16u ) | ( bits >> 16u );
    bits = ( ( bits & 0x55555555u ) << 1u ) | ( ( bits & 0xAAAAAAAAu ) >> 1u );
    bits = ( ( bits & 0x33333333u ) << 2u ) | ( ( bits & 0xCCCCCCCCu ) >> 2u );
    bits = ( ( bits & 0x0F0F0F0Fu ) << 4u ) | ( ( bits & 0xF0F0F0F0u ) >> 4u );
    bits = ( ( bits & 0x00FF00FFu ) << 8u ) | ( ( bits & 0xFF00FF00u ) >> 8u );
    return float( bits ) * 2.3283064365386963e-10f;
}

vec2 hammersley( uint i, uint spp ) {
    return vec2( float( i ) / float( spp ), radical_inverse( i ) );
}

vec3 calc_cartesian(float phi, float theta) {
    return vec3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
}

vec3 importance_sampling_ggx(vec2 xi, float roughness, vec3 n) {
    float a = roughness * roughness;

    float phi = 2.0 * PI * xi.x;
    float costheta = sqrt((1.0 - xi.y) / (1.0 + (a * a - 1.0) * xi.y));
    float sintheta = sqrt(1.0 - costheta * costheta);

    vec3 h = vec3(0.0, 0.0, 0.0);
    h.x = sintheta * cos(phi);
    h.y = sintheta * sin(phi);
    h.z = costheta;

    vec3 up = abs(n.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
    vec3 tx = normalize(cross(up, n));
    vec3 ty = cross(n, tx);

    return tx * h.x + ty * h.y + n * h.z;
}

vec3 convolution_cube_map(samplerCube cube, int faceIndex, vec2 uv) {
    // Calculate tangent space base vector
    vec3 n = calc_normal(faceIndex, uv);
    n = normalize(n);
    vec3 v = n;
    vec3 r = n;

    // Convolution
    uint sampler = 1024u;
    vec3 color = vec3(0.0, 0.0, 0.0);
    float weight = 0.0;
    for (uint i = 0u; i < sampler; i++) {
        vec2 xi = hammersley(i, sampler);
        vec3 h = importance_sampling_ggx(xi, glb_Roughness, n);
        vec3 l = 2.0 * dot(v, h) * h - v;

        float ndotl = max(0, dot(n, l));
        if (ndotl > 0.0) {
            color = color + filtering_cube_map(glb_CubeMap, l).xyz * ndotl;
            weight = weight + ndotl;
        }
    }

    color = color / weight;

    return color;
}

void main() {
    vec3 color = convolution_cube_map(glb_CubeMap, glb_FaceIndex, vs_TexCoord);

    //oColor = color / (1.0 + color);
    oColor = color;
}