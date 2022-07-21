#version 330 core

// required by GLSL spec Sect 4.5.3 (though nvidia does not, amd does)
precision highp float;

///////////////////////////////////////////////////////////////////////////////
// Input varyings from vertex shader
///////////////////////////////////////////////////////////////////////////////
in vec3 Normal;
in vec3 FragPos;
in vec2 texCoord;

///////////////////////////////////////////////////////////////////////////////
// Output color
///////////////////////////////////////////////////////////////////////////////
layout(location = 0) out vec4 FragColor;

uniform vec3 camPos;

///////////////////////////////////////////////////////////////////////////////
// Material
///////////////////////////////////////////////////////////////////////////////
struct Material {
    sampler2D diffuseMap1;
    sampler2D specularMap1;
    float shininess;
}; 
uniform Material material;
uniform vec3 objectColor;

///////////////////////////////////////////////////////////////////////////////
// Light source Directional Light
///////////////////////////////////////////////////////////////////////////////
struct DirLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirLight dirLight;

///////////////////////////////////////////////////////////////////////////////
// Light source Point Light
///////////////////////////////////////////////////////////////////////////////
#define NR_POINT_LIGHTS 1  
struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform PointLight pointLights[NR_POINT_LIGHTS];

///////////////////////////////////////////////////////////////////////////////
// Light source Spot Light
///////////////////////////////////////////////////////////////////////////////
struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};
uniform SpotLight spotLight;







///////////////////////////////////////////////////////////////////////////////
// Fuction declerations
///////////////////////////////////////////////////////////////////////////////
vec3 CalcDirectionalLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);



void main() 
{  
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(camPos - FragPos);
    vec3 result;
    // phase 1: directional lighting
    result = CalcDirectionalLight(dirLight, norm, viewDir);
    // phase 2: point lights
    for(int i = 0; i < NR_POINT_LIGHTS; i++) 
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
    // phase 3: spot light
    //result += CalcSpotLight(spotLight, norm, FragPos, viewDir);

    // Check if we got invalid results in the operations
	if(any(isnan(FragColor)))
	{
		FragColor.xyz = vec3(1.f, 0.f, 1.f);
	}

    FragColor = vec4(result, 1.0);
}

vec3 CalcDirectionalLight(DirLight light, vec3 normal, vec3 viewDirection)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectionDirection = reflect(-lightDir, normal);
    float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), material.shininess);
    // combine results
    vec3 ambient  = light.ambient;
    vec3 diffuse  = light.diffuse  * diff;
    vec3 specular = light.specular * specAmount;
    vec3 result;
    if(objectColor != vec3(0.0))
    {
       result = (ambient + diffuse + specular) * objectColor;
    }
    else
    {
        result = (ambient + diffuse + specular);
    }
    return result;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDirection)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectionDirection = reflect(-lightDir, normal);
    float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), material.shininess);
    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			            light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient  = light.ambient;
    vec3 diffuse  = light.diffuse  * diff;
    vec3 specular = light.specular * specAmount;
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    vec3 result;
    if(objectColor != vec3(0.0))
    {
       result = (ambient + diffuse + specular) * objectColor;
    }
    else
    {
        result = (ambient + diffuse + specular);
    }
    return result;
} 

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDirection)
{
     vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectionDirection = reflect(-lightDir, normal);
    float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), material.shininess);
    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			            light.quadratic * (distance * distance));     
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.ambient;
    vec3 diffuse = light.diffuse * diff;
    vec3 specular = light.specular * specAmount;
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}