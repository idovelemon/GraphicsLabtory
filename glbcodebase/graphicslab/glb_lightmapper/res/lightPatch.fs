#version 450

in vec2 vsTexCoord;
in vec3 vsVertex;

out vec3 oColor;

uniform sampler2D glb_NormalizeWeightMap;
uniform sampler2D glb_LightMap;
uniform vec3 glb_LightColor;
uniform int glb_Face;

void main() {
    if (glb_Face == 0 && vsVertex.x <= 0.0) {  // Right face
        if (glb_LightColor.x <= 0.0 && glb_LightColor.y <= 0.0 && glb_LightColor.z <= 0.0) {
            oColor = texture(glb_LightMap, vsTexCoord).rgb;
        } else {
            oColor = glb_LightColor;
        }
    } else if (glb_Face == 1 && vsVertex.x >= 0.0) {  // Left face
        if (glb_LightColor.x <= 0.0 && glb_LightColor.y <= 0.0 && glb_LightColor.z <= 0.0) {
            oColor = texture(glb_LightMap, vsTexCoord).rgb;
        } else {
            oColor = glb_LightColor;
        }
    } else if (glb_Face == 2 && vsVertex.y <= 0.0) { // Up face
        if (glb_LightColor.x <= 0.0 && glb_LightColor.y <= 0.0 && glb_LightColor.z <= 0.0) {
            oColor = texture(glb_LightMap, vsTexCoord).rgb;
        } else {
            oColor = glb_LightColor;
        }
    } else if (glb_Face == 3 && vsVertex.y >= 0.0) { // Down face
        if (glb_LightColor.x <= 0.0 && glb_LightColor.y <= 0.0 && glb_LightColor.z <= 0.0) {
            oColor = texture(glb_LightMap, vsTexCoord).rgb;
        } else {
            oColor = glb_LightColor;
        }
    } else if (glb_Face == 4) {  // Front face
        if (glb_LightColor.x <= 0.0 && glb_LightColor.y <= 0.0 && glb_LightColor.z <= 0.0) {
            oColor = texture(glb_LightMap, vsTexCoord).rgb;
        } else {
            oColor = glb_LightColor;
        }
    } else {
        oColor = vec3(0.0, 0.0, 0.0);
    }

    vec2 uv = (vec2(vsVertex.x, vsVertex.y) + vec2(1.0, 1.0)) / 2.0;
    oColor = oColor * texture(glb_NormalizeWeightMap, uv).rgb;
}