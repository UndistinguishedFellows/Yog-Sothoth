#version 330 core
out vec4 FragColor;

in vec3 WaterColor;
uniform sampler2D perlin;
in vec2 uv;

void main()
{
    FragColor = vec4(WaterColor, 1.0);
	//vec3 texResult = texture(perlin, uv).rgb;
	//FragColor = vec4(texResult, 1.0);
	//FragColor = vec4(0,0, texResult.b, 1.0);
}
