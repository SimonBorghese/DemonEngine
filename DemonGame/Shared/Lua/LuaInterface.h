//
// Created by simon on 1/20/23.
//

#ifndef DEMONENGINE_LUAINTERFACE_H
#define DEMONENGINE_LUAINTERFACE_H
extern "C"{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
};
#include <functional>

namespace DG {
    namespace Lua {

        class LuaInterface {
        public:
            LuaInterface();
            virtual ~LuaInterface();

            void initFile(const char *fileName);
            void registerFunction(const char *functionName, lua_CFunction function);
            void callFunction(const char *function);
            lua_State* getState() { return L; }
        private:
            lua_State *L;

        };

    } // DG
} // Lua

#endif //DEMONENGINE_LUAINTERFACE_H
