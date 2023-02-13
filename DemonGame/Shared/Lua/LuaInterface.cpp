//
// Created by simon on 1/20/23.
//

#include "LuaInterface.h"

namespace DG {
    namespace Lua {
        LuaInterface::LuaInterface(){
            L = luaL_newstate();
            luaL_openlibs(L);
        }
        LuaInterface::~LuaInterface(){
            lua_close(L);
        }

        void LuaInterface::initFile(const char *fileName){
            luaL_dofile(L, fileName);
        }
        void LuaInterface::registerFunction(const char *functionName, lua_CFunction function){
            lua_register(L, functionName, function);
        }

        void LuaInterface::callFunction(const char *function){
            lua_getglobal(L, function);
            // no returns or parameters, fuck you :)
            lua_call(L, 0,0);
        }

    } // DG
} // Lua