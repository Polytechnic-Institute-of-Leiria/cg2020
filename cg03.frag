#version 330 core

in vec3 ex_Color;
in vec2 ex_TexCoord;

out vec4 FragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform int texturesCount;
uniform vec2 textOffset;

void main(void) {
  if (texturesCount==0) {
    // Pass through our original color with full opacity.
    FragColor = vec4(ex_Color,1.0);
  }
  else if (texturesCount==1){
    FragColor = texture(texture1, ex_TexCoord + textOffset);
  }
  else {
    FragColor = mix(texture(texture1, ex_TexCoord + textOffset), texture(texture2, ex_TexCoord + textOffset), 0.5);
  }
}