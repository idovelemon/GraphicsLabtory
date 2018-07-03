//----------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017 / 02 / 16
// Brief: Depth map
//----------------------------------------------------
#version 330

in vec3 glb_attr_Pos;

uniform mat4 glb_unif_ProjM;
uniform mat4 glb_unif_ViewM;
uniform mat4 glb_unif_WorldM;

out float vs_DepthInViewSpace;

void main() {
	vec4 pos_in_view_space = glb_unif_ViewM * glb_unif_WorldM * vec4(glb_attr_Pos, 1.0);
	gl_Position = glb_unif_ProjM * pos_in_view_space;
	vs_DepthInViewSpace = pos_in_view_space.z / pos_in_view_space.w;
}