#version 330 core

out vec4 Fragcolor;
in vec2 thetex;

uniform vec3 lightcolor;
uniform vec3 viewposition;

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;

};
struct Light{
	vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

	float constant;
	float linear;
	float quadratic;

};

uniform Light light;
uniform Material material;

in vec3 Normal;
in vec3 fragmentposition;

void main()
{
	//ambient 
	float distance = length(light.position - fragmentposition);
	float attenuation= 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
		
	vec3 ambient = vec3(texture(material.diffuse, thetex)) * light.ambient;
	ambient *= attenuation;

	
	
	//difuse
	vec3 norm = normalize(Normal);
	vec3 lightdirection = normalize(light.position - fragmentposition);
	float diff = max(dot(norm, lightdirection), 0.0);
	vec3 diffuse = diff * vec3(texture(material.diffuse, thetex)) * light.diffuse;
	
	diffuse *= attenuation;

	vec3 viewdirection = normalize(viewposition - fragmentposition);
	vec3 reflectdirection = reflect(-lightdirection, norm);
	
	float spec = pow(max(dot(viewdirection, reflectdirection),0.0),material.shininess);
	vec3 specular = vec3(texture(material.specular, thetex)) * spec * light.specular;

	specular *= attenuation;
	vec3 result = (ambient + diffuse + specular)  * lightcolor;
	Fragcolor =vec4(result , 1.0);

} 