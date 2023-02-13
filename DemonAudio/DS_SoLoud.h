//
// Created by simon on 9/21/22.
//

#ifndef DEMONENGINE_DS_SOLOUD_H
#define DEMONENGINE_DS_SOLOUD_H
#include <soloud/soloud.h>
#include "DS_3DAudio.h"
#include "DS_SoLoudSound.h"
#include "DS_SoLoudAttenuator.h"

namespace DemonAudio {

    class DS_SoLoud : public DS_3DAudio{
    public:
        DS_SoLoud(){}
        //~DS_SoLoud() {}

        int initSystem();
        int destroySystem();

        void setListenPosition(glm::vec3 position, glm::vec3 normalizedDirection);

        DS_SoLoudSound *createSound(const char *file);

        void update(){
            soundObj.update3dAudio();
        }


    private:
        SoLoud::Soloud soundObj;
    };

} // DemonAudio

#endif //DEMONENGINE_DS_SOLOUD_H
