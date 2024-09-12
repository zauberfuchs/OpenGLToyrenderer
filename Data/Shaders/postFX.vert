#version 330 core

out vec2 TexCoords;

void main()
{
    vec2 uv			= vec2((gl_VertexID >> 1) & 1, gl_VertexID & 1);
    vec4 position	= vec4(uv * vec2(2.0, -2.0) + vec2(-1.0, 1.0), 0.0, 1.0);
	
	gl_Position = position;
    TexCoords = uv;
}