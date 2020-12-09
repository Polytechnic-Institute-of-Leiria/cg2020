#version 330 core

#define TEXTURE_DIFFUSE1 0x01
#define TEXTURE_DIFFUSE2 0x02
#define TEXTURE_NORMAL 0x04
#define TEXTURE_ALPHA 0x08

in vec3 ex_Position;
in vec4 ex_Color;
in vec2 ex_TexCoord;
in vec3 ex_Normal;

in vec3 viewPosition;

out vec4 FragColor;

uniform int usedTextures;
uniform sampler2D diffuseTexture1;
uniform sampler2D diffuseTexture2;
uniform sampler2D normalTexture;
uniform sampler2D alphaTexture;

uniform vec3 diffuseColor;
uniform float specular;
uniform vec2 textOffset;

// Light model
uniform vec3 ambientComponent;
uniform vec3 lightPosition;
uniform vec3 lightColor;

void main(void) {

    vec4 ambient_color;
    vec4 diffuse_color;
    vec4 specular_color;

    vec4 pointColor;

    vec3 lightPointDir = normalize(ex_Position - lightPosition);
    vec3 normal;

    if ((usedTextures & TEXTURE_NORMAL) != 0) {
        normal = texture(normalTexture, ex_TexCoord + textOffset).rgb;
        normal = normalize(normal * 2.0 - 1.0);
    } else {
        normal = ex_Normal;
    }

  if (usedTextures==0) {
    // Pass through our original color with full opacity.
    pointColor = vec4(diffuseColor * lightColor, 1);
  }
  else if ((usedTextures & TEXTURE_DIFFUSE1)!=0 && (usedTextures & TEXTURE_DIFFUSE2)==0){
    // using diffuse texture 1 but not texture 2
    pointColor = texture(diffuseTexture1, ex_TexCoord + textOffset) * vec4(lightColor, 1.0);
  }
  else if ((usedTextures & TEXTURE_DIFFUSE1)!=0 && (usedTextures & TEXTURE_DIFFUSE2)!=0){
    // using diffuse texture 1 and 2
    pointColor = mix(texture(diffuseTexture1, ex_TexCoord + textOffset), texture(diffuseTexture2, ex_TexCoord + textOffset), 0.5) * vec4(lightColor, 1.0);
  }

  // ambient
  ambient_color = pointColor * vec4(ambientComponent, 1.0);
 
  // diffuse
  float diffStrength = dot(normal, -lightPointDir);
  if (diffStrength<0) {
    diffStrength *= -1;
  }
  diffuse_color = pointColor * diffStrength;
 
  // specular
  vec3 pointViewDir = normalize(viewPosition - ex_Position);
  vec3 reflectDir = reflect(lightPointDir, normal);
  float spec = pow(max(dot(pointViewDir, reflectDir), 0.0), 32);
  specular_color = specular / 100 * spec * vec4(lightColor, 1.0);

  FragColor = ambient_color + diffuse_color + specular_color;

  if ((usedTextures & TEXTURE_ALPHA)!=0) {
    FragColor[3] = texture(alphaTexture, ex_TexCoord + textOffset)[0];
  }

}