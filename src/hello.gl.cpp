#include <iostream>
#include <GLFW/glfw3.h>

int main(int narg, char** argc)
{
	GLFWwindow *window;

	/* library init */
	if (!glfwInit()) {
		return -1;
	}

	/* create window */
	int windowWidth = 640;
	int windowHeight = 480;
	window = glfwCreateWindow(windowWidth, windowHeight, "Hello OpenGL", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	/* Create OpenGL context */
	glfwMakeContextCurrent(window);

	/* loop until window closed */
	while (!glfwWindowShouldClose(window)) {
		/* rendering code */

		/* swap buffers */
		glfwSwapBuffers(window);

		/* poll events */
		glfwPollEvents();
	}

	/* terminate application */
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}