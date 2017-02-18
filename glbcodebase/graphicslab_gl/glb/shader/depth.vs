//----------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017 / 02 / 16
// Brief: Depth map
//----------------------------------------------------
#version 330

in vec3 glb_Pos;

uniform mat4 glb_ProjM;
uniform mat4 glb_ViewM;
uniform mat4 glb_WorldM;

out vec4 vs_Vertex;

void main() {
	gl_Position = glb_ProjM * glb_ViewM * glb_WorldM * vec4(glb_Pos, 1.0);
	vs_Vertex = gl_Position;
}