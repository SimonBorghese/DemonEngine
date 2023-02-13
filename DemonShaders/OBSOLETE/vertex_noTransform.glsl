#version 330 core

// Note, we don't need anything but position and texture coord
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTextCord;

out vec2 textcord;

void main(){
    textcord = aTextCord;
    gl_Position = vec4(aPos, 1.0f) + vec4(0.0f, 0.0f, 1.0f, 0.0f);
}