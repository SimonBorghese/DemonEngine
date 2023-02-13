#version 430 core

layout (location = 0) in vec3 aPos;


layout (std140) uniform Transform{
    mat4 view;
    mat4 projection;
};


out vec4 vertex;

void main(){
    gl_PointSize = 20.0f;
    vertex = vec4(aPos, 1.0f);
    gl_Position = (projection * view * vec4(aPos, 1.0f));
}