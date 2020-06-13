#version 330 core
  

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aTangent;

out VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    mat3 TBN;
} vs_out;  

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;

uniform vec3 lightColor;
uniform vec3 viewPos;

//transformi toti vectorii din planul tangent in planul vectorilor de iluminare
void main()
{
	vec3 T = normalize(vec3(model * vec4(aTangent, 0.0)));
	vec3 N = normalize(vec3(model * vec4(aNormal, 0.0)));

	//scoti componenta normala de pe tangenta
	T = normalize(T - dot(T,N) * N);

	//faci produs vectorial ca sa obtii bitangenta perpendiculara pe T si B
	//si sa ai sistem ortonormat
	vec3 B = cross(N, T);

	mat3 TBN = mat3(T,B,N);

    vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
	vs_out.TexCoords = aTexCoord;	
	vs_out.TBN = TBN;

	gl_Position = projection * view * model * vec4(aPos, 1.0);
}
