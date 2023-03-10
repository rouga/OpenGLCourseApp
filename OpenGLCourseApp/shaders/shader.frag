#version 460

in vec2 v_texCoord;
in vec3 v_normal;
in vec3 v_fragPos;
in vec4 v_DirectionalLightSpacePos;

out vec4 colour;

const int MAX_POINT_LIGHTS = 3;
const int MAX_SPOT_LIGHTS = 3;

struct Light{
	vec3 colour;
	float ambientIntensity;
	float diffuseIntensity;
};

struct DirectionalLight {
	Light base;
	vec3 direction;
	
};

struct PointLight {
	Light base;
	vec3 position;
	float constant;
	float linear;
	float exponent;
	
};

struct SpotLight {
	PointLight base;
	vec3 direction;
	float edge;
	
};

struct Material{
	float specularIntensity;
	float shininess;
};

uniform int u_pointLightCount;
uniform int u_spotLightCount;

uniform DirectionalLight u_directionalLight;
uniform PointLight u_pointLights[MAX_POINT_LIGHTS];
uniform SpotLight u_spotLights[MAX_SPOT_LIGHTS];

uniform sampler2D u_TextureSlot0;
uniform sampler2D u_directionalShadowMap;
uniform sampler2D brickTexture;

uniform Material u_material;

uniform vec3 u_EyePosition;

float CalcDirectionalShadowFactor(DirectionalLight iLight){
	vec3 wProjCoords = v_DirectionalLightSpacePos.xyz / v_DirectionalLightSpacePos.w;
	wProjCoords = (wProjCoords * 0.5) + 0.5;

	float wClosestDepth = texture(u_directionalShadowMap, wProjCoords.xy).r;
	float wModelDepth = wProjCoords.z;

	vec3 wNormal = normalize(v_normal);
	vec3 wLightDir = normalize(iLight.direction);	

	float wBias = max(0.05 * (1- dot(wNormal, wLightDir)), 0.005);

	float wShadow = 0;
	vec2 wTexelSize = 1.0 / textureSize(u_directionalShadowMap, 0);
	for(int x = -1; x <= 1; ++x)
	{
		for(int y = -1; y <= 1; ++y)
		{
			float wPCFDepth = texture(u_directionalShadowMap, wProjCoords.xy + vec2(x,y) * wTexelSize).r;
			wShadow += wModelDepth - wBias > wPCFDepth ? 1.0 : 0.0;
		}
	}

	wShadow /= 9.0f;

	if(wProjCoords.z > 1.0f){
		wShadow = 0.0f;
	}

	return wShadow;
}

vec4 CalcLightByDirection(Light iLight, vec3 iDirection, float iShadowFactor){

	vec4 ambientColour = vec4(iLight.colour, 1.0f) * iLight.ambientIntensity;

	float diffuseFactor = max(dot(normalize(v_normal), normalize(iDirection)), 0.0f);
	vec4 diffuseColor = vec4(iLight.colour,1.0f) * iLight.diffuseIntensity * diffuseFactor;

	vec4 specularColor = vec4(0,0,0,0);

	if(diffuseFactor > 0.0f){
		vec3 fragToEye = normalize(u_EyePosition - v_fragPos);
		vec3 reflectedVertex = normalize(reflect(iDirection, normalize(v_normal)));

		float specularFactor = dot(fragToEye, reflectedVertex);
		if(specularFactor > 0.0f){
			specularFactor = pow(specularFactor, u_material.shininess);
			specularColor = vec4(iLight.colour * u_material.specularIntensity * specularFactor, 1.0f);
		}
	}
	return (ambientColour +  (1.0 - iShadowFactor) * (diffuseColor + specularColor));
}

vec4 CalcDirectionalLight() {
	float wShadowFactor = CalcDirectionalShadowFactor(u_directionalLight);
	return CalcLightByDirection(u_directionalLight.base, u_directionalLight.direction, wShadowFactor);
}

vec4 CalcPointLight(PointLight iPointLight){
	
	vec3 wDirection = v_fragPos - iPointLight.position;
	float wDistance = length(wDirection);
	wDirection = normalize(wDirection);

	vec4 wColor = CalcLightByDirection(iPointLight.base, wDirection, 0.0f);
	float wAttenuation = iPointLight.exponent * wDistance * wDistance +
						iPointLight.linear * wDistance +
						iPointLight.constant;

	return (wColor / wAttenuation);

}

vec4 CalcSpotLight(SpotLight iSpotLight){
	vec3 wRayDirection = normalize(v_fragPos - iSpotLight.base.position);
	float slFactor = dot(wRayDirection, iSpotLight.direction);

	if(slFactor > iSpotLight.edge){
		vec4 color = CalcPointLight(iSpotLight.base);
		return color * (1.0f - (1.0f - slFactor) * (1.0f/(1.0f - iSpotLight.edge)));
	} else{
		return vec4(0,0,0,0);
	}

}

vec4 CalcPointLights() {
	vec4 wTotalColour = vec4(0,0,0,0);
	for(int i=0; i < u_pointLightCount; i++){
		wTotalColour += CalcPointLight(u_pointLights[i]);
	}

	return wTotalColour;
}

vec4 CalcSpotLights(){
	vec4 wTotalColour = vec4(0,0,0,0);
	for(int i=0; i < u_spotLightCount; i++){
		wTotalColour += CalcSpotLight(u_spotLights[i]);
	}

	return wTotalColour;
}

void main() { 
	vec4 finalColor = CalcDirectionalLight();
	finalColor += CalcPointLights();
	finalColor += CalcSpotLights();
	vec4 textureColour = texture(u_TextureSlot0, v_texCoord);
	colour = textureColour * finalColor;
}