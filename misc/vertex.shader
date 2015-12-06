#version 400

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 normal;

const vec3 lightPosition = vec3(6, 4, -7); // Light positio

uniform mat4 MVP;

out vec3 vNormal;
out vec3 vLight;

void main(){
    // Transform vertex normal
    vNormal = (MVP * vec4(normal, 1.0)).xyz;
    vLight = lightPosition;

    // transform vertex position
    gl_Position =  MVP * vec4(vertex, 1.0);
}
