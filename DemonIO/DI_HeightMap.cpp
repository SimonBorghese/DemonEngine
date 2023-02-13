//
// Created by simon on 10/3/22.
//

#include "DI_HeightMap.h"

namespace DemonIO {
    DemonBase::b_Mesh **DI_HeightMap::loadMeshesFromFile(const char *, unsigned int *, glm::vec3 ){
        /*
        int width, height, nChannels;
        unsigned char *data = stbi_load(fileName,
                                        &width, &height, &nChannels,
                                        0);



        std::vector<Vertex> vertices;
        float yScale = 64.0f / 256.0f, yShift = 16.0f;  // apply a scale+shift to the height data
        for(int i = 0; i < height; i++)
        {
            for(int j = 0; j < width; j++)
            {
                // retrieve texel for (i,j) tex coord
                unsigned char* texel = data + (j + width * i) * nChannels;
                // raw height at coordinate
                unsigned char y = texel[0];

                // vertex
                Vertex newVertex;

                glm::vec3 vertex(-height/2.0f + i, (int)y * yScale - yShift, -width/2.0f + j);
                newVertex.iPosition = vertex;
                newVertex.iNormal = glm::vec3(1.0f);
                newVertex.iTextCord = glm::vec2(0.0f);
                newVertex.animationBody = -1;
                newVertex.iBoneIds = glm::ivec4(-1);
                vertices.push_back(newVertex);

            }
        }
        std::vector<unsigned int> indices;
        for( int i = 0; i < height-1; i++)       // for each row a.k.a. each strip
        {
            for( int j = 0; j < width; j++)      // for each column
            {
                for(unsigned int k = 0; k < 2; k++)      // for each side of the strip
                {
                    indices.push_back(j + width * (i + k));
                }
            }
        }

        const unsigned int NUM_STRIPS = height-1;
        const unsigned int NUM_VERTS_PER_STRIP = width*2;

        DemonBase::b_Mesh *map = new DemonBase::b_Mesh(vertices.data(), vertices.size(), indices.data(), indices.size());
       // map->setIndexOffset(sizeof(unsigned int)
       //                     * NUM_VERTS_PER_STRIP
       //                     * strip);
         */


    return nullptr;
    }
} // DemonIO