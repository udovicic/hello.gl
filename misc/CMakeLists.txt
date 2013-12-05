# copy shaders
file(COPY
    ${CMAKE_SOURCE_DIR}/misc/vertex.shader
    ${CMAKE_SOURCE_DIR}/misc/fragment.shader
    DESTINATION ${CMAKE_BINARY_DIR}/bin/)
