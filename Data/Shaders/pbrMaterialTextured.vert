#version 450 core

///////////////////////////////////////////////////////////////////////////////
// Input vertex attributes
///////////////////////////////////////////////////////////////////////////////
layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTex;

///////////////////////////////////////////////////////////////////////////////
// Output to fragment shader
///////////////////////////////////////////////////////////////////////////////
out vec3 fragPos;
out vec3 normal;
out vec2 texCoord;
out vec4 fragPosLightSpace;

///////////////////////////////////////////////////////////////////////////////
// Input uniform variables
///////////////////////////////////////////////////////////////////////////////
uniform mat4 model;
uniform mat4 lightSpaceMatrix;
// ändern
//uniform mat4 normalMatrix;
//uniform mat4 modelViewMatrix;
//uniform mat4 modelViewProjectionMatrix;

layout (std140, binding = 1) uniform ForwardConstants
{
    mat4 view;
    mat4 projection;
	vec3 cameraPos;
	float farPlane;
	float nearPlane;
};

void main() 
{
	fragPos = vec3(model * vec4(vPos, 1.0));

	normal = mat3(transpose(inverse(model))) * vNormal; 
	fragPosLightSpace = lightSpaceMatrix * vec4(fragPos, 1.0);
	// Assigns the texture coordinates from the Vertex Data to "texCoord"
	texCoord = vTex;

	gl_Position = projection * view * vec4(fragPos, 1.0);
}
