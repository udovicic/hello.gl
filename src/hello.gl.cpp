#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdlib.h>
#include "gl.helper.h"
#include "common.h"


void error_reporting(int error, const char* description)
{
    std::cerr << "EE :: " << description << std::endl;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	dout << "Key callback :: key: " << key << "; action: " << action << ";" << std::endl;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

GLFWwindow* GL_init()
{
	GLFWwindow *window;

	glfwSetErrorCallback(error_reporting);

	/* library init */
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}
	dout << "Glfw init passed" << std::endl;

	/* create window */
	int windowWidth = 640;
	int windowHeight = 480;
	window = glfwCreateWindow(windowWidth, windowHeight, "Hello OpenGL", NULL, NULL);
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	dout << "GLFW window created" << std::endl;

	/* Create OpenGL context */
	glfwMakeContextCurrent(window);
	dout << "GLFW current context" << std::endl;

	/* Handle keyboard callback */
	glfwSetKeyCallback(window, key_callback);

	/* GLEW init */ 
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		error_reporting(1, (char*)glewGetErrorString(err));
	}
	dout << "Glew init" << std::endl;

	return window;
}

int main(int narg, char** argc)
{
	const char *strVertexPath = "vertex.shader", *strFragmentPath = "fragment.shader";

	/* read program parameters from command line */
	char *program = argc[0];

	while ((narg>1) && (argc[1][0]=='-')) {
		switch(argc[1][1]) {
			case 'f': strFragmentPath=&argc[1][3]; break;
			case 'v': strVertexPath=&argc[1][3]; break;
			default:
				std::cout << "Usage: " << program << " <options>" << std::endl
					<< "\t-v <file_name>\tVertex shader path" << std::endl
	                << "\t-f <file_name>\tFragment shader path" << std::endl;
				exit(EXIT_SUCCESS);
				break;
	        }
	        
		narg--;
		argc++;
	}

	/* initialize GL context and window */
	GLFWwindow *window;
	window = GL_init();

	/* load shaders */
	dout << "Fragment shader: " << strFragmentPath << std::endl;
	dout << "Vertex shader: " << strVertexPath << std::endl;
	GLuint shaderProgram = InitializeProgram(strVertexPath, strFragmentPath);
	glUseProgram(shaderProgram);
	dout << "Loading shader program complete" << std::endl;

	/* loop until window closed */
	dout << "Entering main loop" << std::endl;
	while (!glfwWindowShouldClose(window)) {
		/* rendering code */
		
		/* swap buffers */
		glfwSwapBuffers(window);

		/* poll events */
		glfwPollEvents();
	}

	/* terminate application */
	dout << "Loop ended, closing" << std::endl;
	glfwDestroyWindow(window);
	glfwTerminate();

	exit(EXIT_SUCCESS);
}
