#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec2 TexCoords;
in vec3 FragPos;

struct Material
{
	vec3  ambient;
	sampler2D  diffuse;
	vec3  specular;
	float shininess;
};

struct Light
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

uniform Material material;
uniform Light    light;


uniform sampler2D texture_diffuse1;
uniform vec3 viewPos;
uniform vec3 lightPos;


void main()
{   
	// 环境光
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

    // 漫反射 
    vec3  norm = normalize(Normal);
    vec3  lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);//随角度变化递减
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

    // 镜面光
    vec3  viewDir = normalize(viewPos - FragPos);
    vec3  reflectDir = reflect(-lightDir, norm); //用入射光和法线计算反射光 
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3  specular = light.specular * (spec * material.specular);

	float dist = length(light.position - FragPos);
	float attenuation = 1.0/(light.constant + light.linear * dist + light.quadratic * dist * dist);

    vec3  result = (ambient + diffuse + specular) * attenuation;
    FragColor = vec4(result, 1.0);
}