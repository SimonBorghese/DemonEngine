#version 430 core

layout (local_size_x = 1, local_size_y = 1) in;

//layout (rgba32f, binding = 0) uniform image2D imgOutput;

layout (std430, binding=2) buffer shader_data
{
    uint num[];
} Out;


void main() {

    uint IndexThingy = (gl_GlobalInvocationID.y * gl_NumWorkGroups.y) + gl_GlobalInvocationID.x;
    atomicExchange(Out.num[IndexThingy], 1);


}