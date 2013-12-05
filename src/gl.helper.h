#include <vector>
#include <string>
#include <GL/glew.h>

GLuint InitializeProgram(const char *vertexShaderPath, const char *fragmentShaderPath);
GLuint CreateShader(GLenum eShaderType, const std::string &strShaderFile);
GLuint CreateProgram(const std::vector<GLuint> &shaderList);
