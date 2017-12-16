#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 WaterColor;

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
    
    float max = A1 + A2;
    float min = -A1 - A2;
    float normalized = ((h + z) - min) / (max - min);
	
	WaterColor = vec3(pow(normalized, 15.0), pow(normalized, 10.0), clamp(normalized, 1.0, 0.3));
	//WaterColor = vec3(0,0,0);
	
    vec3 FragPos = vec3(model * pos);

    gl_Position = projection * view * vec4(FragPos, 1.0);
}
