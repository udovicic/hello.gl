# copy shaders
file(COPY
    ${CMAKE_SOURCE_DIR}/misc/vertex.shader
    ${CMAKE_SOURCE_DIR}/misc/fragment.shader
    ${CMAKE_SOURCE_DIR}/misc/monkey.obj
    ${CMAKE_SOURCE_DIR}/misc/teapot.obj
    ${CMAKE_SOURCE_DIR}/misc/cube.obj
    DESTINATION ${CMAKE_BINARY_DIR}/bin/)
