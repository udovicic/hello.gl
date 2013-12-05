#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <GL/glew.h>
#include "common.h"
#include "gl.helper.h"

GLuint InitializeProgram(const char *vertexShaderPath, const char *fragmentShaderPath)
{
    std::vector<GLuint> shaderList;
    std::string strVertexShader, strFragmentShader;

    dout << "Initializing shaders" << std::endl;

    /* read source from file */
    std::ifstream VertexShaderStream(vertexShaderPath, std::ios::in);
    if(VertexShaderStream.is_open())
    {
        std::string Line = "";
        while(getline(VertexShaderStream, Line))
            strVertexShader += "\n" + Line;
        VertexShaderStream.close();
    }
    std::ifstream FragmentShaderStream(fragmentShaderPath, std::ios::in);
    if(FragmentShaderStream.is_open())
    {
        std::string Line = "";
        while(getline(FragmentShaderStream, Line))
            strFragmentShader += "\n" + Line;
        FragmentShaderStream.close();
    }

    dout << "Reading shaders completed" << std::endl;

    /* compile shaders */
    shaderList.push_back(CreateShader(GL_VERTEX_SHADER, strVertexShader));
    shaderList.push_back(CreateShader(GL_FRAGMENT_SHADER, strFragmentShader));

    /* link program */
    GLuint program = CreateProgram(shaderList);

    std::for_each(shaderList.begin(), shaderList.end(), glDeleteShader);

    return program;
}

GLuint CreateShader(GLenum eShaderType, const std::string &strShaderFile)
{
    dout << "Compiling shader" << std::endl;

    GLuint shader = glCreateShader(eShaderType);
    const char *strFileData = strShaderFile.c_str();
    glShaderSource(shader, 1, &strFileData, NULL);

    glCompileShader(shader);

    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        GLint infoLogLen;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLen);

        GLchar *strInfoLog = new GLchar[infoLogLen +1];
        glGetShaderInfoLog(shader, infoLogLen, NULL, strInfoLog);

       std::string strShaderType;
        switch(eShaderType)
        {
            case GL_VERTEX_SHADER: strShaderType = "vertex"; break;
            case GL_GEOMETRY_SHADER: strShaderType = "geometry"; break;
            case GL_FRAGMENT_SHADER: strShaderType = "fragment"; break;
        }

        std::string strErr;
        strErr = "Compile failure in " + strShaderType + " shader: " + strInfoLog;
        error_reporting(0, strErr.c_str());

        delete[] strInfoLog;
    }

    dout << "Compiling complete" << std::endl;
    return shader;
}

GLuint CreateProgram(const std::vector<GLuint> &shaderList)
{
    dout << "Linking shaders" << std::endl;
    GLuint program = glCreateProgram();

    for(size_t i=0; i< shaderList.size(); i++) {
        glAttachShader(program, shaderList[i]);
    }

    glLinkProgram(program);

    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        GLint infoLogLen;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLen);

        GLchar *strInfoLog = new GLchar[infoLogLen +1];
        glGetProgramInfoLog(program, infoLogLen, NULL, strInfoLog);

        std::string strErr;
        strErr = "Linker failure: ";
        strErr += strInfoLog;
        error_reporting(0, strErr.c_str());

        delete[] strInfoLog;
    }

    for(size_t i=0; i< shaderList.size(); i++) {
        glDetachShader(program, shaderList[i]);
    }

    dout << "Linking complete" << std::endl;
    return program;
}
