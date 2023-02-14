//
// Created by simon on 1/10/23.
//

#ifndef DEMONENGINE_SHADER_H
#define DEMONENGINE_SHADER_H
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <cstdio>
#include <string>
#include <DemonMacro/DemonLog.h>
#include <map>

namespace DGL {

    struct transformBlock{
        glm::mat4 view;
        glm::mat4 projection;
    };
    struct _textureStruct{
        int _enableDiffuse;
        int _enableNormal;
    };

    struct _lightSpec{
        glm::vec3  *position;
        glm::vec3  *direction;
        float      *cutOff;
        float      *outerCutOff;
        float      *distance;
        glm::vec3  *colour;
        float      *ambient;
        float      *specularStrength;
        int        *specValue;
        int        *enableShadow;
    };
#define NUM_LIGHTS 20
    struct dynamicLights{
        int numLights;
        glm::vec3 viewPos;
        glm::vec3 position[NUM_LIGHTS];
        glm::vec3 direction[NUM_LIGHTS];
        float cutOff[NUM_LIGHTS];
        float outerCutOff[NUM_LIGHTS];

        float distance[NUM_LIGHTS];

        glm::vec3 colour[NUM_LIGHTS];
        float ambient[NUM_LIGHTS];
        float specularStrength[NUM_LIGHTS];
        int  specValue[NUM_LIGHTS];
        int  enableShadow[NUM_LIGHTS];
    };

    class Shader {
    public:
        Shader(const char *vertex, const char *fragment);
        Shader(const char *vertex, const char *geometry, const char *fragment);
        virtual ~Shader();

        void createShader();
        void useShader() const;
        void destroyShader() const;

        void bindGlobals();
        static void setView(glm::mat4 view);
        static void setProjection(glm::mat4 projection);
        static void remakeGlobals();
        static void remakeTextures();

        void uniformMat4(std::string location, glm::mat4 target);
        void uniformInt(std::string location, glm::int32 target);
        void uniformVec3(std::string location, glm::vec3 target);
        void uniformFloat(std::string location, glm::float32 target);

        static GLuint _transformBlock;
        static struct transformBlock _globalTransform;

        static GLuint _dynamicLightUBO;
        static struct dynamicLights _dynamicLights;

        static GLuint _texturesUBO;
        static struct _textureStruct _textures;
    private:

        GLuint getUniformLocation(std::string uniform);

        std::string _shaders[3] = {"", "", ""};
        GLuint _shaderProgram = 0;
        GLuint _transformLocation = 0;
        GLuint _dynamicLightsLocation = 0;
        GLuint _texturesLocation = 0;

        std::map<std::string, GLuint> _locationCache;
    };

} // DGL

#endif //DEMONENGINE_SHADER_H
