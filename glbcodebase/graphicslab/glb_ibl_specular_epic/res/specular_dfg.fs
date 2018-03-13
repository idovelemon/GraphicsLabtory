#version 330

in vec2 vs_TexCoord;

out vec3 oColor;

const float PI = 3.1415926536898;

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

float calc_Geometry_GGX_IBL(float costheta, float roughness) {
    float a = roughness * roughness;
    float k = a / 2.0;

    float t = costheta * (1.0 - k) + k;

    return costheta / t;
}

float calc_Geometry_Smith_IBL(vec3 n, vec3 v, vec3 l, float roughness) {
    float ndotv = max(dot(n, v), 0.0);
    float ndotl = max(dot(n, l), 0.0);
    float ggx1 = calc_Geometry_GGX_IBL(ndotv, roughness);
    float ggx2 = calc_Geometry_GGX_IBL(ndotl, roughness);
    return ggx1 * ggx2;    
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

vec3 convolution_cube_map(vec2 uv) {
    vec3 n = vec3(0.0, 0.0, 1.0);
    float roughness = uv.y;
    float ndotv = uv.x;

    vec3 v = vec3(0.0, 0.0, 0.0);
    v.x = sqrt(1.0 - ndotv * ndotv);
    v.z = ndotv;

    float scalar = 0.0;
    float bias = 0.0;

    // Convolution
    uint sampler = 1024u;
    for (uint i = 0u; i < sampler; i++) {
        vec2 xi = hammersley(i, sampler);
        vec3 h = importance_sampling_ggx(xi, roughness, n);
        vec3 l = 2.0 * dot(v, h) * h - v;

        float ndotl = max(0.0, l.z);
        float ndoth = max(0.0, h.z);
        float vdoth = max(0.0, dot(v, h));

        if (ndotl > 0.0) {
            float G = calc_Geometry_Smith_IBL(n, v, l, roughness);

            float G_vis = G * vdoth / (ndotv * ndoth);
            float Fc = pow(1.0 - vdoth, 5.0);

            scalar = scalar + G_vis * (1.0 - Fc);
            bias = bias + G_vis * Fc;
        }
    }


    vec3 color = vec3(scalar, bias, 0.0);
    color = color / sampler;

    return color;
}

void main() {
    vec3 color = convolution_cube_map(vs_TexCoord);

    oColor = color;
}