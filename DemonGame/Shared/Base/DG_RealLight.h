//
// Created by simon on 6/24/22.
//

#ifndef DEMONENGINE_DG_REALLIGHT_H
#define DEMONENGINE_DG_REALLIGHT_H
#include "DG_Light.h"
#include <glm/glm.hpp>

namespace DemonGame {

    class DG_RealLight : public DG_Light{
    public:

    protected:
        float _distance;
        glm::vec3 _position;
    };

} // DemonGame

#endif //DEMONENGINE_DG_REALLIGHT_H
