//
// Created by simon on 9/21/22.
//

#include "DS_SoLoudSound.h"

namespace DemonAudio {
    int DS_SoLoudSound::loadSound(const char *targetFile){
        handle = soundSource.load(targetFile);
        _mainAttenuator = new DS_SoLoudAttenuator;
        //soundSource.set3dAttenuator((SoLoud::AudioAttenuator*) _mainAttenuator);
        return 1;
    }
    int DS_SoLoudSound::playSound(int loops){
        assert(handle != -1);
        //soundSource.set3dMinMaxDistance(0.0f, 10.0f);

        soundSource.set3dAttenuation(SoLoud::AudioSource::LINEAR_DISTANCE, 0.56f);
        soundSource.set3dMinMaxDistance(0.0f, 10.0f);
        soundHandle = soundBase->play3d(soundSource, _position.x, _position.y, _position.z);
        //soundBase->set3dSourceMinMaxDistance(soundHandle, 0.0f, 1.0f);
        //soundBase->set3dSourceAttenuation(soundHandle, SoLoud::AudioSource::EXPONENTIAL_DISTANCE, 0.25f);
        //


        //soundBase->set3dSourceAttenuation(soundHandle, 0, 0.5f );
        //soundSource.set3dMinMaxDistance( 10000.0f, 10000000000.0f);
        if (loops == -1){
            soundBase->setLooping(soundHandle, true);
        }
        return 1;
    }
    int DS_SoLoudSound::stopSound(){
        assert(handle != -1);
        soundBase->stop(handle);
        return 1;
    }

    int DS_SoLoudSound::setPosition(glm::vec3 newPosition){
        //assert(soundHandle != -1);
        if (soundHandle != -1) {
            soundBase->set3dSourcePosition(soundHandle, newPosition.x, newPosition.y, newPosition.z);
        }
        _position = newPosition;

        return 1;
    }
    int DS_SoLoudSound::setVelocity(glm::vec3 velocity){
        soundBase->set3dSourceVelocity(soundHandle, velocity.x, velocity.y, velocity.z);
        return 1;
    }

    void DS_SoLoudSound::setListenPosition(glm::vec3 position, glm::vec3 normalizedDirection){
        soundBase->set3dListenerAt(position.x, position.y, position.z);
        soundBase->set3dListenerAt(normalizedDirection.x, normalizedDirection.y, normalizedDirection.z);
        soundBase->set3dListenerUp(0.0f, 1.0f, 0.0f);
    }
} // DemonAudio