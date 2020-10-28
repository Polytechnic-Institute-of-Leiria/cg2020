#version 330
// in_Position was bound to attribute index 0 and in_Color was bound to attribute index 1

in vec3 in_Position;
in vec3 in_Color;
in vec2 in_TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// We output the ex_Color variable to the next shader in the chain
out vec3 ex_Color;
out vec2 ex_TexCoord;

void main(void) {
	// GLSL allows shorthand use of vectors too, the following is also valid:
	gl_Position = projection * view * model * vec4(in_Position, 1.0);
	// We're simply passing the color and texture coords through unmodified
	ex_Color = in_Color;
	ex_TexCoord = in_TexCoord;
}