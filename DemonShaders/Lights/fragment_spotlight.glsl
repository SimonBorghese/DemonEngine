#version 330 core

//...Empty Fragment Shader...


float near_plane = 1.0f;
float far_plane = 50.0f;

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0;// Back to NDC
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));
}

void main(){
    //gl_FragDepth = (LinearizeDepth(gl_FragCoord.z)/far_plane);
    gl_FragDepth = gl_FragCoord.z;
}