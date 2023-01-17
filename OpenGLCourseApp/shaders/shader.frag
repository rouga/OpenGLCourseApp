#version 460

in vec2 v_texCoord;

out vec4 color;

uniform sampler2D dirtTexture;
uniform sampler2D brickTexture;

void main() { 
	color = texture(dirtTexture, v_texCoord)*0.3 + texture(brickTexture, v_texCoord)*0.7;
}