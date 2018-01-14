#version 430

out vec4 outFragColor;

in vec4 interpNorm;
in vec2 interpUV;

void main()
{
	vec3 lightDir = vec3(0.707, 0.707, 0.0);
	
	vec2 repeat = mod(interpUV, vec2(0.05)) / 0.05;
	vec2 grid = smoothstep(vec2(0.02), vec2(0.05), 1.0 - abs(repeat - vec2(0.5)) / 0.5);
	float amount = max(grid.x * grid.y, 0.3);
	
	outFragColor = vec4(vec3(amount), 1.0) * max(dot(lightDir, normalize(interpNorm.xyz)), 0.3);
}