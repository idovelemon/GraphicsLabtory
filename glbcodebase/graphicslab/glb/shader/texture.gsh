//----------------------------------------------------
// begin: Method about texture
//----------------------------------------------------

//----------------------------------------------------
// Fetech cube map texel
// cube: Cube map
// n: Sampler direction vector
// return: Feteched texel
//----------------------------------------------------
vec3 glbFetechCubeMap(samplerCube cube, vec3 n) {
    n.yz = -n.yz;
    return texture(cube, n).xyz;
}

//----------------------------------------------------
// Fetech cube map texel with lod
// cube: Cube map
// n: Sampler direction vector
// lod: Mip map level
// return: Feteched texel
//----------------------------------------------------
vec3 glbFetechCubeMapLOD(samplerCube cube, vec3 n, float lod) {
    n.yz = -n.yz;
    return textureLod(cube, n, lod).xyz;
}

//----------------------------------------------------
// end: Method about texture
//----------------------------------------------------