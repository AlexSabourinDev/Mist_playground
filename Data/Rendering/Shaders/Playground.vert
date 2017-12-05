#version 330

layout (location = 0) in vec3 inVert;

void main()
{
	gl_Position = vec4(inVert, 1.0);
}