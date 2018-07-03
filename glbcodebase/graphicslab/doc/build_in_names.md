# Shader Input(in attribute, uniform))(build-in)
|Attribute Name|Location|Type|Description|
|----|----|---|-----------|
|glb_attr_Pos|0|vec3|Position of vertex|
|glb_attr_Color|1|vec3|Color of vertex|
|glb_attr_Normal|2|vec3|Normal of vertex|
|glb_attr_Tangent|3|vec3|Tangent of vertex|
|glb_attr_Binormal|4|vec3|Binormal of vertex|
|glb_attr_TexCoord|5|vec2|Texcoord of vertex|
|glb_attr_LightMapTexCoord|6|vec2|Light map texcoord of vertex|
|glb_attr_WorldMatrix|7|mat4|Geometry Instance Rendering's world matrix|
|glb_attr_TransInvWorldMatrix|11|mat4|Geometry Instance Rendering's world matrix|

|Uniform Name|Type|Description|
|----|----|-----------|
|glb_unif_ProjM|mat4|Projection matrix of camera|
|glb_unif_ViewM|mat4|View matrix of camera|
|glb_unif_WorldM|mat4|World matrix of model|
|glb_unif_ShadowM0|mat4|Shadow matrix 0|
|glb_unif_ShadowM1|mat4|Shadow matrix 1|
|glb_unif_ShadowM2|mat4|Shadow matrix 2|
|glb_unif_ShadowM3|mat4|Shadow matrix 3|
|glb_unif_ShadowSplit0|float|Frustum split value with shadow map 0|
|glb_unif_ShadowSplit1|float|Frustum split value with shadow map 1|
|glb_unif_ShadowSplit2|float|Frustum split value with shadow map 2|
|glb_unif_ShadowSplit3|float|Frustum split value with shadow map 3|
|glb_unif_ShadowMIndex|int|Current using shadow map index|
|glb_unif_Trans_Inv_WorldM|mat4|World matrix's inverse-transpose matrix for normal mapping|
|glb_unif_AlbedoTex|sampler2D|Albedo texture in material|
|glb_unif_RoughnessTex|sampler2D|Roughness texture in material|
|glb_unif_MetallicTex|sampler2D|Metallic texture in material|
|glb_unif_AlphaTex|sampler2D|Alpha texture in material|
|glb_unif_NormalTex|sampler2D|Normal texture in material|
|glb_unif_EmissionTex|sampler2D|Emission texture in material|
|glb_unif_DiffusePFCTex|samplerCube|Prefilter diffuse cubemap for IBL|
|glb_unif_SpecularPFCTex|samplerCube|Prefilter specular cubemap for IBL|
|glb_unif_BRDFPFTTex|sampler2D|Precompute brdf table texture for IBL|
|glb_unif_SpecularPFCLOD|float|Specular prefilter texture's max lod level value|
|glb_unif_Light0Tex|sampler2D|Precompute Radiosity Light Map 0|
|glb_unif_Light1Tex|sampler2D|Precompute Radiosity Light Map 1|
|glb_unif_Light2Tex|sampler2D|Precompute Radiosity Light Map 2|
|glb_unif_ShadowTex0|sampler2D|Shadow map 0|
|glb_unif_ShadowTex1|sampler2D|Shadow map 1|
|glb_unif_ShadowTex2|sampler2D|Shadow map 2|
|glb_unif_ShadowTex3|sampler2D|Shadow map 3|
|glb_unif_Material_Ambient|vec3|Ambient material(Discard)|
|glb_unif_Material_Diffuse|vec3|Diffuse material(Discard)|
|glb_unif_Material_Specular|vec3|Specular material(Discard)|
|glb_unif_Material_Emission|vec3|Emission material|
|glb_unif_Material_Pow|float|Strength of specular(Discard)|
|glb_unif_Matetrial_Albedo|vec3|Albedo of material|
|glb_unif_Material_Roughness|float|Roughness of material|
|glb_unif_Material_Metallic|float|Metallic of material|
|glb_unif_EyePos|vec3|Position of camera|
|glb_unif_LookAt|vec3|Target of camera|
|glb_unif_GlobalLight_Ambient|vec3|Ambient light|
|glb_unif_ParallelLight_Dir|vec3|Direction of parallel light|
|glb_unif_ParallelLight|vec3|Color of parallel light|
|glb_unif_FarClip|float|Far clip plane value of frustum|
|glb_unif_ScreenWidth|float|Width of screen|
|glb_unif_ScreenHeight|float|Height of screen|
|glb_unif_DepthMap|sampler2D|Depth map of scene|
|glb_unif_RandomRotateMap|sampler2D|Sampler kernel of SSAO|
|glb_unif_AOMap|sampler2D|Ambient Occlusion map|
|glb_unif_BiBlurMap|sampler2D|Blur Map for SSAO|
|glb_unif_HighLightBase|float|Base light strength value for brightness filter|