#version 330 core

uniform sampler2D diffuse_texture;

in vec2 textcord;

out vec4 FragColor;

void main(){
    FragColor = texture(diffuse_texture, textcord);
}