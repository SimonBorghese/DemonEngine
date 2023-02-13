//
// Created by simon on 9/21/22.
//

#include "DS_SoLoud.h"

namespace DemonAudio {

    int DS_SoLoud::initSystem(){
        soundObj.init(SoLoud::Soloud::CLIP_ROUNDOFF, SoLoud::Soloud::OPENAL);
        return 1;
    }
    int DS_SoLoud::destroySystem(){
        soundObj.deinit();
        return 1;
    }

    void DS_SoLoud::setListenPosition(glm::vec3 position, glm::vec3 normalizedDirection){
        soundObj.set3dListenerAt(position.x, position.y, position.z);
        soundObj.set3dListenerAt(normalizedDirection.x, normalizedDirection.y, normalizedDirection.z);
        soundObj.set3dListenerUp(0.0f, 1.0f, 0.0f);
    }

    DS_SoLoudSound* DS_SoLoud::createSound(const char *file){
        DS_SoLoudSound *newSound = new DS_SoLoudSound(&soundObj);
        newSound->loadSound(file);
        return newSound;
    }
} // DemonAudio