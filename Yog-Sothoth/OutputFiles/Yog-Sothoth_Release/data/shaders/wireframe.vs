#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;
uniform vec3 objectColor;

out vec3 oColor;

void main()
{
	oColor = objectColor;
	vec3 wirePos = aPos * 1.001; 
	gl_Position = projection * view * model * vec4(wirePos, 1.0f);
}
