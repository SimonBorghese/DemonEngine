//
// Created by simon on 5/23/22.
//

#ifndef DEMONENGINE_DR_LUA_H
#define DEMONENGINE_DR_LUA_H
#include <DemonLua/LuaHelper.h>
#include <functional>
#include "DR_Window.h"
#include "DR_OpenGL.h"

#ifdef DEFINEVARS
DemonRender::DR_Window *__DR_LUA_win;
DemonRender::DR_OpenGL *__DR_LUA_renManager;

int DR_WINDOW_MOVE_WINDOW(lua_State *L){
    lua_Integer newX = lua_tointeger(L, -2);
    lua_Integer newY = lua_tointeger(L, -1);

    __DR_LUA_win->moveWindow(newX, newY);

    return 1;
}

int RENDER_FLIP(lua_State *){
    __DR_LUA_win->flip();
    return 1;
}

int RENDER_CLEAR(lua_State *){
    __DR_LUA_renManager->clear();
    return 1;
}

void DR_registerLuaFuncs(DemonRender::DR_Window *win, DemonRender::DR_OpenGL *oGL){
    __DR_LUA_win = win;
    __DR_LUA_renManager = oGL;
    DemonLua::LuaHelper::registerFunction("DR_WINDOW_MOVE_WINDOW", DR_WINDOW_MOVE_WINDOW );
    DemonLua::LuaHelper::registerFunction("RENDER_FLIP", RENDER_FLIP);
    DemonLua::LuaHelper::registerFunction("RENDER_CLEAR", RENDER_CLEAR);
}
#else
extern void DR_registerLuaFuncs(DemonRender::DR_Window *win);
extern int DR_WINDOW_MOVE_WINDOW(lua_State *L);
#endif

#endif //DEMONENGINE_DR_LUA_H
