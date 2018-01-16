#version 330
#extension GL_NV_shadow_samplers_cube : enable

in vec2 vs_TexCoord;

out vec3 oColor;

uniform samplerCube glb_CubeMap;
uniform int glb_FaceIndex;

const float PI = 3.1415926536898;

vec3 filtering_cube_map(samplerCube cube, vec3 n) {
    n.yz = -n.yz;
    return textureCube(cube, n).xyz;
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

vec3 calc_cartesian(float phi, float theta) {
    return vec3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
}

vec3 convolution_cube_map(samplerCube cube, int faceIndex, vec2 uv) {
    // Calculate tangent space base vector
    vec3 n = calc_normal(faceIndex, uv);
    n = normalize(n);
    vec3 u = vec3(0.0, 1.0, 0.0);
    vec3 r = cross(u, n);
    r = normalize(r);
    u = cross(n, r);
    u = normalize(u);

    // Convolution
    float samplingStep = 0.025;
    int sampler = 0;
    vec3 l = vec3(0.0, 0.0, 0.0);
    for (float phi = 0.0; phi < 2.0 * PI; phi = phi + samplingStep) {
        for (float theta = 0.0; theta < 0.5 * PI; theta = theta + samplingStep) {
            vec3 d = calc_cartesian(phi, theta);  // Transform spherical coordinate to cartesian coordinate
            d = d.x * r + d.y * u + d.z * n;  // Transform tangent space coordinate to world space coordinate
            l = l + filtering_cube_map(glb_CubeMap, normalize(d)) * cos(theta) * sin(theta);  // L * (ndotv) * sin(theta) d(theta)d(phi)
            sampler = sampler + 1;
        }
    }
    l = PI * l * (1.0 / sampler);

    return l;
}

void main() {
    vec3 color = convolution_cube_map(glb_CubeMap, glb_FaceIndex, vs_TexCoord);
    oColor = color / (1.0 + color);
}