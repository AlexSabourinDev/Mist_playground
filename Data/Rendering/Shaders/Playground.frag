#version 330

out vec4 outFragColor;

in vec4 interpolatorVertPos;

void main()
{
	vec2 uv = interpolatorVertPos.xy / 2.0 + 0.5;
	outFragColor = vec4(uv.x, uv.y, 1.0, 1.0);
}