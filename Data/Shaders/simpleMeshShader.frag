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
struct Material { // Material koeffizenten
    vec3 color;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};
uniform int hasTexture;
uniform int hasAmbient;
uniform int hasDiffuse;
uniform int hasSpecular;
uniform Material material;
uniform sampler2D u_Texture;

///////////////////////////////////////////////////////////////////////////////
// Light source Directional Light
///////////////////////////////////////////////////////////////////////////////
struct Light {
    vec3 position;
    vec3 color;
};
uniform Light light;

uniform vec3 lightPos; 
uniform vec3 lightColor; // Lichtintensitaet
uniform vec3 camPos;



void main() 
{
////////////////////// nicht Anfassen!! ////////////////////////////////////
     
    vec3 ambient = vec3(0.0f); 
    vec3 diffuse = vec3(0.0f);
    vec3 specular = vec3(0.0f);
    vec4 tex = vec4(1.0f);

    // texture
    if(hasTexture == 1)
    {
        tex = texture(u_Texture, texCoord);
    }

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Studenten Anfang - Aufgabe 4
/////////////////////////////////////////////////////////////////////////////////////////////////////////
	
    
    vec3 L = normalize(light.position - FragPos);
    vec3 N = normalize(Normal);
    vec3 V = normalize(camPos - FragPos);
    vec3 R = 2 * N * (dot(N, L)) - L;

    //--- Ambient ---///
    if(hasAmbient == 1)
    {
        ambient = light.color * material.ambient;
    }
    
  	
    //--- Diffuse ---///
    if(hasDiffuse == 1)
    {
        diffuse = light.color * material.diffuse * max(dot(L , N), 0.0f);
    }

    ///--- Specular ---///
    if(hasSpecular == 1)
    {
       specular = light.color * material.specular * pow(max(dot(R, V), 0.0f), material.shininess);
    }
    

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Studenten Ende
/////////////////////////////////////////////////////////////////////////////////////////////////////////

    vec3 result = (ambient + diffuse + specular) * material.color;

    FragColor = tex * vec4(result, 1.0);
}