//
// Created by simon on 1/13/23.
//

#ifndef DEMONENGINE_TEXTURE_H
#define DEMONENGINE_TEXTURE_H
#include <glad/glad.h>
#include <STB_IMAGE/stb_image.h>
#include "Shader.h"
#include <string>


namespace DGL {

    class Texture {
    public:
        Texture(const char *diffuse, const char *normal = "");
        virtual ~Texture();

        void createTexture();
        void bindTextures();
        void destroyTextures();

        void setDiffuse(GLuint diffuse){
            _diffuseTexture = diffuse;
        }

    private:
        std::string _diffuseFile;
        std::string _normalFile;
        GLuint _diffuseTexture = 0;
        GLuint _normalTexture = 0;
    };

} // DGL

#endif //DEMONENGINE_TEXTURE_H
