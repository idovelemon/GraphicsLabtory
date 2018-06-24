# Shader Input(in attribute, uniform))(build-in)
|Name|Type|Description|
|----|----|-----------|
|glb_Pos|vec3|Position of vertex|
|glb_Color|vec3|Color of vertex|
|glb_Normal|vec3|Normal of vertex|
|glb_Tangent|vec3|Tangent of vertex|
|glb_Binormal|vec3|Binormal of vertex|
|glb_TexCoord|vec2|Texcoord of vertex|
|glb_LightMapTexCoord|vec2|Light map texcoord of vertex|
|glb_ProjM|mat4|Projection matrix of camera|
|glb_ViewM|mat4|View matrix of camera|
|glb_WorldM|mat4|World matrix of model|
|glb_ShadowM0|mat4|Shadow matrix 0|
|glb_ShadowM1|mat4|Shadow matrix 1|
|glb_ShadowM2|mat4|Shadow matrix 2|
|glb_ShadowM3|mat4|Shadow matrix 3|
|glb_ShadowSplit0|float|Frustum split value with shadow map 0|
|glb_ShadowSplit1|float|Frustum split value with shadow map 1|
|glb_ShadowSplit2|float|Frustum split value with shadow map 2|
|glb_ShadowSplit3|float|Frustum split value with shadow map 3|
|glb_ShadowMIndex|int|Current using shadow map index|
|glb_Trans_Inv_WorldM|mat4|World matrix's inverse-transpose matrix for normal mapping|
|glb_AlbedoTex|sampler2D|Albedo texture in material|
|glb_RoughnessTex|sampler2D|Roughness texture in material|
|glb_MetallicTex|sampler2D|Metallic texture in material|
|glb_AlphaTex|sampler2D|Alpha texture in material|
|glb_NormalTex|sampler2D|Normal texture in material|
|glb_EmissionTex|sampler2D|Emission texture in material|
|glb_DiffusePFCTex|samplerCube|Prefilter diffuse cubemap for IBL|
|glb_SpecularPFCTex|samplerCube|Prefilter specular cubemap for IBL|
|glb_BRDFPFTTex|sampler2D|Precompute brdf table texture for IBL|
|glb_SpecularPFCLOD|float|Specular prefilter texture's max lod level value|
|glb_Light0Tex|sampler2D|Precompute Radiosity Light Map 0|
|glb_Light1Tex|sampler2D|Precompute Radiosity Light Map 1|
|glb_Light2Tex|sampler2D|Precompute Radiosity Light Map 2|
|glb_ShadowTex0|sampler2D|Shadow map 0|
|glb_ShadowTex1|sampler2D|Shadow map 1|
|glb_ShadowTex2|sampler2D|Shadow map 2|
|glb_ShadowTex3|sampler2D|Shadow map 3|
|glb_Material_Ambient|vec3|Ambient material(Discard)|
|glb_Material_Diffuse|vec3|Diffuse material(Discard)|
|glb_Material_Specular|vec3|Specular material(Discard)|
|glb_Material_Emission|vec3|Emission material|
|glb_Material_Pow|float|Strength of specular(Discard)|
|glb_Matetrial_Albedo|vec3|Albedo of material|
|glb_Material_Roughness|float|Roughness of material|
|glb_Material_Metallic|float|Metallic of material|
|glb_EyePos|vec3|Position of camera|
|glb_LookAt|vec3|Target of camera|
|glb_GlobalLight_Ambient|vec3|Ambient light|
|glb_ParallelLight_Dir|vec3|Direction of parallel light|
|glb_ParallelLight|vec3|Color of parallel light|
|glb_FarClip|float|Far clip plane value of frustum|
|glb_ScreenWidth|float|Width of screen|
|glb_ScreenHeight|float|Height of screen|
|glb_DepthMap|sampler2D|Depth map of scene|
|glb_RandomRotateMap|sampler2D|Sampler kernel of SSAO|
|glb_AOMap|sampler2D|Ambient Occlusion map|
|glb_BiBlurMap|sampler2D|Blur Map for SSAO|
|glb_HighLightBase|float|Base light strength value for brightness filter|