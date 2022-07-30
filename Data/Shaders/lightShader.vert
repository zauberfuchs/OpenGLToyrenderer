#version 330 core

///////////////////////////////////////////////////////////////////////////////
// Input vertex attributes
///////////////////////////////////////////////////////////////////////////////
layout (location = 0) in vec3 vPos;

///////////////////////////////////////////////////////////////////////////////
// Input uniform variables
///////////////////////////////////////////////////////////////////////////////
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;



void main()
{
	gl_Position = projection * view * model * vec4(vPos, 1.0);
}