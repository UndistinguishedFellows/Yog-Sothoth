#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;
uniform vec4 objectColor;

out vec4 oColor;

void main()
{
	oColor = objectColor;
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
}
