//----------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017 / 02 / 16
// Brief: Depth map
//----------------------------------------------------
#version 330

// Input attributes
in float vs_DepthInViewSpace;

uniform float glb_unif_FarClip;

void main() {
	// Note: In opengl, camera look at -z axis
	// so the depth value in view space is negative value
	gl_FragDepth = -vs_DepthInViewSpace / glb_unif_FarClip;
}