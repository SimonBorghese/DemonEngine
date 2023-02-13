//
// Created by simon on 9/21/22.
//

#ifndef DEMONENGINE_DS_SOLOUDSOUND_H
#define DEMONENGINE_DS_SOLOUDSOUND_H
#include <soloud/soloud.h>
#include <soloud/soloud_wav.h>
#include "DS_3DSound.h"
#include "DS_SoLoudAttenuator.h"

namespace DemonAudio {

    class DS_SoLoudSound : public DS_3DSound{
    public:
        DS_SoLoudSound(SoLoud::Soloud *base) : soundBase(base){}
        //~DS_SoLoudSound() {}

        int loadSound(const char *targetFile);
        int playSound(int loops);
        int stopSound();

        int setPosition(glm::vec3 newPosition);
        int setVelocity(glm::vec3 newVelocity);

        void setListenPosition(glm::vec3 position, glm::vec3 normalizedDirection);


    private:
        SoLoud::Soloud *soundBase;
        DS_SoLoudAttenuator *_mainAttenuator;
        SoLoud::Wav soundSource;
        glm::vec3 _position = glm::vec3(0.0f);
        int soundHandle = -1;
        int handle = -1;
    };

} // DemonAudio

#endif //DEMONENGINE_DS_SOLOUDSOUND_H
