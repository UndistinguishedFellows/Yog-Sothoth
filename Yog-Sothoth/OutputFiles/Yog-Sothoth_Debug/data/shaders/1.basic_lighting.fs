#version 330 core
out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;  
in vec2 TexCoords;
  
uniform vec3 lightPos; 
uniform vec3 viewPos; 
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform sampler2D tex;
uniform int hasTex; // 0 if no texture 1 if texture

void main()
{
    // ambient
    float ambientStrength = 0.1;
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);

    vec3 texResult = texture(tex, TexCoords).rgb;
        
	if(hasTex == 0)
	{
		texResult = vec3(1, 1, 1);
	}

	vec3 ambient = ambientStrength * lightColor * texResult;  	
	vec3 diffuse = diff * lightColor * texResult;    
    vec3 specular = specularStrength * spec * lightColor * texResult;   

    vec3 result = (ambient + diffuse + specular) * objectColor;

    FragColor = vec4(result, 1.0);
}
