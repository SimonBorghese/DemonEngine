//
// Created by simon on 9/21/22.
//

#ifndef DEMONENGINE_DS_3DSOUND_H
#define DEMONENGINE_DS_3DSOUND_H
#include <glm/glm.hpp>

namespace DemonAudio {

    class DS_3DSound {
    public:
        DS_3DSound() {}
        //virtual ~DS_3DSound();

        virtual int loadSound(const char *targetFile) = 0;
        virtual int playSound(int loops) = 0;
        virtual int stopSound() = 0;

        virtual int setPosition(glm::vec3 newPosition) = 0;
        virtual int setVelocity(glm::vec3 newVelocity) = 0;
        //virtual int setFalloff(int constant, int linear, int quadratic) = 0;
    private:

    };

} // DemonAudio

#endif //DEMONENGINE_DS_3DSOUND_H
