#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <sstream>
#include <streambuf>
#include <string>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
std::string loadShaderSrc(const char* filename);

int main()
{
	std::cout << "Hello, World!" << std::endl;

	int success;
	char infolog[512];

	//glm Test
	//glm::vec4 vec(1.0f, 1.0f, 1.0f, 1.0f);
	//std::cout << vec.x << ", " << vec.y << ", " << vec.z << std::endl;		
	//glm::mat4 trans = glm::mat4(1.0f);							// Identity matrix.
	//trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));				//make translate matrix.
	//trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));			//make rotate matrix
	//trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));						//make scaling matrix.
	//vec = trans * vec;											// vector translate.
	//std::cout << vec.x << ", " << vec.y << ", " << vec.z << std::endl;

	glfwInit();

	// openGL version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Hello, Window!", NULL, NULL);
	if (window == NULL)	// window not created.
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, 800, 600);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	/*
		shaders
	*/

	// compile vertex shader
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	std::string vertShaderSrc = loadShaderSrc("assets/vertex_core.glsl");
	const GLchar* vertShader = vertShaderSrc.c_str();
	glShaderSource(vertexShader, 1, &vertShader, NULL);
	glCompileShader(vertexShader);

	//catch error
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infolog);
		std::cout << "error with vertex shader comp." << std::endl << infolog;
	}

	// compile fragment shader
	unsigned int fragmentShader[2];
	fragmentShader[0] = glCreateShader(GL_FRAGMENT_SHADER);
	std::string fragShaderSrc = loadShaderSrc("assets/fragment_core.glsl");
	const GLchar* fragShader = fragShaderSrc.c_str();
	glShaderSource(fragmentShader[0], 1, &fragShader, NULL);
	glCompileShader(fragmentShader[0]);

	//catch error
	glGetShaderiv(fragmentShader[0], GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader[0], 512, NULL, infolog);
		std::cout << "error with fragment shader comp." << std::endl << infolog;
	}

	fragmentShader[1] = glCreateShader(GL_FRAGMENT_SHADER);
	std::string fragShaderSrc1 = loadShaderSrc("assets/fragment_core2.glsl");
	const GLchar* fragShader1 = fragShaderSrc1.c_str();
	glShaderSource(fragmentShader[1], 1, &fragShader1, NULL);
	glCompileShader(fragmentShader[1]);

	//catch error
	glGetShaderiv(fragmentShader[1], GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader[1], 512, NULL, infolog);
		std::cout << "error with fragment shader comp." << std::endl << infolog;
	}

	// create program
	unsigned int shaderProgram[2];
	shaderProgram[0] = glCreateProgram();

	glAttachShader(shaderProgram[0], vertexShader);
	glAttachShader(shaderProgram[0], fragmentShader[0]);
	glLinkProgram(shaderProgram[0]);

	// catch errors
	glGetProgramiv(shaderProgram[0], GL_LINK_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shaderProgram[0], 512, NULL, infolog);
		std::cout << "Linking error : " << infolog << std::endl;
	}

	shaderProgram[1] = glCreateProgram();

	glAttachShader(shaderProgram[1], vertexShader);
	glAttachShader(shaderProgram[1], fragmentShader[1]);
	glLinkProgram(shaderProgram[1]);

	// catch errors
	glGetProgramiv(shaderProgram[1], GL_LINK_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shaderProgram[1], 512, NULL, infolog);
		std::cout << "Linking error : " << infolog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader[0]);
	glDeleteShader(fragmentShader[1]);

	// vertex array
	//float vertices[] = {				// 
	//	//1st triangle
	//	 0.5f,  0.5f, 0.0f,		//top right
	//	-0.5f,  0.5f, 0.0f,		//top left
	//	-0.5f, -0.5f, 0.0f,		//bottom left

	//	 //2nd triangle
	//	 -0.5f, -0.5f, 0.0f,	//bottom left
	//	  0.5f, -0.5f, 0.0f,	//bottom right
	//	  0.5f,  0.5f, 0.0f		//top right
	//};
	float vertices[] = {
		//1st triangle
		 0.5f,  0.5f, 0.0f,		//top right
		-0.5f,  0.5f, 0.0f,		//top left
		-0.5f, -0.5f, 0.0f,		//bottom left
		 0.5f, -0.5f, 0.0f		//bottom right
	};

	unsigned int indices[] = {
		0, 1, 2,				//first triangle,  tr -> tl -> bl
		2, 3, 0				//second triangle, bl -> br -> tr
	};
	// VAO, VBO
	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// bind VAO
	glBindVertexArray(VAO);

	// bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// set attribute pointer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// set up EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);

		// rendering commands here
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		 
		//draw shapes
		glBindVertexArray(VAO);

		//first shader.
		glUseProgram(shaderProgram[0]);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

		//2nd shader.
		glUseProgram(shaderProgram[1]);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(3*sizeof(unsigned int)));

		// send new frame to window.
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VAO);
	glDeleteBuffers(1, &EBO);


	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

std::string loadShaderSrc(const char* filename){
	std::ifstream  file;
	std::stringstream buf;

	std::string ret = "";

	file.open(filename);

	if (file.is_open()) {
		buf << file.rdbuf();
		ret = buf.str();
	}
	else {
		std::cout << "Could not open" << filename << std::endl;
	}

	file.close();

	return ret;
}