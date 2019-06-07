#version 330

// Input attributes
layout (location = 0) in vec3 glb_attr_Pos;
layout (location = 2) in vec3 glb_attr_Normal;
layout (location = 3) in vec3 glb_attr_Tangent;
layout (location = 4) in vec3 glb_attr_Binormal;
layout (location = 5) in vec2 glb_attr_TexCoord;

// Output attributes
out vec4 vs_Vertex;
out vec3 vs_Normal;
out vec3 vs_Tangent;
out vec3 vs_Binormal;
out vec2 vs_TexCoord;

uniform mat4 glb_unif_ProjM;
uniform mat4 glb_unif_ViewM;

uniform mat4 glb_unif_WorldM;
uniform mat4 glb_unif_Trans_Inv_WorldM;

uniform float glb_unif_Timer;
uniform float glb_unif_WindPower;
uniform float glb_unif_WindSpeed;
uniform vec3 glb_unif_WindDir;
uniform float glb_unif_HeightPower;

vec3 calc_wind_animation(vec2 uv, vec3 pos) {
    float height = pow(uv.y, glb_unif_HeightPower);
    float offset = height * glb_unif_WindPower * sin(uv.y * glb_unif_WindSpeed + glb_unif_WindSpeed * glb_unif_Timer);
    return pos + glb_unif_WindDir * offset;
}

void main() {
    vec3 pos = calc_wind_animation(glb_attr_TexCoord, glb_attr_Pos);
	gl_Position = glb_unif_ProjM * glb_unif_ViewM * glb_unif_WorldM * vec4(pos, 1.0);
	vs_Vertex = glb_unif_WorldM * vec4(pos, 1.0);

    vs_Normal = (glb_unif_Trans_Inv_WorldM * vec4(glb_attr_Normal, 0.0)).xyz;
    vs_Tangent = (glb_unif_Trans_Inv_WorldM * vec4(glb_attr_Tangent, 0.0)).xyz;
    vs_Binormal = (glb_unif_Trans_Inv_WorldM * vec4(glb_attr_Binormal, 0.0)).xyz;

	vs_TexCoord = glb_attr_TexCoord;
}