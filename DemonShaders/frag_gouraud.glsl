#version 460 core

uniform sampler2D diffuse_texture;

in vec4 outputColour;
in vec2 textcord;

out vec4 FragColor;

void main(){
    FragColor = outputColour * texture(diffuse_texture, textcord);
}