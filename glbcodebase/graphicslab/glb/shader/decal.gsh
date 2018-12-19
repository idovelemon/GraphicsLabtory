//----------------------------------------------------
// begin: Method about decal
//----------------------------------------------------

//----------------------------------------------------
// Calculate pixel's decal color
// decalProj: Decal pass projection matrix
// decalView: Decal pass view matrix
// pos: The world position
// decalTex: The texture hold all decals already
// return: Pixel's decal color with alpha
//----------------------------------------------------
vec4 glbCalculateDecalColor(mat4 decalProj, mat4 decalView, vec4 pos, sampler2D decalTex) {
  	vec4 decalTexcoord = decalProj * decalView * pos;
    decalTexcoord.xyz /= 2.0;
    decalTexcoord.xyz += 0.5;
    decalTexcoord.xyz /= decalTexcoord.w;

    return texture(decalTex, decalTexcoord.xy);
}

//----------------------------------------------------
// end: Method about decal
//----------------------------------------------------