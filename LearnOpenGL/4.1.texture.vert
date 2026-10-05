#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;

void main() 
{
	// Mandatory built-in output variable in the vertex shader.
	// Tells the GPU where that vertex belongs on the screen.
	// Refer to https://salivity.github.io/glsl/article/what-does-gl-position-do-in-glsl-vertex-shaders
	gl_Position = vec4(aPos, 1.0);
	ourColor = aColor;
	TexCoord = aTexCoord;
}
