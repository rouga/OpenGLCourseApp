#version 460

layout (location=0) in vec3 pos;
layout (location=1) in vec2 texCoord;
layout (location=2) in vec3 norm;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 u_DirectionalLightSpaceTransform;

out vec2 v_texCoord;
out vec3 v_normal;
out vec3 v_fragPos;
out vec4 v_DirectionalLightSpacePos;

void main() { 
	gl_Position = projection * view * model * vec4(pos, 1.0);
	v_DirectionalLightSpacePos = u_DirectionalLightSpaceTransform * model * vec4(pos, 1.0f);

	v_texCoord = texCoord;
	v_normal = mat3(transpose(inverse(model))) * norm;
	v_fragPos = (model * vec4(pos, 1.0)).xyz; 
}