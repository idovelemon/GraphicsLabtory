//----------------------------------------------------
// begin: Method about shadow calculating
//----------------------------------------------------

//----------------------------------------------------
// Calculate shadow map index
// pos: The vertex position
// shadowM0-4: Split shadow matrix
// return: Shadow map index
//----------------------------------------------------
int glbCalculateShadowIndex(vec3 pos, mat4 shadowM0, mat4 shadowM1, mat4 shadowM2, mat4 shadowM3) {
    int index = -1;

	vec4 lightSpacePos0 = shadowM0 * vec4(pos, 1.0);
	lightSpacePos0.xyz /= lightSpacePos0.w;
	lightSpacePos0.xyz /= 2.0;
	lightSpacePos0.xyz += 0.5;

	vec4 lightSpacePos1 = shadowM1 * vec4(pos, 1.0);
	lightSpacePos1.xyz /= lightSpacePos1.w;
	lightSpacePos1.xyz /= 2.0;
	lightSpacePos1.xyz += 0.5;

	vec4 lightSpacePos2 = shadowM2 * vec4(pos, 1.0);
	lightSpacePos2.xyz /= lightSpacePos2.w;
	lightSpacePos2.xyz /= 2.0;
	lightSpacePos2.xyz += 0.5;

	vec4 lightSpacePos3 = shadowM3 * vec4(pos, 1.0);
	lightSpacePos3.xyz /= lightSpacePos3.w;
	lightSpacePos3.xyz /= 2.0;
	lightSpacePos3.xyz += 0.5;

	if (0.0 <= lightSpacePos0.x && lightSpacePos0.x <= 1.0
		&& 0.0 <= lightSpacePos0.y && lightSpacePos0.y <= 1.0) {
		index = 0;
	} else if (0.0 <= lightSpacePos1.x && lightSpacePos1.x <= 1.0
		&& 0.0 <= lightSpacePos1.y && lightSpacePos1.y <= 1.0) {
		index = 1;
	} else if (0.0 <= lightSpacePos2.x && lightSpacePos2.x <= 1.0
		&& 0.0 <= lightSpacePos2.y && lightSpacePos2.y <= 1.0) {
		index = 2;
	} else {
		index = 3;
	}

    return index;
}

//----------------------------------------------------
// Calculate shadow map index with blend band
// pos: The vertex position
// shadowM0-4: Split shadow matrix
// return: Shadow map index with blend amount
//----------------------------------------------------
int glbCalculateShadowIndexBlend(vec3 pos, mat4 shadowM0, mat4 shadowM1, mat4 shadowM2, mat4 shadowM3, out float blendAmount) {
    int index = -1;

	vec4 lightSpacePos0 = shadowM0 * vec4(pos, 1.0);
	lightSpacePos0.xyz /= lightSpacePos0.w;
	lightSpacePos0.xyz /= 2.0;
	lightSpacePos0.xyz += 0.5;

	vec4 lightSpacePos1 = shadowM1 * vec4(pos, 1.0);
	lightSpacePos1.xyz /= lightSpacePos1.w;
	lightSpacePos1.xyz /= 2.0;
	lightSpacePos1.xyz += 0.5;

	vec4 lightSpacePos2 = shadowM2 * vec4(pos, 1.0);
	lightSpacePos2.xyz /= lightSpacePos2.w;
	lightSpacePos2.xyz /= 2.0;
	lightSpacePos2.xyz += 0.5;

	vec4 lightSpacePos3 = shadowM3 * vec4(pos, 1.0);
	lightSpacePos3.xyz /= lightSpacePos3.w;
	lightSpacePos3.xyz /= 2.0;
	lightSpacePos3.xyz += 0.5;

	float blendBandArea = 0.25;
	if (0.0 <= lightSpacePos0.x && lightSpacePos0.x <= 1.0
		&& 0.0 <= lightSpacePos0.y && lightSpacePos0.y <= 1.0) {
		index = 0;
		vec2 distanceToOne = vec2(1.0 - lightSpacePos0.x, 1.0 - lightSpacePos0.y);
		float min0 = min(lightSpacePos0.x, lightSpacePos0.y);
		float min1 = min(distanceToOne.x, distanceToOne.y);
		float distanceToBorder = min(min0, min1);
		blendAmount = distanceToBorder / blendBandArea;
	} else if (0.0 <= lightSpacePos1.x && lightSpacePos1.x <= 1.0
		&& 0.0 <= lightSpacePos1.y && lightSpacePos1.y <= 1.0) {
		index = 1;
		vec2 distanceToOne = vec2(1.0 - lightSpacePos1.x, 1.0 - lightSpacePos1.y);
		float min0 = min(lightSpacePos1.x, lightSpacePos1.y);
		float min1 = min(distanceToOne.x, distanceToOne.y);
		float distanceToBorder = min(min0, min1);
		blendAmount = distanceToBorder / blendBandArea;
	} else if (0.0 <= lightSpacePos2.x && lightSpacePos2.x <= 1.0
		&& 0.0 <= lightSpacePos2.y && lightSpacePos2.y <= 1.0) {
		index = 2;
		vec2 distanceToOne = vec2(1.0 - lightSpacePos2.x, 1.0 - lightSpacePos2.y);
		float min0 = min(lightSpacePos2.x, lightSpacePos2.y);
		float min1 = min(distanceToOne.x, distanceToOne.y);
		float distanceToBorder = min(min0, min1);
		blendAmount = distanceToBorder / blendBandArea;
	} else {
		index = 3;
		vec2 distanceToOne = vec2(1.0 - lightSpacePos3.x, 1.0 - lightSpacePos3.y);
		float min0 = min(lightSpacePos3.x, lightSpacePos3.y);
		float min1 = min(distanceToOne.x, distanceToOne.y);
		float distanceToBorder = min(min0, min1);
		blendAmount = distanceToBorder / blendBandArea;
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
// return: Pixel shadow factor [0 or 1]
//----------------------------------------------------
float glbCalculateShadowFactor(vec3 pos, vec3 eyePos, vec3 lookAt, 
                            float split0, float split1, float split2,
                            mat4 shadowM0, mat4 shadowM1, mat4 shadowM2, mat4 shadowM3,
                            sampler2D shadowMap0, sampler2D shadowMap1, sampler2D shadowMap2, sampler2D shadowMap3) {
	float shadowFactor = 0.0;

	int index = glbCalculateShadowIndex(pos, shadowM0, shadowM1, shadowM2, shadowM3);
	vec4 lightSpacePos;

	if (index == 0) {
		lightSpacePos = shadowM0 * vec4(pos, 1.0);
		lightSpacePos.xyz /= lightSpacePos.w;
		lightSpacePos.xyz /= 2.0f;
		lightSpacePos.xyz += 0.5f;
		shadowFactor = texture2D(shadowMap0, lightSpacePos.xy).z;
	} else if (index == 1) {
		lightSpacePos = shadowM1 * vec4(pos, 1.0);
		lightSpacePos.xyz /= lightSpacePos.w;
		lightSpacePos.xyz /= 2.0f;
		lightSpacePos.xyz += 0.5f;	
		shadowFactor = texture2D(shadowMap1, lightSpacePos.xy).z;
	} else if (index == 2) {
		lightSpacePos = shadowM2 * vec4(pos, 1.0);
		lightSpacePos.xyz /= lightSpacePos.w;
		lightSpacePos.xyz /= 2.0f;
		lightSpacePos.xyz += 0.5f;				
		shadowFactor = texture2D(shadowMap2, lightSpacePos.xy).z;
	} else {
		lightSpacePos = shadowM3 * vec4(pos, 1.0);
		lightSpacePos.xyz /= lightSpacePos.w;
		lightSpacePos.xyz /= 2.0f;
		lightSpacePos.xyz += 0.5f;
		shadowFactor = texture2D(shadowMap3, lightSpacePos.xy).z;
	}

	if (shadowFactor < lightSpacePos.z && shadowFactor < 1.0) {
		// In shadow
		shadowFactor = 0.0;
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
// Calculate pixel shadow factor using PCF(Percent Closet Filter)
// pos: The vertex position
// eyePos: Camera position
// lookAt: Camera look at vector
// split0-2: PSSM split value 0,1,2
// shadowM0-3: PSSM shadow matrix 0,1,2,3
// shadowMap0-3: PSSM shadow map 0,1,2,3
// return: Pixel shadow factor [0 - 1]
//----------------------------------------------------
float glbCalculatePCFShadowFactor(vec3 pos, vec3 eyePos, vec3 lookAt, 
                            float split0, float split1, float split2,
                            mat4 shadowM0, mat4 shadowM1, mat4 shadowM2, mat4 shadowM3,
                            sampler2D shadowMap0, sampler2D shadowMap1, sampler2D shadowMap2, sampler2D shadowMap3) {
	float shadowFactor = 0.0;

	int index = glbCalculateShadowIndex(pos, shadowM0, shadowM1, shadowM2, shadowM3);
	vec4 lightSpacePos;
	vec2 offset = vec2(1.0 / glb_unif_ShadowMapWidth, 1.0 / glb_unif_ShadowMapHeight);

	if (index == 0) {
		lightSpacePos = shadowM0 * vec4(pos, 1.0);
		lightSpacePos.xyz /= lightSpacePos.w;
		lightSpacePos.xyz /= 2.0f;
		lightSpacePos.xyz += 0.5f;
		float singleShadowFactor = 0.0;
		for (float i = -1.5; i <= 1.5; i = i + 1.0) {
			for (float j = -1.5; j <= 1.5; j = j + 1.0) {
				vec2 newLightSpacePos = lightSpacePos.xy + vec2(offset.x * i, offset.y * j);
				if (newLightSpacePos.x < 0.0 || 
					newLightSpacePos.x > 1.0 ||
					newLightSpacePos.y < 0.0 ||
					newLightSpacePos.y > 1.0) {
					// Out of shadow
					//continue;
				}

				float shadowDepth = texture2D(shadowMap0, newLightSpacePos).z;
				if (shadowDepth < lightSpacePos.z && shadowDepth < 1.0) {
					// In shadow
					singleShadowFactor = singleShadowFactor + 1.0;
				}				
			}
		}
		shadowFactor = 1.0 - singleShadowFactor / 16.0;
	} else if (index == 1) {
		lightSpacePos = shadowM1 * vec4(pos, 1.0);
		lightSpacePos.xyz /= lightSpacePos.w;
		lightSpacePos.xyz /= 2.0f;
		lightSpacePos.xyz += 0.5f;
		float singleShadowFactor = 0.0;
		for (float i = -1.5; i <= 1.5; i = i + 1.0) {
			for (float j = -1.5; j <= 1.5; j = j + 1.0) {
				vec2 newLightSpacePos = lightSpacePos.xy + vec2(offset.x * i, offset.y * j);
				if (newLightSpacePos.x < 0.0 || 
					newLightSpacePos.x > 1.0 ||
					newLightSpacePos.y < 0.0 ||
					newLightSpacePos.y > 1.0) {
					// Out of shadow
					//continue;
				}

				float shadowDepth = texture2D(shadowMap1, newLightSpacePos).z;
				if (shadowDepth < lightSpacePos.z && shadowDepth < 1.0) {
					// In shadow
					singleShadowFactor = singleShadowFactor + 1.0;
				}				
			}
		}
		shadowFactor = 1.0 - singleShadowFactor / 16.0;
	} else if (index == 2) {
		lightSpacePos = shadowM2 * vec4(pos, 1.0);
		lightSpacePos.xyz /= lightSpacePos.w;
		lightSpacePos.xyz /= 2.0f;
		lightSpacePos.xyz += 0.5f;
		float singleShadowFactor = 0.0;
		for (float i = -1.5; i <= 1.5; i = i + 1.0) {
			for (float j = -1.5; j <= 1.5; j = j + 1.0) {
				vec2 newLightSpacePos = lightSpacePos.xy + vec2(offset.x * i, offset.y * j);
				if (newLightSpacePos.x < 0.0 || 
					newLightSpacePos.x > 1.0 ||
					newLightSpacePos.y < 0.0 ||
					newLightSpacePos.y > 1.0) {
					// Out of shadow
					//continue;
				}

				float shadowDepth = texture2D(shadowMap2, newLightSpacePos).z;
				if (shadowDepth < lightSpacePos.z && shadowDepth < 1.0) {
					// In shadow
					singleShadowFactor = singleShadowFactor + 1.0;
				}				
			}
		}
		shadowFactor = 1.0 - singleShadowFactor / 16.0;
	} else {
		lightSpacePos = shadowM3 * vec4(pos, 1.0);
		lightSpacePos.xyz /= lightSpacePos.w;
		lightSpacePos.xyz /= 2.0f;
		lightSpacePos.xyz += 0.5f;
		float singleShadowFactor = 0.0;
		for (float i = -1.5; i <= 1.5; i = i + 1.0) {
			for (float j = -1.5; j <= 1.5; j = j + 1.0) {
				vec2 newLightSpacePos = lightSpacePos.xy + vec2(offset.x * i, offset.y * j);
				if (newLightSpacePos.x < 0.0 || 
					newLightSpacePos.x > 1.0 ||
					newLightSpacePos.y < 0.0 ||
					newLightSpacePos.y > 1.0) {
					// Out of shadow
					//continue;
				}

				float shadowDepth = texture2D(shadowMap3, newLightSpacePos).z;
				if (shadowDepth < lightSpacePos.z && shadowDepth < 1.0) {
					// In shadow
					singleShadowFactor = singleShadowFactor + 1.0;
				}				
			}
		}
		shadowFactor = 1.0 - singleShadowFactor / 16.0;
	}

	return shadowFactor;
}

//----------------------------------------------------
// Calculate pixel shadow factor using VSM(Variance Shadow Map)
// pos: The vertex position
// eyePos: Camera position
// lookAt: Camera look at vector
// shadowM: PSSM shadow matrix
// shadowMap: PSSM shadow map
// return: Pixel shadow factor [0 - 1]
//----------------------------------------------------
float glbCalculateShadowFactorForOneMap(vec3 pos, vec3 eyePos, vec3 lookAt, mat4 shadowM, sampler2D shadowMap) {
	float shadowFactor = 1.0;
	float minVariance = 0.0001;

	vec4 lightSpacePos = shadowM * vec4(pos, 1.0);
	lightSpacePos.xyz /= lightSpacePos.w;
	lightSpacePos.xyz /= 2.0;
	lightSpacePos.xyz += 0.5;
	if (lightSpacePos.x <= 0.0 || 
		lightSpacePos.x >= 1.0 ||
		lightSpacePos.y <= 0.0 ||
		lightSpacePos.y >= 1.0) {
		// Out of shadow
		shadowFactor = 1.0;
	} else {
		vec2 shadowMoments = texture(shadowMap, lightSpacePos.xy).xy;
		if (lightSpacePos.z < shadowMoments.x) {
			// Out of shadow
			shadowFactor = 1.0;
		} else {
			float variance = shadowMoments.y - shadowMoments.x * shadowMoments.x;
			variance = min(1.0, max(minVariance, variance));
			float d = lightSpacePos.z - shadowMoments.x;
			float pmax = variance / (variance + d * d);
			shadowFactor = pow(pmax, 5.0);
		}
	}

	return shadowFactor;
}

//----------------------------------------------------
// Calculate pixel shadow factor using VSM(Variance Shadow Map)
// pos: The vertex position
// eyePos: Camera position
// lookAt: Camera look at vector
// split0-2: PSSM split value 0,1,2
// shadowM0-3: PSSM shadow matrix 0,1,2,3
// shadowMap0-3: PSSM shadow map 0,1,2,3
// return: Pixel shadow factor [0 - 1]
//----------------------------------------------------
float glbCalculateVSMShadowFactor(vec3 pos, vec3 eyePos, vec3 lookAt, 
                            float split0, float split1, float split2,
                            mat4 shadowM0, mat4 shadowM1, mat4 shadowM2, mat4 shadowM3,
                            sampler2D shadowMap0, sampler2D shadowMap1, sampler2D shadowMap2, sampler2D shadowMap3) {
	float shadowFactor = 1.0;
	float blendAmount = 0.0;
	int index = glbCalculateShadowIndexBlend(pos, shadowM0, shadowM1, shadowM2, shadowM3, blendAmount);
	vec4 lightSpacePos;

	if (index == 0) {
		shadowFactor = glbCalculateShadowFactorForOneMap(pos, eyePos, lookAt, shadowM0, shadowMap0);
		if (blendAmount < 1.0) {
			float f1 = glbCalculateShadowFactorForOneMap(pos, eyePos, lookAt, shadowM1, shadowMap1);
			shadowFactor = mix(f1, shadowFactor, blendAmount);
		}
	} else if (index == 1) {
		shadowFactor = glbCalculateShadowFactorForOneMap(pos, eyePos, lookAt, shadowM1, shadowMap1);
		if (blendAmount < 1.0) {
			float f1 = glbCalculateShadowFactorForOneMap(pos, eyePos, lookAt, shadowM2, shadowMap2);
			shadowFactor = mix(f1, shadowFactor, blendAmount);
		}
	} else if (index == 2) {
		
		shadowFactor = glbCalculateShadowFactorForOneMap(pos, eyePos, lookAt, shadowM2, shadowMap2);
		if (blendAmount < 1.0) {
			float f1 = glbCalculateShadowFactorForOneMap(pos, eyePos, lookAt, shadowM3, shadowMap3);
			shadowFactor = mix(f1, shadowFactor, blendAmount);
		}
	} else {
		shadowFactor = glbCalculateShadowFactorForOneMap(pos, eyePos, lookAt, shadowM3, shadowMap3);
	}

	return shadowFactor;
}

//----------------------------------------------------
// end: Method about shadow calculating
//----------------------------------------------------