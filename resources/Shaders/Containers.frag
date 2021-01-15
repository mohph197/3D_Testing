#version 330 core

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};
uniform Material material;

struct DirLight {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform DirLight dirLight;
vec3 CalculateDirLight(DirLight light, vec3 normal, vec3 viewDir);

struct PointLight {
	vec3 position;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
#define NR_POINT_LIGHTS 4
uniform PointLight pointLight[NR_POINT_LIGHTS];
vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

struct SpotLight {
	vec3 position;
	vec3 direction;

	float innerCutOff;
	float outerCutOff;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform SpotLight spotLight;
vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

in vec3 FragPos;
in vec2 TexCoords;
in vec3 Normal;

uniform vec3 ViewPos;
uniform vec3 Brightness;

void main()
{
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(ViewPos - FragPos);

	vec3 result = CalculateDirLight(dirLight, norm, viewDir);

	for(int i = 0; i < NR_POINT_LIGHTS; i++)
		result += CalculatePointLight(pointLight[i], norm, FragPos, viewDir);

	result += CalculateSpotLight(spotLight, norm, FragPos, viewDir);

	gl_FragColor = vec4(result, 1.0);
}

vec3 CalculateDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);

	float diff = max(dot(lightDir, normal), 0.0);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);

	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

	return (ambient + diffuse + specular);
}

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);

	float diff = max(dot(lightDir, normal), 0.0);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);

	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	vec3 ambient = attenuation * light.ambient * vec3(texture(material.diffuse, TexCoords));
	vec3 diffuse = attenuation * light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
	vec3 specular = attenuation * light.specular * spec * vec3(texture(material.specular, TexCoords));

	return (ambient + diffuse + specular);
}

vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);

	float diff = max(dot(lightDir, normal), 0.0);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);

	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	float theta = dot(-lightDir, light.direction);
	float intensity = clamp((theta - light.outerCutOff) / (light.innerCutOff - light.outerCutOff), 0.0, 1.0);

	vec3 ambient =  attenuation * light.ambient  * vec3(texture(material.diffuse, TexCoords));
	vec3 diffuse =  attenuation * intensity * light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
	vec3 specular = attenuation * intensity * light.specular * spec * vec3(texture(material.specular, TexCoords));

	return (ambient + diffuse + specular);
}
