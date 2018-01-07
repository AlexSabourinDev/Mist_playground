#version 430

out vec4 outFragColor;

in vec4 interpWorldPos;
in vec4 interpLocalPos;

void main()
{
	vec3 lightPos = vec3(-200.0, 100.0, -300.0);
	vec3 lightDir = normalize(lightPos - interpWorldPos.xyz);
	
	outFragColor = normalize(interpLocalPos) * max(dot(lightDir, normalize(interpWorldPos.xyz)), 0.3);
}