#version 330 core
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 
struct Light {
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    mat3 TBN;
} fs_in;

//in vec3 FragPos;
//in vec3 Normal;
//in vec2 TexCoords;

uniform sampler2D normalMap;
uniform vec3 lightColor;
uniform vec3 viewPos;

uniform Material material;
uniform Light light;
out vec4 FragColor;


void main()
{    
    
    FragColor = vec4(1.0, 0.0, 1.0, 1.0);
}