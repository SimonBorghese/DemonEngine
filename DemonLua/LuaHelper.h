//
// Created by simon on 5/23/22.
//

#ifndef DEMONENGINE_LUAHELPER_H
#define DEMONENGINE_LUAHELPER_H

#include <lua.hpp>



namespace DemonLua{
    class LuaHelper{
    public:
        inline static lua_State *L = nullptr;

        static void initLua();

        static void registerFunction(const char *name, int (*targetFunc)(lua_State*) );

        static void loadScript(const char *scriptName);

        static void executeLuaVoid(const char *functionName);

        static void closeLua();
    private:

    };
};

#endif //DEMONENGINE_LUAHELPER_H
