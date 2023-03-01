#version 460

layout (location=0) in vec3 pos;

uniform mat4 model;
uniform mat4 u_DirectionalLightSpaceTransform;

void main(){
	gl_Position = u_DirectionalLightSpaceTransform * model * vec4(pos, 1.0f);
}
