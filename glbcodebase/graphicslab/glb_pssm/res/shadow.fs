//----------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016 / 12 / 01
// Brief: Cast Shadow
//----------------------------------------------------
#version 330

// Input attributes
in vec3 vs_Vertex;

out vec3 oColor;

void main() {
	// If you do not want to use fix pipeline, you must convert depth to [0,1]
	// so that we can store it in a float texture
	float depth = vs_Vertex.z + 1.0;
	depth /= 2.0;

	oColor = vec3(depth, depth * depth, 0.0);
}


// // Input attributes
// in vec3 vs_Vertex;
// in vec3 vs_Normal;

// out vec3 oColor;

// // Uniform
// uniform vec3 glb_unif_ParallelLight_Dir;

// // Constant value
// const float cMinOffset = 0.001;
// const float cMaxOffset = 0.01;

// void main() {
// 	vec3 normal = normalize(vs_Normal);
// 	float offset = cMinOffset + (cMaxOffset - cMinOffset) * dot(normal, -glb_unif_ParallelLight_Dir);

// 	// If you do not want to use fix pipeline, you must convert depth to [0,1]
// 	// so that we can store it in a float texture
// 	float depth = vs_Vertex.z + 1.0;
// 	depth /= 2.0;
// 	oColor = vec3(depth + offset, depth + offset, depth + offset);
// }