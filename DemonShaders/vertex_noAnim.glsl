#version 460 core

layout (location=0) in vec3 aPos;
//layout (location=1) in vec3 aNormal;
//layout (location=2) in vec2 aTextCord;

/*
out vec3 Normal;
out vec2 textcord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
*/

out vec3 fragPos;

void main(){
    fragPos = aPos;
    gl_Position = (vec4(aPos, 1.0f));

}