#version 330 core

in vec3 ex_Color;
in vec2 ex_TexCoord;

uniform sampler2D ourTexture;

out vec4 FragColor;

void main(void) {
	// Pass through our original color with full opacity.
	// FragColor = vec4(ex_Color, 1.0);
	FragColor = texture(ourTexture, ex_TexCoord);
}