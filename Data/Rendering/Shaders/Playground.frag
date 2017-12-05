#version 330

out vec4 outFragColor;

void main()
{
	vec2 uv = gl_FragCoord.xy / 400.0;
	outFragColor = vec4(uv.x, uv.y, 1.0, 1.0);
}