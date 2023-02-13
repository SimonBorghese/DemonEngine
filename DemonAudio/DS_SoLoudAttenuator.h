//
// Created by simon on 9/22/22.
//

#ifndef DEMONENGINE_DS_SOLOUDATTENUATOR_H
#define DEMONENGINE_DS_SOLOUDATTENUATOR_H
#include <soloud/soloud.h>

namespace DemonAudio {

    class DS_SoLoudAttenuator : SoLoud::AudioAttenuator {
    public:
        DS_SoLoudAttenuator() {}

        float attenuate(float aDistance,
                        float aMinDistance,
                        float aMaxDistance,
                        float ){
            float soundDist = aMaxDistance - aMinDistance;
            //printf("Unused: %f\n", aRolloffFactor);
            float aConst = 1.0f;
            float aLinear = (4.5f / soundDist);
            float aQuadratic = 75.0f / (soundDist * soundDist);
            return 1.0f / (aConst + aLinear * aDistance +
                                 aQuadratic * (aDistance * aDistance));
        }
    private:

    };

} // DemonAudio

#endif //DEMONENGINE_DS_SOLOUDATTENUATOR_H
