#version 460

layout (location=0) in vec3 pos;

uniform mat4 model;
uniform mat4 DirectionalLightSpaceTransform;

void main(){
	gl_Position = DirectionallightSpaceTransform * model * vec4(pos, 1.0f);
}
