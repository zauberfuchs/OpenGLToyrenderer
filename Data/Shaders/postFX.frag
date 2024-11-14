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
	//// HDR tonemapping
	//colorSample.rgb = colorSample.rgb / (colorSample.rgb + vec3(1.0));
	//// gamma correct
	//
 //   FragColor = colorSample / sampleSize;
	
	colorSample.rgb = (colorSample.rgb * (2.51f * colorSample.rgb + 0.03f)) / (colorSample.rgb * (2.43f * colorSample.rgb + 0.59f) + 0.14f);
	colorSample.rgb = pow(colorSample.rgb, vec3(1.0/2.2)); 
	//
    FragColor = clamp(colorSample, 0.0f, 1.0f) / sampleSize;
}


//vec3 col = tex2d(maintex, i.uv).rgb;
//vec3 cout = (col*(2.51f * col + 0.03f)) / (col*(2.43f * col + 0.59f) + 0.14f);
//
//return vec4(saturate(cout), 1.0f);
//
//precision highp float;
//uniform highp sampler2D texture1;
//uniform float         arraySlice;
//uniform float         mipLevel;
//uniform int           isHdr;
//uniform vec4          colorScale;
//uniform vec4          colorOffset;
//in vec4               varColor;
//in vec4               varTexcoord;
//out vec4 fragColor;
//
//void main()
//{
//    vec4 texcoord = varTexcoord;
//    
//    vec4 color = vec4(0, 0, 0, 1);
//    color = vec4(textureLod(texture1, texcoord.xy , mipLevel));
//    if (isHdr != 0) {
//        const float gamma = 2.2f;
//        vec3 v = color.rgb / (1.f + color.rgb);
//        v = pow(v, vec3(1.f/gamma));
//        color.rgb = v;
//    } else {
//        color = vec4(colorOffset + colorScale * color);
//    }
//    fragColor = vec4(color.rgba * varColor);
//}
