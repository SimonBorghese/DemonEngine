#version 440 core

out vec4 FragColor;

in vec3 vertex;
in vec2 iTextCord;

layout (std140) uniform Textures{
    int _enableDiffuse;
    int _enableNormal;
};

uniform sampler2D _diffuse;
uniform sampler2D _normal;

void main(){
    float depthValue = texture(_diffuse, iTextCord).r;
    FragColor = vec4(vec3(depthValue), 1.0);
}