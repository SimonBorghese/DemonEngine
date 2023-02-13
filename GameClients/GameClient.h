//
// Created by simon on 1/29/23.
//

#ifndef DEMONENGINE_GAMECLIENT_H
#define DEMONENGINE_GAMECLIENT_H

class GameClient{
public:
    virtual void init() = 0;
    virtual void loop() = 0;
    virtual void destroy() = 0;
};

#endif //DEMONENGINE_GAMECLIENT_H
