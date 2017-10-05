#version 330 core

out vec3 FragColor;
in vec3 oColor;

void main()
{
	FragColor = vec3(oColor);
}
