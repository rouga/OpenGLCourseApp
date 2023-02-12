#version 460

in vec2 v_texCoord;
in vec3 v_normal;
in vec3 v_fragPos;

out vec4 colour;

struct DirectionalLight {
	vec3 mColour;
	float mAmbientIntensity;
	vec3 mDirection;
	float mDiffuseIntensity;
};

struct Material{
	float mSpecularIntensity;
	float mShininess;
};

uniform sampler2D dirtTexture;
uniform sampler2D brickTexture;
uniform DirectionalLight u_directionalLight;
uniform Material u_material;

uniform vec3 u_EyePosition;

void main() { 
	vec4 ambientColour = vec4(u_directionalLight.mColour, 1.0f) * u_directionalLight.mAmbientIntensity;

	float diffuseFactor = max(dot(normalize(v_normal), normalize(u_directionalLight.mDirection)), 0.0f);
	vec4 diffuseColor = vec4(u_directionalLight.mColour,1.0f) * u_directionalLight.mDiffuseIntensity * diffuseFactor;

	vec4 specularColor = vec4(0,0,0,0);

	if(diffuseFactor > 0.0f){
		vec3 fragToEye = normalize(u_EyePosition - v_fragPos);
		vec3 reflectedVertex = normalize(reflect(u_directionalLight.mDirection, normalize(v_normal)));

		float specularFactor = dot(fragToEye, reflectedVertex);
		if(specularFactor > 0.0f){
			specularFactor = pow(specularFactor, u_material.mShininess);
			specularColor = vec4(u_directionalLight.mColour * u_material.mSpecularIntensity * specularFactor, 1.0f);
		}
	}

	vec4 textureColour = texture(dirtTexture, v_texCoord)*0.3 + texture(brickTexture, v_texCoord)*0.7;
	colour = textureColour * (ambientColour + diffuseColor + specularColor);
}