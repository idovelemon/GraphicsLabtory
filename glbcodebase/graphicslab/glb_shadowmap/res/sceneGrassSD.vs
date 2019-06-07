#version 330

in vec3 glb_attr_Pos;
in vec3 glb_attr_Normal;
in vec2 glb_attr_TexCoord;

uniform mat4 glb_unif_ShadowM;
uniform mat4 glb_unif_WorldM;
uniform mat4 glb_unif_Trans_Inv_WorldM;

out vec3 vs_Vertex;
out vec3 vs_Normal;
out vec2 vs_TexCoord;

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
	mat4 shadowM = glb_unif_ShadowM;

    vec3 pos = calc_wind_animation(glb_attr_TexCoord, glb_attr_Pos);
	gl_Position = shadowM * glb_unif_WorldM * vec4(pos, 1.0);
	vs_Vertex = vec3(gl_Position.xyz) / gl_Position.w;
	//vs_Normal = (glb_unif_Trans_Inv_WorldM * vec4(glb_attr_Normal, 0.0)).xyz;
	vs_Normal = vec3(0.0, 1.0, 0.0);
	vs_TexCoord = glb_attr_TexCoord;
}