#version 460

in vec2 v_texCoord;
in vec3 v_normal;
in vec3 v_fragPos;

out vec4 colour;

const int MAX_POINT_LIGHTS = 3;

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

struct Material{
	float specularIntensity;
	float shininess;
};

uniform int u_pointLightCount;
uniform DirectionalLight u_directionalLight;
uniform PointLight u_pointLights[MAX_POINT_LIGHTS];

uniform sampler2D u_TextureSlot0;
uniform sampler2D brickTexture;

uniform Material u_material;

uniform vec3 u_EyePosition;

vec4 CalcLightByDirection(Light iLight, vec3 iDirection){

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
	return (ambientColour + diffuseColor + specularColor);
}

vec4 CalcDirectionalLight() {
	return CalcLightByDirection(u_directionalLight.base, u_directionalLight.direction);
}

vec4 CalcPointLights() {
	vec4 wTotalColour = vec4(0,0,0,0);
	for(int i=0; i < u_pointLightCount; i++){
		vec3 wDirection = v_fragPos - u_pointLights[i].position;
		float wDistance = length(wDirection);
		wDirection = normalize(wDirection);

		vec4 wColor = CalcLightByDirection(u_pointLights[i].base, wDirection);
		float wAttenuation = u_pointLights[i].exponent * wDistance * wDistance +
							u_pointLights[i].linear * wDistance +
							u_pointLights[i].constant;

		wTotalColour += (wColor / wAttenuation);
	}

	return wTotalColour;
}

void main() { 
	vec4 finalColor = CalcDirectionalLight();
	finalColor += CalcPointLights();
	vec4 textureColour = texture(u_TextureSlot0, v_texCoord);
	colour = textureColour * finalColor;
}