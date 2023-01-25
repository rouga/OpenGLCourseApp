#version 460

in vec2 v_texCoord;
in vec3 v_normal;

out vec4 colour;

struct DirectionalLight {
	vec3 mColour;
	float mAmbientIntensity;
	vec3 mDirection;
	float mDiffuseIntensity;
};

uniform sampler2D dirtTexture;
uniform sampler2D brickTexture;

uniform DirectionalLight u_directionalLight;

void main() { 
	vec4 ambientColour = vec4(u_directionalLight.mColour, 1.0f) * u_directionalLight.mAmbientIntensity;

	float diffuseFactor = max(dot(normalize(v_normal), normalize(u_directionalLight.mDirection)), 0.0f);
	vec4 diffuseColor = vec4(u_directionalLight.mColour,1.0f) * u_directionalLight.mDiffuseIntensity * diffuseFactor;

	vec4 textureColour = texture(dirtTexture, v_texCoord)*0.3 + texture(brickTexture, v_texCoord)*0.7;
	colour = textureColour * (ambientColour + diffuseColor);
}