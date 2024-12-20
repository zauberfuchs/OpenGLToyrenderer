#version 450 core
#include math.glsl
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

///////////////////////////////////////////////////////////////////////////////
// Input uniform variables
///////////////////////////////////////////////////////////////////////////////
uniform mat4 model;

struct Light {
    vec3 position;
    vec3 color;
};

layout (std140, binding = 1) uniform ForwardConstants
{
    mat4 view;
    mat4 projection;
	vec3 cameraPos;
	float farPlane;
	float nearPlane;
	Light light[];
};

void main() 
{
	fragPos = vec3(model * vec4(vPos, 1.0));

	normal = mat3(transpose(inverse(model))) * vNormal; 
	
	// Assigns the texture coordinates from the Vertex Data to "texCoord"
	texCoord = vTex;

	gl_Position = projection * view * vec4(fragPos, 1.0);
}
