#version 460 core

layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aNormal;
layout (location=2) in vec2 aTextCord;

//flat out vec2 textcord;
//out vec3 iNormal;
//out vec3 iFragPos;
const int MAX_PARTICLES = 100;
uniform mat4 PARTICLE_MODELS[MAX_PARTICLES];


out VS_OUT {
    vec2 textcord;
    vec3 iNormal;
    vec3 iFragPos;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;




void main(){
    vs_out.textcord = aTextCord;
    vs_out.iNormal = mat3(transpose(inverse(model))) * aNormal;
    vs_out.iFragPos = vec3(model * vec4(aPos, 1.0f));
    mat4 modelView = view * model;

    // Thanks random github gist for this solution
    modelView[0][0] = 1;
    modelView[1][1] = 1;
    modelView[2][2] = 1;

    modelView[0][1] = 0;
    modelView[0][2] = 0;
    modelView[1][0] = 0;
    modelView[1][2] = 0;
    modelView[2][0] = 0;
    modelView[2][1] = 0;
    gl_Position = (projection * modelView * vec4(aPos, 1.0f));
}