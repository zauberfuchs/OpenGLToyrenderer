#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform int sampleSize;
uniform sampler2DMS screenTexture;

void main()
{ 
    vec4 colorSample;
    ivec2 texturePosition = ivec2(gl_FragCoord.x, gl_FragCoord.y);
    for(int i = 0; i < sampleSize; i++)
    {
        colorSample += texelFetch(screenTexture, texturePosition, i);
    }
    FragColor = colorSample / sampleSize;
}