//
// Created by simon on 1/29/23.
//

#ifndef DEMONENGINE_GAMECLIENT_H
#define DEMONENGINE_GAMECLIENT_H

#include <string>

#define DEFAULT_TYPE "NOTYPE"
#define DEFAULT_NAME "NONAME"

class GameClient {
public:
    virtual void init() = 0;

    virtual void loop() = 0;

    virtual void destroy() = 0;

    virtual void onContact(GameClient *) {}

    virtual void onTrigger(GameClient *) {}

    std::string clientType = DEFAULT_TYPE;
    std::string clientName = DEFAULT_NAME;
};

#endif //DEMONENGINE_GAMECLIENT_H
