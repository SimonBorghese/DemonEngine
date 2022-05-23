#include <stdlib.h>
#include <stdio.h>

#include <DemonLua/LuaHelper.h>
#include <string>
//#include <DemonRender/DR_LUA.h>
#include <DemonRender/DR_RenderManager.h>

int main(void)
{
    DemonLua::LuaHelper::initLua();
    DemonLua::LuaHelper::loadScript("script.lua");

    DemonRender::DR_RenderManager renderManager;
    renderManager.createRenderer("OwO", 800, 600);

    DemonLua::LuaHelper::executeLuaVoid("coolFunction");

    getchar();
    DemonLua::LuaHelper::closeLua();

    return 0;
}