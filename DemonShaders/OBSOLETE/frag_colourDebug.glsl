#version 460 core

uniform sampler2D diffuse_texture;
uniform sampler2D normalMap;
uniform int enableNormal;

struct lightValues{
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

uniform lightValues Lights[100];
uniform int LightCount;
uniform vec3 viewPos;

// vec3 iLightPos;
in vec3 iFragPos;
in vec3 iNormal;
in vec2 textcord;

out vec4 FragColor;

flat in int penableBAD;


void main(){
    if (penableBAD == 0){
        vec3 lightColour = vec3(1.0f);
        vec3 ambient = vec3(0.0f);
        vec3 diffuse = vec3(0.0f);
        vec3 specular = vec3(0.0f);

        for (int l = 0; l < LightCount; l++){
            if (Lights[l].specValue > 0){
                //vec3 norm = normalize(iNormal);
                vec3 rgbnormal = vec3(iNormal);
                if (enableNormal == 1){
                    rgbnormal = texture(normalMap, textcord).rgb;
                    rgbnormal = normalize(rgbnormal * 2.0 - 1.0);
                }
                vec3 norm = normalize(rgbnormal);

                vec3 lightDir = vec3(0.0f);
                float intensity = 1.0f;
                float attenuation = 1.0f;
                if (Lights[l].position != vec3(0.0f) && Lights[l].cutOff == 0.0f){
                    lightDir = normalize(Lights[l].position - iFragPos);

                    // Now Calculate Attenuation
                    float distance = length(Lights[l].position - iFragPos);
                    float aConst = 1.0f;
                    float aLinear = 4.5f / Lights[l].distance;
                    float aQuadratic = 75.0f / (Lights[l].distance * Lights[l].distance);
                    attenuation = 1.0 / (aConst + aLinear * distance +
                    aQuadratic * (distance * distance));

                }
                else if (Lights[l].position != vec3(0.0f) && Lights[l].cutOff != 0.0f){
                    lightDir = normalize(Lights[l].position - iFragPos);
                    float theta = dot(lightDir, normalize(-Lights[l].direction));


                    float outerCutOff = Lights[l].outerCutOff;
                    if (outerCutOff == 0.0f){
                        outerCutOff = Lights[l].cutOff;
                    }

                    float epsilon   = outerCutOff - Lights[l].cutOff;
                    intensity = clamp((theta - outerCutOff) / epsilon, 0.0, 1.0);

                    if (theta > Lights[l].cutOff){
                        float distance = length(Lights[l].position - iFragPos);
                        float aConst = 1.0f;
                        float aLinear = 4.5f / Lights[l].distance;
                        float aQuadratic = 75.0f / (Lights[l].distance * Lights[l].distance);
                        attenuation = 1.0 / (aConst + aLinear * distance + aQuadratic * (distance * distance));
                        //attenuation *= intensity;

                    }
                    else {
                        attenuation = 0.0f;
                    }

                }

                else {
                    lightDir = normalize(-Lights[l].direction);
                }

                ambient += vec3(Lights[l].ambient) * attenuation;


                float diff = max(dot(norm, lightDir), 0.0);
                diffuse += diff * Lights[l].colour * attenuation * intensity;

                vec3 viewDir = normalize(viewPos - iFragPos);
                vec3 reflectDir = reflect(-lightDir, norm);

                float spec = pow(max(dot(viewDir, reflectDir), 0.0), Lights[l].specValue);
                specular += Lights[l].specularStrength * spec * Lights[l].colour * attenuation * intensity;

            }
        }

        vec4 outputText = vec4((ambient+diffuse+specular), 1.0f) * texture(diffuse_texture, textcord);
        //vec4 outputText = vec4(1.0f) * texture(diffuse_texture, textcord);
        //vec4 outputText = texture(text2, textcord);
        FragColor = outputText;
    } else if (penableBAD == 1){
        //gl_FragDepth = gl_FragCoord.z;
        //FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    }

}