#version 460 core

layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aNormal;
layout (location=2) in vec2 aTextCord;
layout (location=3) in ivec4 boneIds;
layout (location=4) in vec4 weights;
layout (location=5) in int body;

out vec2 textcord;
out vec3 iNormal;
out vec3 iFragPos;

const int MAX_BONES = 100;
const int MAX_BODIES = 10;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 bodyTransforms[MAX_BODIES][MAX_BONES];

flat out int mark;


uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform int enableBAD;

vec3 qrot(vec4 q, vec3 v)
{
    return v + 2.0*cross(q.xyz, cross(q.xyz,v) + q.w*v);
}

flat out int penableBAD;

void main(){
    penableBAD = enableBAD;

    if (enableBAD == 0){
        vec4 totalPosition = vec4(0.0f);
        vec3 totalNormal = vec3(0.0f);
        mat4 boneTransform = mat4(1.0f);

        if (body >= 69420){
            mark = 1;
        }
        else {
            mark = 0;
        }

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
            vec3 localNormal = mat3(bodyTransforms[body][boneIds[i]]) * aNormal;
            totalNormal += localNormal * weights[i];
        }
        if (boneIds.x < 0){
            totalPosition = vec4(aPos, 1.0f);
            totalNormal = aNormal;
        }


        textcord = aTextCord;
        iNormal = mat3(transpose(inverse(model))) * totalNormal;
        iFragPos = vec3(model * totalPosition);
        //totalPosition.y += sin(totalPosition.x) + cos(totalPosition.z);
        gl_Position = (projection * view * boneTransform * model * totalPosition);
    } else{
        textcord = aTextCord;
        gl_Position = projection * view * model * vec4(aPos, 1.0f);
    }

}