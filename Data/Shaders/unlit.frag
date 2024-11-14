#version 330 core

// required by GLSL spec Sect 4.5.3 (though nvidia does not, amd does)
precision highp float;

///////////////////////////////////////////////////////////////////////////////
// Input varyings from vertex shader
///////////////////////////////////////////////////////////////////////////////
in vec3 fragPos;
in vec3 normal;
in vec2 texCoord;

///////////////////////////////////////////////////////////////////////////////
// Output color
///////////////////////////////////////////////////////////////////////////////
layout(location = 0) out vec4 FragColor;

///////////////////////////////////////////////////////////////////////////////
// Material
///////////////////////////////////////////////////////////////////////////////
struct Material { // Material koeffizenten
    sampler2D albedoMap;
    vec3 color;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};
uniform int hasTexture;
uniform Material material;
uniform sampler2D u_Texture;

///////////////////////////////////////////////////////////////////////////////
// Light source Directional Light
///////////////////////////////////////////////////////////////////////////////
struct Light {
    vec3 position;
    vec3 color;
};
uniform Light light[4];

uniform vec3 lightPos; 
uniform vec3 lightColor;
uniform vec3 camPos;


#define NUM_BANDS 100


void main() 
{
    //vec4 tex = vec4(1.0f);
//
    //// texture
    //if(hasTexture == 1)
    //{
    //    tex = texture(material.albedoMap, texCoord);
    //}
    //
    //vec3 L = normalize(light[0].position - FragPos);
    //vec3 N = normalize(Normal);
    //vec3 V = normalize(camPos - FragPos);
    //vec3 R = 2 * N * (dot(N, L)) - L;
//
    // // Calculate diffuse and specular terms
    //float diffuse = max(dot(N, light[0].position), 0.0);
    //float specular = pow(max(dot(N, L), 0.0), material.shininess);
//
    //// Quantize the lighting intensity into NUM_BANDS bands
    //float band = floor(diffuse * NUM_BANDS) / NUM_BANDS;
    //vec3 color = mix(material.ambient, material.diffuse, band) * light[0].color;
    //color += mix(vec3(0.0), material.specular, specular);

    //FragColor = vec4(color, 1.0);
   // FragColor = tex * vec4(result, 1.0);
	FragColor = vec4(1.0);
}