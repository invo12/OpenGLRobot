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
    // ambient
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, fs_in.TexCoords));
  	
    // diffuse 
	
    vec3 norm = texture(normalMap, fs_in.TexCoords).rgb;
	norm = normalize(norm * 2.0 - 1.0);
	norm = normalize(fs_in.TBN * norm);

    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, fs_in.TexCoords));
    
    // specular
    vec3 viewDir = normalize(viewPos -	fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, fs_in.TexCoords));
        
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}