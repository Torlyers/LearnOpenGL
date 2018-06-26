#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec2 TexCoords;
in vec3 FragPos;

struct Material
{
	vec3  ambient;
	sampler2D  diffuse;
	sampler2D  specular;
	float shininess;
};

struct ParallelLight
{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight
{
	vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

struct SpotLight
{
	vec3 position;
	vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
	float cutOff;
    float outerCutOff;

	float constant;
	float linear;
	float quadratic;

	float biasAngle;
};

uniform Material material;

uniform vec3 viewPos;
uniform ParallelLight paraLight;
uniform PointLight    pointLight;
uniform SpotLight     spotLight;

vec3 CalParallelLight(ParallelLight i_light, vec3 i_normal, vec3 i_viewDir);
vec3 CalPointLight   (PointLight i_light,    vec3 i_normal, vec3 i_fragPos, vec3 i_viewDir);
vec3 CalSpotLight    (SpotLight i_light,     vec3 i_normal, vec3 i_fragPos, vec3 i_viewDir);

void main()
{   
	vec3 norm    = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

	vec3 result = CalPointLight(pointLight, norm, FragPos, viewDir);

	FragColor = vec4(result, 1.0f);
}

vec3 CalParallelLight(ParallelLight i_light, vec3 i_normal, vec3 i_viewDir)
{
	vec3 lightDir = normalize(-i_light.direction);
	
	// 环境光
    vec3 ambient = i_light.ambient * vec3(texture(material.diffuse, TexCoords));

    // 漫反射 
    float diff    = max(dot(i_normal, lightDir), 0.0);//随角度变化递减
    vec3  diffuse = i_light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

    // 镜面光
    vec3  reflectDir = reflect(-lightDir, i_normal); //用入射光和法线计算反射光 
    float spec       = pow(max(dot(i_viewDir, reflectDir), 0.0), material.shininess);
    vec3  specular = i_light.specular * spec * vec3(texture(material.specular, TexCoords));

	return ambient + diffuse + specular;
}

vec3 CalPointLight(PointLight i_light, vec3 i_normal, vec3 i_fragPos, vec3 i_viewDir)
{
	vec3 lightDir = normalize(i_light.position - i_fragPos);
	
	// 环境光
    vec3 ambient = i_light.ambient * vec3(texture(material.diffuse, TexCoords));

    // 漫反射 
    float diff = max(dot(i_normal, lightDir), 0.0);//随角度变化递减
    vec3  diffuse = i_light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

    // 镜面光
    vec3  reflectDir = reflect(-lightDir, i_normal); //用入射光和法线计算反射光 
    float spec = pow(max(dot(i_viewDir, reflectDir), 0.0), material.shininess);
    vec3  specular = i_light.specular * spec * vec3(texture(material.specular, TexCoords));

	float dist = length(i_light.position - i_fragPos);
	float attenuation = 1.0/(i_light.constant + i_light.linear * dist + i_light.quadratic * dist * dist);

    return (ambient + diffuse + specular) * attenuation;
}

vec3 CalSpotLight(SpotLight i_light, vec3 i_normal, vec3 i_fragPos, vec3 i_viewDir)
{
	vec3 lightDir = normalize(i_light.position - i_fragPos);
	
	// 环境光
    vec3 ambient = i_light.ambient * vec3(texture(material.diffuse, TexCoords));

    // 漫反射 
    float diff = max(dot(i_normal, lightDir), 0.0);//随角度变化递减
    vec3 diffuse = i_light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

    // 镜面光
    vec3  reflectDir = reflect(-lightDir, i_normal); //用入射光和法线计算反射光 
    float spec = pow(max(dot(i_viewDir, reflectDir), 0.0), material.shininess);
    vec3  specular = i_light.specular * spec * vec3(texture(material.specular, TexCoords));

	//计算衰减
	float dist = length(i_light.position - i_fragPos);
	float attenuation = 1.0/(i_light.constant + i_light.linear * dist + i_light.quadratic * dist * dist);

	//计算片段被照亮的程度
	float theta = dot(lightDir, normalize(-i_light.direction)); 
    float epsilon = i_light.cutOff - i_light.outerCutOff;
    float intensity = clamp((theta - i_light.outerCutOff) / epsilon, 0.0, 1.0);

    return (ambient + diffuse + specular) * attenuation * intensity;
}