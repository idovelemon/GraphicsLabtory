#version 330

// Input attributes
in vec3 vs_Vertex;
in vec3 vs_Normal;

// Uniform
uniform vec3 glb_unif_ParallelLight_Dir;

// Constant value
uniform float glb_unif_MinOffset;
uniform float glb_unif_MaxOffset;

void main() {
	vec3 normal = normalize(vs_Normal);
	float offset = glb_unif_MinOffset + (glb_unif_MaxOffset - glb_unif_MinOffset) * dot(normal, -glb_unif_ParallelLight_Dir);

	// If you do not want to use fix pipeline, you must convert depth to [0,1]
	// so that we can store it in a float texture
	float depth = vs_Vertex.z + 1.0;
	depth /= 2.0;
	gl_FragDepth = depth + offset;
}