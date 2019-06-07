#version 330

// Input attributes
in vec3 vs_Vertex;
in vec3 vs_Normal;
in vec2 vs_TexCoord;

out vec3 oColor;

// Uniform
uniform vec3 glb_unif_ParallelLight_Dir;

// Constant value
uniform float glb_unif_MinOffset;
uniform float glb_unif_MaxOffset;

uniform sampler2D glb_unif_MaskMap;

void main() {
	vec4 mask = texture(glb_unif_MaskMap, vs_TexCoord, 0);
	if (mask.w < 0.5 || vs_TexCoord.y > 0.9) discard;

	float depth = vs_Vertex.z;
	depth = depth + 1.0;
	depth = depth / 2.0;

	oColor = vec3(depth, depth * depth, 0.0);
}