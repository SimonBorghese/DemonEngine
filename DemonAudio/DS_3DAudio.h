//
// Created by simon on 9/21/22.
//

#ifndef DEMONENGINE_DS_3DAUDIO_H
#define DEMONENGINE_DS_3DAUDIO_H
#include <glm/glm.hpp>

namespace DemonAudio {

    class DS_3DAudio {
    public:
        DS_3DAudio() {}
        //virtual ~DS_3DAudio() = 0;
        virtual int initSystem() = 0;
        virtual int destroySystem() = 0;

        virtual void setListenPosition(glm::vec3 position, glm::vec3 normalizedDirection) = 0;

    private:

    };

} // DemonAudio

#endif //DEMONENGINE_DS_3DAUDIO_H
