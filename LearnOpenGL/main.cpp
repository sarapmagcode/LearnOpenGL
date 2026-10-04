#include <glad/glad.h>
#include <GLFW/glfw3.h>

// C++ cannot open source file
// Refer to https://stackoverflow.com/questions/42679720/c-cannot-open-source-file
#include <LearnOpenGl/shader_s.h>

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
	Shader ourShader("3.3.shader.vert", "3.3.shader.frag");

	// VERTEX INPUT
	// ------------

	float vertices[] = {
		// positions		// colors
		-0.5f, -0.5f, 0.0f,	1.0f, 0.0f, 0.0f, // left  
		0.5f, -0.5f, 0.0f,	0.0f, 1.0f, 0.0f, // right 
		0.0f,  0.5f, 0.0f,	0.0f, 0.0f, 1.0f // top   
	};

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// Position vertex attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// Color vertex attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Render loop (each iteration is a frame)
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		// Clear the screen with a dark green-blueish color
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		float offset = 0.5f;
		ourShader.use();
		ourShader.setFloat("xOffset", offset);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window); // Double-buffer technique
		glfwPollEvents(); // Checks if any events are triggered
	}

	// Optional: De-allocate all resources
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

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
