#version 460 core

layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aNormal;
layout (location=2) in vec2 aTextCord;

/*
uniform sampler2D diffuse_texture;

// Light stuff
uniform vec3 ambient;
uniform vec3 lightPos;
uniform vec3 viewPos;

// vec3 iLightPos;
in vec3 iFragPos;
in vec3 iNormal;
in vec2 textcord;

out vec4 FragColor;

void main(){
    vec3 norm = normalize(iNormal);
    vec3 lightDir = normalize(lightPos - iFragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * ambient;

    vec3 viewDir = normalize(viewPos - iFragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float specStrength = 0.5;
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specStrength * spec * ambient;


    vec3 _ambient = ambient;
    FragColor = vec4(ambient+diffuse+specular, 1.0f) * texture(diffuse_texture, textcord);
}
*/
out vec3 iFragPos;
out vec3 iNormal;
flat out vec2 textcord;
out vec4 outputColour;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform vec3 ambient;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main(){
    textcord = aTextCord;
    iNormal = mat3(transpose(inverse(model))) * aNormal;
    iFragPos = aPos;
    // Light
    vec3 norm = normalize(aNormal);
        vec3 lightDir = normalize(lightPos - aPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * ambient;

        vec3 viewDir = normalize(viewPos - aPos);
        vec3 reflectDir = reflect(-lightDir, norm);

        float specStrength = 0.9;
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
        vec3 specular = specStrength * spec * ambient;


        vec3 _ambient = ambient;
        outputColour = vec4(ambient+diffuse+specular, 1.0f);

    gl_Position = (projection * view * model * vec4(aPos, 1.0f));

}