#version 460 core

layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aNormal;
layout (location=2) in vec2 aTextCord;

flat out vec2 textcord;
out vec3 iNormal;
out vec3 iFragPos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;


void main(){
    textcord = aTextCord;
    iNormal = mat3(transpose(inverse(model))) * aNormal;
    iFragPos = vec3(model * vec4(aPos, 1.0f));
    gl_Position = (projection * view * model * vec4(aPos, 1.0f));

}