#version 330 core
out vec4 FragColor;

in vec4 vertexPos;

void main() 
{
    FragColor = vertexPos;
}
