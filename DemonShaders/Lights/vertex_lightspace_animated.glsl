/*
#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 model;

out vec4 FragPos;

void main()
{
    gl_Position = model * vec4(aPos, 1.0f);
}
*/
#version 430 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTextCords;
layout (location=3) in ivec4 boneIds;
layout (location=4) in vec4 weights;
layout (location=5) in int body;

uniform mat4 model;

const int MAX_BONES = 25;
const int MAX_BODIES = 10;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 bodyTransforms[MAX_BODIES][MAX_BONES];

out vec3 FragPos;



vec3 qrot(vec4 q, vec3 v)
{
    return v + 2.0*cross(q.xyz, cross(q.xyz,v) + q.w*v);
}

void main(){

    vec4 totalPosition = vec4(0.0f);


    for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
    {
        if (boneIds[i] < 0){
            continue;
        }
        if (boneIds[i] >=MAX_BONES)
        {
            totalPosition = vec4(aPos, 1.0f);
            break;
        }

        vec4 localPosition = bodyTransforms[body][boneIds[i]] * vec4(aPos, 1.0f);
        totalPosition += localPosition * weights[i];
    }

    //if (boneIds.x < 0){
    //    totalPosition = vec4(aPos, 1.0f);
    //}


    FragPos = vec3(model * totalPosition);

    gl_Position = ( model * totalPosition);
}

