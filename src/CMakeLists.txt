# compiler options
add_definitions(-Wall)

# target
ADD_EXECUTABLE(gl hello.gl.cpp)
TARGET_LINK_LIBRARIES(gl glfw GLEW GL)
