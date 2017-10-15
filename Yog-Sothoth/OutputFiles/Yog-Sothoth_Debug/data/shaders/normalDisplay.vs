#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out Vertex
{
  vec4 normal;
  vec4 color;
} vertex;

void main()
{
  gl_Position = vec4(aPos, 1.0);
  vertex.normal = vec4(aNormal, 1.0);
  vertex.color =  vec4(1.0, 1.0, 0.0, 1.0);
}
