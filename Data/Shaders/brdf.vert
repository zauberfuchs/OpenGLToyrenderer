#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

//void main()
//{
//    TexCoords = aTexCoords;
//	gl_Position = vec4(aPos, 1.0);
//}

void main()
{
    vec2 uv			= vec2((gl_VertexID >> 1) & 1, 1 - gl_VertexID & 1);
    vec4 position	= vec4(uv * vec2(2.0, 2.0) - vec2(1.0, 1.0), 0.0, 1.0);
	
    TexCoords = uv;
	gl_Position = position;
}