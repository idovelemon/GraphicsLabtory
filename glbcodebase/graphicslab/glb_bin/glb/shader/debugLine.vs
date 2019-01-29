//-----------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2018. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2018 / 07 / 03
// Brief: Draw debug color line
//-----------------------------------------------------------
#version 330

in vec3 glb_attr_Pos;
in vec3 glb_attr_Color;

uniform mat4 glb_unif_WVP;

out vec3 vs_Color;
out vec4 vs_Vertex;

void main() {
    vs_Vertex = glb_unif_WVP * vec4(glb_attr_Pos, 1.0);
	gl_Position = vs_Vertex;
	vs_Color = glb_attr_Color;
}