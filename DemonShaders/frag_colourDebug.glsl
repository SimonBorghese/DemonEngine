#version 460 core

uniform sampler2D diffuse_texture;

in vec3 fragPos;
in vec2 textcord;

out vec4 FragColor;

void main(){
    //FragColor = vec4(fragPos, 1.0f);
    //FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    FragColor = texture(diffuse_texture, textcord);
}