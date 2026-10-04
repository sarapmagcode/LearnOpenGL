#version 330 core
out vec4 FragColor;

//in vec3 ourColor;
in vec3 ourPosition;

void main() 
{
    FragColor = vec4(ourPosition, 1.0);
}

// The bottom-left is black because the position is set to (-0.5f, -0.5f, 0.0f) and 
// in here, the xy values are negative so they are clamped to a value of 0.0f.
// The RGB would be (0, 0, 0) which is black.
