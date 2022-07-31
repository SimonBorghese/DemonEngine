#version 460 core
layout (points) in;
layout (triangle_strip, max_vertices = 6) out;

/*
billiboardVertices[0].iPosition = glm::vec3(-0.5f, -0.5f, -0.0f);
    billiboardVertices[0].iTextCord = glm::vec3(-1.0f, -1.0f, 0.0f);
    billiboardVertices[0].iNormal = glm::vec3(1.0f);

    billiboardVertices[1].iPosition = glm::vec3(0.5f, -0.5f, 0.0f);
    billiboardVertices[1].iTextCord = glm::vec3(1.0f, -1.0f, 0.0f);
    billiboardVertices[1].iNormal = glm::vec3(1.0f);

    billiboardVertices[2].iPosition = glm::vec3(-0.5f, 0.5f, -0.0f);
    billiboardVertices[2].iTextCord = glm::vec3(-1.0f, 1.0f, 0.0f);
    billiboardVertices[2].iNormal = glm::vec3(1.0f);

    billiboardVertices[3].iPosition = glm::vec3(0.5f, 0.5f, 0.0f);
    billiboardVertices[3].iTextCord = glm::vec3(1.0f, 1.0f, 0.0f);
    billiboardVertices[3].iNormal = glm::vec3(1.0f);
*/

/*
indices[0] = 0;
    indices[1] = 2;
    indices[2] = 3;

    indices[3] = 3;
    indices[4] = 1;
    indices[5] = 0;
*/

in VS_OUT {
    vec2 textcord;
    vec3 iNormal;
    vec3 iFragPos;
} gs_in[];

flat out vec2 textcord;
out vec3 iNormal;
out vec3 iFragPos;

void main() {

    gl_Position = gl_in[0].gl_Position + vec4(-0.5, -0.5, 0.0, 0.0);
    textcord = vec2(-0.5, -0.5) * 2;
        iNormal = gs_in[0].iNormal;
        iFragPos = gs_in[0].iFragPos;
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + vec4(-0.5, 0.5, 0.0, 0.0);
    textcord = vec2(-0.5, 0.5) * 2;
        iNormal = gs_in[0].iNormal;
        iFragPos = gs_in[0].iFragPos;
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + vec4(0.5, 0.5, 0.0, 0.0);
    textcord = vec2(0.5, 0.5) * 2;
        iNormal = gs_in[0].iNormal;
        iFragPos = gs_in[0].iFragPos;
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + vec4(0.5, 0.5, 0.0, 0.0);
    textcord = vec2(0.5, 0.5) * 2;
        iNormal = gs_in[0].iNormal;
        iFragPos = gs_in[0].iFragPos;
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + vec4(0.5, -0.5, 0.0, 0.0);
    textcord = vec2(0.5, -0.5) * 2;
        iNormal = gs_in[0].iNormal;
        iFragPos = gs_in[0].iFragPos;
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + vec4(-0.5, -0.5, 0.0, 0.0);
    textcord = vec2(-0.5, -0.5) * 2;
        iNormal = gs_in[0].iNormal;
        iFragPos = gs_in[0].iFragPos;
    EmitVertex();

    EndPrimitive();
}