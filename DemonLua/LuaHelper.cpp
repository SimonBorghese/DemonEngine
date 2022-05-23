//
// Created by simon on 5/23/22.
//
#include "LuaHelper.h"

void DemonLua::LuaHelper::initLua(){
    L = luaL_newstate();
    luaL_openlibs(L);
}

void DemonLua::LuaHelper::registerFunction(const char *name, int (*targetFunc)(lua_State*) ){
    lua_register(L, name, targetFunc);
}

void DemonLua::LuaHelper::loadScript(const char *scriptName){
    luaL_dofile(L, scriptName);
}

void DemonLua::LuaHelper::executeLuaVoid(const char *functionName){
    lua_getglobal(L, functionName);
    if (lua_isfunction(L, -1)){
        lua_pcall(L,0,0,0);
    }
}

void DemonLua::LuaHelper::closeLua(){
    lua_close(L);
}