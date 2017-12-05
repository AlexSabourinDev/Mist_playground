#version 330

layout (location = 0) in vec3 inVert;

out vec4 interpolatorVertPos;

void main()
{
	interpolatorVertPos = vec4(inVert, 1.0);
	gl_Position = vec4(inVert, 1.0);
}