//
// Created by simon on 1/13/23.
//

#define STB_IMAGE_IMPLEMENTATION
#include "Texture.h"

namespace DGL {
    Texture::Texture(const char *diffuse, const char *normal) : _diffuseFile(diffuse), _normalFile(normal){}
    Texture::~Texture(){}

    void Texture::createTexture(){
        if (strcmp(_diffuseFile.c_str(), "")){
            glGenTextures(1, &_diffuseTexture);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, _diffuseTexture);


            stbi_set_flip_vertically_on_load(1);

            int width = 0, height = 0, nrChannels = 0;
            //fmt::print("Found file: {}\n", fileName);
            void *data = stbi_load(_diffuseFile.c_str(), &width, &height, &nrChannels, 0);

            GLenum targetFormat = GL_RGB;
                switch (nrChannels) {
                    case 3:
                        targetFormat = GL_RGB;
                        break;
                    case 4:
                        targetFormat = GL_RGBA;
                        break;
                    default:
                        targetFormat = GL_RGB;
                        break;
                }


            if (!data) {
                if (strcmp(_diffuseFile.c_str(), "")) {
                    ALERT("MESH");
                    printf("Failed to load image: %s\n", _diffuseFile.c_str());
                }
                data = stbi_load("data/missing.png", &width, &height, &nrChannels, 3);
                if (!data) {
                    ALERT("MESH");
                    printf("CRITICAL ERROR: UN-ABLE TO LOAD ESSENTIAL FILE!\n");
                }
            }
            glTexImage2D(GL_TEXTURE_2D, 0, targetFormat, width, height, 0, targetFormat, GL_UNSIGNED_BYTE, data);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glGenerateMipmap(GL_TEXTURE_2D);
            stbi_image_free(data);
        } else{
            _diffuseTexture = (GLuint) -1;
        }

        if (strcmp(_normalFile.c_str(), "")){
            glGenTextures(1, &_normalTexture);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, _normalTexture);


            stbi_set_flip_vertically_on_load(1);


            int width = 0, height = 0, nrChannels = 0;
            //fmt::print("Found file: {}\n", fileName);
            void *data = stbi_load(_normalFile.c_str(), &width, &height, &nrChannels, 0);

            GLenum targetFormat = GL_RGB;
            switch (nrChannels) {
                case 3:
                    targetFormat = GL_RGB;
                    break;
                case 4:
                    targetFormat = GL_RGBA;
                    break;
                default:
                    targetFormat = GL_RGB;
                    break;
            }


            if (!data) {
                if (strcmp(_normalFile.c_str(), "")) {
                    ALERT("MESH");
                    printf("Failed to load image: %s\n", _normalFile.c_str());
                }
                data = stbi_load("data/missing.png", &width, &height, &nrChannels, 3);
                if (!data) {
                    ALERT("MESH");
                    printf("CRITICAL ERROR: UN-ABLE TO LOAD ESSENTIAL FILE!\n");
                }
            }
            glTexImage2D(GL_TEXTURE_2D, 0, targetFormat, width, height, 0, targetFormat, GL_UNSIGNED_BYTE, data);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glGenerateMipmap(GL_TEXTURE_2D);
            stbi_image_free(data);
        } else{
            _normalTexture = (GLuint) -1;
        }

    }
    void Texture::bindTextures(){
        if (_diffuseTexture != (GLuint) -1){
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, _diffuseTexture);
            DGL::Shader::_textures._enableDiffuse = 1;
        } else { DGL::Shader::_textures._enableDiffuse = 0; }

        if (_normalTexture != (GLuint) -1){
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, _normalTexture);
            DGL::Shader::_textures._enableNormal = 1;
        } else { DGL::Shader::_textures._enableNormal = 1; }

    }
    void Texture::destroyTextures(){

    }
} // DGL