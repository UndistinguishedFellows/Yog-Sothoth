#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 WaterColor;
out vec2 uv;

uniform sampler2D perlin;
uniform sampler2D caustics;
uniform sampler2D caustics2;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float offset1;
uniform float Hz1;
uniform float A1;
uniform float offset2;
uniform float Hz2;
uniform float A2;


void main()
{
	float h = A1*sin(Hz1*aPos.x+offset1);
    float z = A2*cos(Hz2*aPos.z+(offset2)/(2));
	vec4 pos = vec4(aPos, 1);
	pos.y = h + z;
	uv = aTexCoord;
	float height1 = texture(caustics, vec2(uv.x + offset1/20, uv.y)).r * 0.06;
	float height2 = texture(caustics2, vec2(uv.x + offset2/100, uv.y - offset2/7)).r * 0.03;
	float height3 = texture(perlin, uv - offset1/40).r * 0.004;
	pos.y += height1 + height2 + height3;
	
    
    float max = A1 + A2;
    float min = -A1 - A2;
    float normalized = ((h + z) - min) / (max - min);

    normalized = height1/0.10 + height2/0.10 + height3/0.04;
	
	//WaterColor = vec3(pow(normalized, 15.0), pow(normalized, 10.0), clamp(normalized, 1.0, 0.03));
	//WaterColor = vec3(pow(normalized, 2.0), pow(normalized, 2.0), normalized);
    WaterColor = vec3(normalized/4, normalized/1.3, normalized/0.8);
	//WaterColor = vec3(0,0,0);
	
	
    vec3 FragPos = vec3(model * pos);

    gl_Position = projection * view * vec4(FragPos, 1.0);
}
