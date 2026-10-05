#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// The fragment shader should also have access to the texture object.
// GLSL has a built-in data-type for texture objects called a sampler.
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	// Default built-in output variable for fragment shaders, responsible for assigning
	// the final color to a processed pixel. 
	// Refer to https://salivity.github.io/glsl/article/why-was-gl-frag-color-deprecated-in-glsl
	FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
}
