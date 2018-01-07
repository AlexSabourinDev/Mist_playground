#version 430

out vec4 outFragColor;

in vec4 interpolatorVertPos;

void main()
{
	outFragColor = interpolatorVertPos * 10.0 * 0.5 + 0.5;
}