//----------------------------------------------------
// begin: Method about shadow calculating
//----------------------------------------------------

//----------------------------------------------------
// Calculate shadow map index
// pos: The vertex position
// eyePos: Camera position
// lookAt: Camera look at vector
// split0-2: PSSM split value 0,1,2
// return: Shadow map index
//----------------------------------------------------
int glbCalculateShadowIndex(vec3 pos, vec3 eyePos, vec3 lookAt, float sp0, float sp1, float sp2) {
    int index = -1;

    vec3 toVtx = eyePos - pos;
    float z = dot(toVtx, lookAt);
    if (z < sp0) {
        index = 0;
    } else if (sp0 < z && z < sp1) {
        index = 1;
    } else if (sp1 < z && z < sp2) {
        index = 2;
    } else if (z > sp2) {
        index = 3;
    }

    return index;
}

//----------------------------------------------------
// Calculate pixel shadow factor
// pos: The vertex position
// eyePos: Camera position
// lookAt: Camera look at vector
// split0-2: PSSM split value 0,1,2
// shadowM0-3: PSSM shadow matrix 0,1,2,3
// shadowMap0-3: PSSM shadow map 0,1,2,3
// return: Pixel shadow factor
//----------------------------------------------------
float glbCalculateShadowFactor(vec3 pos, vec3 eyePos, vec3 lookAt, 
                            float split0, float split1, float split2,
                            mat4 shadowM0, mat4 shadowM1, mat4 shadow2, mat4 shadow3,
                            sampler2D shadowMap0, sampler2D shadowMap1, sampler2D shadowMap2, sampler2D shadowMap3) {
	float shadowFactor = 1.0;

	int index = glbCalculateShadowIndex(pos, eyePos, lookAt, split0, split1, split2);
	vec4 lightSpacePos;

	if (index == 0) {
		lightSpacePos = shadowM0 * vec4(pos, 1.0);
		lightSpacePos.xyz /= 2.0f;
		lightSpacePos.xyz += 0.5f;
		lightSpacePos.xyz /= lightSpacePos.w;
		shadowFactor = texture2D(shadowMap0, lightSpacePos.xy).z;
	} else if (index == 1) {
		lightSpacePos = shadowM1 * vec4(pos, 1.0);
		lightSpacePos.xyz /= 2.0f;
		lightSpacePos.xyz += 0.5f;
		lightSpacePos.xyz /= lightSpacePos.w;	
		shadowFactor = texture2D(shadowMap1, lightSpacePos.xy).z;
	} else if (index == 2) {
		lightSpacePos = shadow2 * vec4(pos, 1.0);
		lightSpacePos.xyz /= 2.0f;
		lightSpacePos.xyz += 0.5f;
		lightSpacePos.xyz /= lightSpacePos.w;				
		shadowFactor = texture2D(shadowMap2, lightSpacePos.xy).z;
	} else {
		lightSpacePos = shadow3 * vec4(pos, 1.0);
		lightSpacePos.xyz /= 2.0f;
		lightSpacePos.xyz += 0.5f;
		lightSpacePos.xyz /= lightSpacePos.w;
		shadowFactor = texture2D(shadowMap3, lightSpacePos.xy).z;
	}

	if (shadowFactor < lightSpacePos.z) {
		// In shadow
		shadowFactor = 0.2;
	} else {
		// Out of shadow
		shadowFactor = 1.0;
	}
	if (lightSpacePos.x < 0.0 || 
		lightSpacePos.x > 1.0 ||
		lightSpacePos.y < 0.0 ||
		lightSpacePos.y > 1.0) {
		// Out of shadow
		shadowFactor = 1.0;
	}

	return shadowFactor;
}

//----------------------------------------------------
// end: Method about shadow calculating
//----------------------------------------------------