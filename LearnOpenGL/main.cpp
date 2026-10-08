#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

// C++ cannot open source file
// Refer to https://stackoverflow.com/questions/42679720/c-cannot-open-source-file
#include <shader_s.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
	// Configure GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif // __APPLE__

	// Window creation
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // Gets called each the window is resized

	// Glad: Load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialized GLAD" << std::endl;
		return -1;
	}

	// Let CMake set the "Exclude From Build" option for a single source file in Visual Studio
	// Refer to https://stackoverflow.com/questions/51780689/let-cmake-set-the-exclude-from-build-option-for-a-single-source-file-in-visual
	Shader ourShader("4.1.texture.vert", "4.1.texture.frag");

	// VERTEX INPUT
	// ------------

	float vertices[] = {
		// positions        // colors           // texture coords
		0.5f,  0.5f, 0.0f,	1.0f, 0.0f, 0.0f,	1.0f, 1.0f, // top right
		0.5f, -0.5f, 0.0f,	0.0f, 1.0f, 0.0f,	1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,	0.0f, 0.0f, 1.0f,	0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,	1.0f, 1.0f, 0.0f,	0.0f, 1.0f  // top left 
	};
	
	unsigned int indices[] = {
		0, 1, 3, // First triangle
		1, 2, 3 // Second triangle
	};

	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// NOTE (below):
	// "8 * sizeof(float)" means each vertex (along w/ other attributes connected to it) spans 8 floats 
	// in our vertices[] array

	// So, basically, in glVertexAttribPointer(), the "pointer" argument is the start of a specific attribute (i.e., vertex, color, texture, etc.)
	// and then "stride" is how many elements in-between until we reach the next data of the same attribute.

	// Position vertex attribute
	// -------------------------
	// "(void*)0" is called pointer. It's the offset of the first component of the first generic vertex attribute
	// Refer to https://registry.khronos.org/OpenGL-Refpages/gl4/html/glVertexAttribPointer.xhtml
	// In our scenario, below the the vertices data start at index 0 in vertices[].
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0); // Should match with the corresponding input in the vertex shader (location)

	// Color vertex attribute
	// ----------------------
	// "(void*)(3 * sizeof(float)" is the start of color data at index 3 in vertices[]
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Texture coord vertex attribute
	// ------------------------------
	// "(void*)(6 * sizeof(float))" is the start of texture data at index 6 in vertices[]
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// TEXTURE BINDING
	// ---------------
	unsigned int texture1, texture2;

	// Texture 1
	// ---------
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1); // All upcoming operations now have effect on our texture object (Basically, bind first before applying)

	// Texture wrapping
	// ----------------
	// The equivalent of (x, y, z) in texture coordinates is called (s, t, r) (refer to https://open.gl/textures)
	// S and T just mean U and V (or X and Y if you prefer), or in GLSL:
	// vec4.xyzw == vec4.rgba == vec4.strq
	// Refer to https://gamedev.stackexchange.com/questions/62548/what-does-changing-gl-texture-wrap-s-t-do
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Texture filtering parameters (when scaling up or downwards)
	// -----------------------------------------------------------
	// "GL_TEXTURE_MIN_FILTER" is the texture minifying function. There are six defined minifiying functions
	// (i.e., GL_NEAREST, GL_LINEAR, etc.)
	// Refer to https://registry.khronos.org/OpenGL-Refpages/gl4/html/glTexParameter.xhtml
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Texture magnification function (2 magnification function)

	// Load image, create texture, and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // Flip loaded texture on the y-axis
	unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		// Once glTexImage2D() is called, the currently bound texture now has the texture image attached to it.
		glGenerateMipmap(GL_TEXTURE_2D); // Automatically generate all the required mipmaps for the currently bound texture
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	// Free the image memory after
	stbi_image_free(data);
	
	// Texture 2
	// ---------

	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data);

	ourShader.use(); // Don't forget to activate the shader before setting uniforms
	// Tell OpenGL which texture unit each shader sampler belongs to
	glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0); // Set it manually
	ourShader.setInt("texture2", 1);

	// Render loop (each iteration is a frame)
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		// Clear the screen with a dark green-blueish color
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		
		// Render container
		ourShader.use();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window); // Double-buffer technique
		glfwPollEvents(); // Checks if any events are triggered
	}

	// Optional: De-allocate all resources
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window)
{
	// Window will close when you press 'Esc' key
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// Tell OpenGL the size of the rendering window so it knows how we want to
	// display the data and coordinates with respect to the window.
	glViewport(0, 0, width, height);
}
