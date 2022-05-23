#include <stdlib.h>
#include <stdio.h>

#include <DemonLua/LuaHelper.h>


int poop(lua_State* L) {
    // function args are on the lua stack, last arg is on top
    const char *arg_1 = lua_tostring(L, -2);
    const char *arg_2 = lua_tostring(L, -1);



    printf("[LUA&CPP] Found owo sussy: %s & an ultra sus: %s\n", arg_1, arg_2);
    return 1;
}

int main(void)
{
    printf("[CPP] Owo\n");
    DemonLua::LuaHelper::initLua();
    DemonLua::LuaHelper::loadScript("script.lua");
    DemonLua::LuaHelper::registerFunction("poop", poop);
    printf("[CPP] Preparing to execute\n");
    DemonLua::LuaHelper::executeLuaVoid("coolFunction");
    DemonLua::LuaHelper::closeLua();

    return 0;
}