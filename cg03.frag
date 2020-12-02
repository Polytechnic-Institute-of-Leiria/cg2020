#version 330 core

#define TEXTURE_DIFFUSE1 0x01
#define TEXTURE_DIFFUSE2 0x02
#define TEXTURE_NORMAL 0x04
#define TEXTURE_ALPHA 0x08

in vec3 ex_Color;
in vec2 ex_TexCoord;

out vec4 FragColor;

uniform int usedTextures;
uniform sampler2D diffuseTexture1;
uniform sampler2D diffuseTexture2;
uniform sampler2D normalTexture;
uniform sampler2D alphaTexture;

uniform vec3 diffuseColor;
uniform float shininess;
uniform vec2 textOffset;

void main(void) {
  if (usedTextures==0) {
    // Pass through our original color with full opacity.
    //FragColor = vec4(ex_Color,1.0);
    FragColor = vec4(diffuseColor,1.0);
  }
  else if ((usedTextures & TEXTURE_DIFFUSE1)!=0 && (usedTextures & TEXTURE_DIFFUSE2)==0){
    // using diffuse texture 1 but not texture 2
    FragColor = texture(diffuseTexture1, ex_TexCoord + textOffset);
  }
  else if ((usedTextures & TEXTURE_DIFFUSE1)!=0 && (usedTextures & TEXTURE_DIFFUSE2)!=0){
    // using diffuse texture 1 and 2
    FragColor = mix(texture(diffuseTexture1, ex_TexCoord + textOffset), texture(diffuseTexture2, ex_TexCoord + textOffset), 0.5);
  }

  if ((usedTextures & TEXTURE_ALPHA)!=0) {
    FragColor[3] = texture(alphaTexture, ex_TexCoord + textOffset)[0];
  }

}