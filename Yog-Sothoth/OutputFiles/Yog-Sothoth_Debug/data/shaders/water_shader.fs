#version 330 core
out vec4 FragColor;

in vec3 WaterColor;

void main()
{
    FragColor = vec4(WaterColor, 1.0);
}
