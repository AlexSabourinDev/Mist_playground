#version 430

layout (location = 4) uniform vec4 color;

out vec4 outFragColor;

in vec3 interpNorm;
in vec2 interpUV;

void main()
{
	vec3 lightDir = vec3(0.707, 0.707, 0.0);
	
	vec2 repeat = mod(interpUV, vec2(0.05)) / 0.05;
	vec2 grid = smoothstep(vec2(0.02), vec2(0.05), 1.0 - abs(repeat - vec2(0.5)) / 0.5);
	float amount = max(grid.x * grid.y, 0.3);
	
	outFragColor = color * amount * max(dot(lightDir, normalize(interpNorm)), 0.3);
}