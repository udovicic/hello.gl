# compiler options
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

# target
ADD_EXECUTABLE(gl hello.gl.cpp gl.helper.cpp OBJLoader.cpp)
TARGET_LINK_LIBRARIES(gl glfw GLEW GL)
