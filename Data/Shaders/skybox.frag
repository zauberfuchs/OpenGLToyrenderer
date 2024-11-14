#version 450 core

// required by GLSL spec Sect 4.5.3 (though nvidia does not, amd does)
precision highp float;

///////////////////////////////////////////////////////////////////////////////
// Input varyings from vertex shader
///////////////////////////////////////////////////////////////////////////////
in vec3 TexCoords;
in vec3 WorldPos;

///////////////////////////////////////////////////////////////////////////////
// Output color
///////////////////////////////////////////////////////////////////////////////
layout(location = 0) out vec4 FragColor;

///////////////////////////////////////////////////////////////////////////////
// Skybox Texture
///////////////////////////////////////////////////////////////////////////////
layout(binding = 0) uniform samplerCube skybox;
layout(binding = 1) uniform sampler2D ditherSampler;
	

void main()
{

    vec3 envColor = textureLod(skybox, WorldPos, 0.0).rgb;
    
    //// HDR tonemap
    //envColor = envColor / (envColor + vec3(1.0));
	//// gamma correct
    //envColor = pow(envColor, vec3(1.0/2.2));
	vec4 color = vec4(envColor, 1.0);
    color += vec4(texture2D(ditherSampler, gl_FragCoord.xy / 8.0).r / 32.0 - (1.0 / 128.0));
    //FragColor = vec4(envColor, 1.0);
	FragColor = color;
}