#version 330 core

out vec4 FragColor;
in vec4 oColor;

void main()
{
	FragColor = vec4(oColor);
	//FragColor = vec4(1.0,1.0,1.0,1.0);
}