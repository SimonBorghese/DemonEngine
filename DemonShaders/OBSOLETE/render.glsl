#version 330 core

uniform sampler2D diffuse_texture;

in vec2 textcord;

out vec4 FragColor;

flat in int depth;


const float near_plane = 0.1;
const float far_plane = 100.0;

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // Back to NDC
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));
}
void main(){
    float depthValue = texture(diffuse_texture, textcord).r;
    FragColor = vec4(vec3(depthValue), 1.0);
}