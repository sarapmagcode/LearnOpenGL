#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// The fragment shader should also have access to the texture object.
// GLSL has a built-in data-type for texture objects called a sampler.
uniform sampler2D ourTexture;

void main()
{
	// Default built-in output variable for fragment shaders, responsible for assigning
	// the final color to a processed pixel. 
	// Refer to https://salivity.github.io/glsl/article/why-was-gl-frag-color-deprecated-in-glsl
	FragColor = texture(ourTexture, TexCoord);
}
