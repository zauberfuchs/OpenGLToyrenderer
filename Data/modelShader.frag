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

///////////////////////////////////////////////////////////////////////////////
// Material
///////////////////////////////////////////////////////////////////////////////
struct Material {
    sampler2D diffuseMap1;
    sampler2D specularMap1;
    float shininess;    
}; 
uniform vec3 objectColor;
uniform Material material;

///////////////////////////////////////////////////////////////////////////////
// Light source
///////////////////////////////////////////////////////////////////////////////
struct Light {
    vec3 lightPos;
    vec3 ambient;
    vec3 specular;
    vec3 diffuse;
};
uniform vec3 camPos;
uniform Light light;
  


void main() 
{
    vec3 ambient = vec3(0.0f);
    vec3 diffuse = vec3(0.0f);
    vec3 specular = vec3(0.0f);

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.lightPos - FragPos);


	// ambient
    ambient = light.ambient * texture(material.diffuseMap1, texCoord).rgb;
    
  	
    // diffuse
    float diff = max(dot(norm, lightDir), 0.0);
    diffuse = light.diffuse * diff * texture(material.diffuseMap1, texCoord).rgb;
    

    // specular
    float specularStrength = 0.2f;
    vec3 viewDirection = normalize(camPos - FragPos);
    vec3 reflectionDirection = reflect(-lightDir, norm);
    float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), material.shininess);
    specular = light.specular * specAmount * texture(material.specularMap1, texCoord).rgb;

    // Check if we got invalid results in the operations
	if(any(isnan(FragColor)))
	{
		FragColor.xyz = vec3(1.f, 0.f, 1.f);
	}

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}