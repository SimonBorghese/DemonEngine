#version 440 core

out vec4 FragColor;

in vec3 vertex;


struct _lightSpec{
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    float distance;

    vec3 colour;
    float ambient;
    float specularStrength;
    int  specValue;
};

layout (std430, binding=1) readonly buffer dynamicLights {
    int numLights;

    float _viewPos[3];

    float _position[20][3];
    float _direction[20][3];
    float cutOff[20];
    float outerCutOff[20];

    float distance[20];

    float _colour[20][3];
    float ambient[20];
    float specularStrength[20];
    int  specValue[20];
    int enableShadow[20];
};

layout (std140, binding=2) uniform Textures{
    int _enableDiffuse;
    int _enableNormal;
};

uniform sampler2D _diffuse;
uniform sampler2D _normal;
uniform samplerCube _shadowMap[20];

in vec3 iNormal;
in vec3 iFragPos;
in vec2 iTextCord;

vec3 viewPos;

uint samples = 20;
vec3 samplerOffsets[20] = vec3[]
(
vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1),
vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
);
/*
float ShadowCalculation(vec3 fragPos, vec3 lightPos, float planarCutOff, samplerCube targetShadow)
{
    float shadow = 0.0f;
    float shadow_bias = max(0.05 * (1.0 - dot(iNormal, (fragPos - lightPos))), 0.01 * (length(fragPos - lightPos)));
    float vDistance = length(viewPos - fragPos);
    float disk = (1.0 + (vDistance / planarCutOff)) / 25.0f;
    for (uint s = 0; s < samples; s++){
        float depth = texture(targetShadow, (fragPos - lightPos) + samplerOffsets[s] * disk).r;
        depth *= planarCutOff;
        if (length(fragPos - lightPos) - shadow_bias > depth){
            shadow += 1.0f;
        } else{
            shadow -= 1.0f;
        }
    }
    shadow /= float(samples);
    return shadow;
}
*/

float ShadowCalculation(vec3 fragPos, vec3 lightPos, float planarCutOff, samplerCube targetShadow)
{
    float shadow = 0.0f;
    float shadow_bias = max(0.05 * (1.0 - dot(iNormal, (fragPos - lightPos))) * max(length(fragPos - lightPos)*0.1, 1), 0.001);
    float vDistance = length(viewPos - fragPos);
    float disk = (1.0 + (vDistance / planarCutOff)) / 25.0f;

    float depth = texture(targetShadow, (fragPos - lightPos) * disk).r;
    depth *= planarCutOff;
    if ((length(fragPos - lightPos) > depth) || (length(fragPos - lightPos) >= planarCutOff)){
        shadow += 1.0f;
    } else{
        shadow -= 1.0f;
    }
    return shadow;
}


void main(){


    vec3 lightColour = vec3(1.0f);
    vec3 ambient = vec3(0.0f);
    vec3 diffuse = vec3(0.0f);
    vec3 specular = vec3(0.0f);

    viewPos = vec3(_viewPos[0], _viewPos[1], _viewPos[2]);
    int LightCount = numLights;

    float shadowSum = 0.0f;

    for (int l = 0; l < LightCount; l++){
        // DELETE ME!:
        vec3 cPositon = vec3(_position[l][0], _position[l][1], _position[l][2]);
        vec3 cColor = vec3(_colour[l][0], _colour[l][1], _colour[l][2]);
        vec3 cDirection = vec3(_direction[l][0], _direction[l][1], _direction[l][2]);

        float attenuation = 1.0f;
        if (specValue[l] > 0){
            //vec3 norm = normalize(iNormal);
            vec3 rgbnormal = vec3(iNormal);
            vec3 norm = normalize(rgbnormal);

            vec3 lightDir = vec3(0.0f);
            float intensity = 1.0f;
            if (cPositon != vec3(0.0f) && cutOff[l] == 0.0f){
                lightDir = normalize(cPositon - iFragPos);

                // Now Calculate Attenuation
                float lDistance = length(cPositon - iFragPos);
                float aConst = 1.0f;
                float aLinear = 4.5f / distance[l];
                float aQuadratic = 75.0f / (distance[l] * distance[l]);
                attenuation = 1.0 / (aConst + aLinear * lDistance +
                aQuadratic * (lDistance * lDistance));

            }
            else if (cPositon != vec3(0.0f) && cutOff[l] != 0.0f){
                lightDir = normalize(cPositon - iFragPos);

                float theta = dot(lightDir, normalize(-cDirection));
                //float theta = dot(lightDir, normalize(-vec3(0.0f)));


                float louterCutOff = outerCutOff[l];
                if (louterCutOff == 0.0f){
                    louterCutOff = cutOff[l];
                }

                float epsilon   = louterCutOff - cutOff[l];
                intensity = clamp((theta - louterCutOff) / epsilon, 0.0, 1.0);

                if (theta > cutOff[l]){
                    float ldistance = length(cPositon - iFragPos);
                    float aConst = 1.0f;
                    float aLinear = 4.5f / distance[l];
                    float aQuadratic = 75.0f / (distance[l] * distance[l]);
                    attenuation = 1.0 / (aConst + aLinear * ldistance + aQuadratic * (ldistance * ldistance));
                    attenuation *= intensity;

                }
                else {
                    attenuation = 0.0f;
                }

            }

            else {
                lightDir = normalize(-cDirection);
                //lightDir = normalize(-vec3(0.0f));
            }

            //attenuation *= 2.0f;
            float shadow = 0.0;
            if (enableShadow[l] == 1){
                //shadowSum += ShadowCalculation(iFragPos, cPositon, distance[l], _shadowMap[l]);
                shadow = ShadowCalculation(iFragPos, cPositon, distance[l], _shadowMap[l]);
            }
            //shadow *= attenuation;
            //if (shadowSum <= 0.0){
            if (shadow <= 0.0){
                ambient += (vec3(ambient[l]) * attenuation);

                //if (shadow <= 0.0){
                float diff = max(dot(norm, lightDir), 0.0);
                diffuse += (diff * cColor * attenuation * intensity);

                vec3 viewDir = normalize(viewPos - iFragPos);
                vec3 reflectDir = reflect(-lightDir, norm);

                float spec = pow(max(dot(viewDir, reflectDir), 0.0), specValue[l]);
                specular += (specularStrength[l] * spec * cColor * attenuation * intensity);
            }
            //}

        }


    }

    vec4 outputText = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    //shadowSum = max(0.0, shadowSum);
    //ambient += (1.0 - shadowSum);


    if (_enableDiffuse == 1){
        outputText = vec4( (ambient + diffuse + specular) , 1.0f) * texture(_diffuse, iTextCord);
        //outputText = vec4(normalize(viewPos) * normalize(iFragPos), 1.0f);
    } else {
        outputText = vec4(normalize(viewPos) * normalize(iFragPos), 1.0f);
    }


    FragColor = outputText;
    //FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f)


}