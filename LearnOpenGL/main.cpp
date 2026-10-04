#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char* vertexShaderSource = "#version 330 core\n" // '330' means OpenGL version 3.3 (if we're using 4.2, then it would become '420' instead)
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";

const char* fragmentShaderSources[2] = {
	// Orange
	"#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\n\0",

	// Yellow
	"#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"	FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
	"}\n\0"
};

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

	// VERTEX SHADER
	// -------------

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	int success;
	char infoLog[512];
	
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// FRAGMENT SHADERS
	// ----------------

	unsigned int fragmentShaders[2] = { 
		glCreateShader(GL_FRAGMENT_SHADER),
		glCreateShader(GL_FRAGMENT_SHADER)
	};

	int fragmentShadersLength = sizeof(fragmentShaders) / sizeof(fragmentShaders[0]);

	for (int i = 0; i < fragmentShadersLength; i++)
	{
		glShaderSource(fragmentShaders[i], 1, &fragmentShaderSources[i], NULL);
		glCompileShader(fragmentShaders[i]);

		glGetShaderiv(fragmentShaders[i], GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentShaders[i], 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT" << "[" << i << "]" << "::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
	}

	// SHADER PROGRAMS
	// ---------------

	unsigned int shaderPrograms[2] = { 
		glCreateProgram(), 
		glCreateProgram() 
	};

	for (int i = 0; i < fragmentShadersLength; i++)
	{
		glAttachShader(shaderPrograms[i], vertexShader);
		glAttachShader(shaderPrograms[i], fragmentShaders[i]);
		glLinkProgram(shaderPrograms[i]);

		glGetProgramiv(shaderPrograms[i], GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shaderPrograms[i], 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM" << "[" << i << "]" << "::LINKING_FAILED\n" << infoLog << std::endl;
		}

		glDeleteShader(fragmentShaders[i]);
	}

	// Delete the common vertex shader after attaching on the 
	// two shader programs
	glDeleteShader(vertexShader);

	// VERTEX INPUT
	// ------------

	float triangles[2][9] = {
		// First triangle
		{
			-0.9f, -0.5f, 0.0f, // left  
			0.0f, -0.5f, 0.0f, // right 
			-0.45f,  0.5f, 0.0f, // top
		},
		// Second triangle
		{
			0.0f, -0.5f, 0.0f, // left
			0.9f, -0.5f, 0.0f, // right
			0.45f, 0.5f, 0.0f, // top
		}
	};

	unsigned int VBOs[2], VAOs[2];
	glGenVertexArrays(2, VAOs);
	glGenBuffers(2, VBOs);

	int buffersLength = sizeof(VBOs) / sizeof(VBOs[0]);
	for (int i = 0; i < buffersLength; i++)
	{
		glBindVertexArray(VAOs[i]);
		glBindBuffer(GL_ARRAY_BUFFER, VBOs[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(triangles[i]), triangles[i], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
	}

	// Render loop (each iteration is a frame)
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		// Clear the screen with a dark green-blueish color
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw two triangles (with different colors)
		for (int i = 0; i < buffersLength; i++)
		{
			glUseProgram(shaderPrograms[i]);
			glBindVertexArray(VAOs[i]);
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}

		glfwSwapBuffers(window); // Double-buffer technique
		glfwPollEvents(); // Checks if any events are triggered
	}

	glDeleteVertexArrays(2, VAOs);
	glDeleteBuffers(2, VBOs);

	for (int i = 0; i < fragmentShadersLength; i++)
	{
		glDeleteProgram(shaderPrograms[i]);
	}

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
