#version 330
// in_Position was bound to attribute index 0 and in_Color was bound to attribute index 1

in vec3 in_Position;
in vec3 in_Color;
in vec2 in_TexCoord;
in vec3 in_Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// We output the ex_Color variable to the next shader in the chain
out vec3 ex_Position;
out vec4 ex_Color;
out vec2 ex_TexCoord;
out vec3 ex_Normal;

void main(void) {
	mat4 vmMatrix;
	mat4 pvmMatrix;

	vmMatrix = view * model;
	pvmMatrix = projection * vmMatrix;

	gl_Position = pvmMatrix * vec4(in_Position, 1.0);

	// pass to fragment shader
	ex_Position = vec3(model * vec4(in_Position, 1.0));
	ex_Color = vec4(in_Color, 1.0);
	ex_TexCoord = in_TexCoord;
	ex_Normal = normalize(vec3(model * vec4(in_Normal, 1.0)));
}