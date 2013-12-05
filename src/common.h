#include <GLFW/glfw3.h>
#include <iostream>

#ifdef DEBUG
#define dout std::cout << "DD :: "
#else
#define dout 0 && std::cout
#endif

void error_reporting(int error, const char* description);
