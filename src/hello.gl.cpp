#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <stdlib.h>
#include "gl.helper.h"
#include "common.h"
#include "OBJLoader.hpp"

GLFWwindow* window;

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

    // Dark blue background
    glClearColor(0.93f, 0.93f, 0.98f, 1.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

    // Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

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
	window = GL_init();

	/* load shaders */
	dout << "Fragment shader: " << strFragmentPath << std::endl;
	dout << "Vertex shader: " << strVertexPath << std::endl;
	GLuint shaderProgram = InitializeProgram(strVertexPath, strFragmentPath);
	glUseProgram(shaderProgram);
	dout << "Loading shader program complete" << std::endl;

    // Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(shaderProgram, "MVP");

    // Get a handle for our buffers
	GLuint vertexPosition_modelspaceID = glGetAttribLocation(shaderProgram, "vertex");
    GLuint vertexNormalID = glGetAttribLocation(shaderProgram, "normal");

    // Read our .obj file
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	loadOBJ("monkey.obj", vertices, normals);

    // Load it into a VBO

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

    GLuint normalbuffer;
	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

    // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 1.0f, 1000.0f);

    // Camera matrix
    glm::mat4 View = glm::lookAt(
        glm::vec3(0, 1, 3), // Camera is at (4,3,3), in World Space
        glm::vec3(0,0,0), // and looks at the origin
        glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
    );

    // Model matrix : an identity matrix (model will be at the origin)
    glm::mat4 Model = glm::mat4(1.0f);
    // Our ModelViewProjection : multiplication of our 3 matrices
    glm::mat4 MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around

	/* loop until window closed */
	dout << "Entering main loop" << std::endl;
	while (!glfwWindowShouldClose(window)) {
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Send our transformation to the currently bound shader,
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

        // 1rst attribute buffer : vertices
		glEnableVertexAttribArray(vertexPosition_modelspaceID);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			vertexPosition_modelspaceID,  // The attribute we want to configure
			3,                            // size
			GL_FLOAT,                     // type
			GL_FALSE,                     // normalized?
			0,                            // stride
			(void*)0                      // array buffer offset
		);

        glEnableVertexAttribArray(vertexNormalID);
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
		glVertexAttribPointer(
			vertexNormalID,               // The attribute we want to configure
			3,                            // size
			GL_FLOAT,                     // type
			GL_FALSE,                     // normalized?
			0,                            // stride
			(void*)0                      // array buffer offset
		);

        // Draw the triangles !
		glDrawArrays(GL_TRIANGLES, 0, vertices.size() );

		glDisableVertexAttribArray(vertexPosition_modelspaceID);
        glDisableVertexAttribArray(vertexNormalID);

		/* swap buffers */
		glfwSwapBuffers(window);

		/* poll events */
		glfwPollEvents();
	}

	/* terminate application */
	dout << "Loop ended, closing" << std::endl;
    // Cleanup VBO and shader
	glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &normalbuffer);
	glDeleteProgram(shaderProgram);
	glfwDestroyWindow(window);
	glfwTerminate();

	exit(EXIT_SUCCESS);
}
