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
    vec4 outputColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    if (_enableDiffuse == 1){
        outputColor = texture(_diffuse, iTextCord);
    } else{
        outputColor = vec4(normalize(abs(-vertex)), 1.0f)  * vec4(2.0f, 2.0f, 2.0f, 1.0f);
    }
    FragColor = outputColor;
}