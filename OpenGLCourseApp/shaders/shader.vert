#version 460

layout (location=0) in vec3 pos;
layout (location=1) in vec2 texCoord;
layout (location=2) in vec3 norm;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 v_texCoord;
out vec3 v_normal;
out vec3 v_fragPos;

void main() { 
	gl_Position = projection * view * model * vec4(pos, 1.0); 
	v_texCoord = texCoord;
	v_normal = mat3(transpose(inverse(model))) * norm;
	v_fragPos = (model * vec4(pos, 1.0)).xyz; 
}