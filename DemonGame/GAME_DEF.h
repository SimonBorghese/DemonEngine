//
// Created by simon on 6/3/22.
//

#ifndef DEMONENGINE_GAME_DEF_H
#define DEMONENGINE_GAME_DEF_H

// No MacOS
// Because I said so (and no OpenGL support)
enum PLATFORMS {
    WINDOWS,
    LINUX
};

#define GAME_NAME "Protal"
#define GAME_VERSION 0.1
#define GAME_SHADER_DIRECTORY "Shaders"

#define GAME_DEBUG_SHADER_VERTEX "vertex_debug.glsl"
#define GAME_DEBUG_SHADER_FRAGMENT "fragment_debug.glsl"

// Literally hard coded shaders moment OwO
#define GAME_OBJECT_SHADER_VERTEX "vertex_noAnim.glsl"
#define GAME_OBJECT_SHADER_FRAGMENT "frag_colourDebug.glsl"
//#define GAME_OBJECT_SHADER_VERTEX "vertex_gouraud.glsl"
//#define GAME_OBJECT_SHADER_FRAGMENT "frag_gouraud.glsl"

#if defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__) || defined(__MINGW32__) || defined(__WIN32__)
#define OS_WINDemonShaders
#endif // WIN32 CHECK

#ifdef OS_WIN
#define GAME_PLATFORM WINDOWS
#else
#define GAME_PLATFORM LINUX
#endif // OS_WIN

#endif //DEMONENGINE_GAME_DEF_H
