//
// Created by simon on 6/3/22.
//

#ifndef DEMONENGINE_GAME_DEF_H
#define DEMONENGINE_GAME_DEF_H

// No MacOS
// Because I said so (and no OpenGL support)
enum PLATFORMS{
    WINDOWS,
    LINUX
};

#define GAME_NAME "stage-game"
#define GAME_VERSION 0.1f

#if defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__) || defined(__MINGW32__) || defined(__WIN32__)
#define OS_WIN
#endif // WIN32 CHECK

#ifdef OS_WIN
#define GAME_PLATFORM WINDOWS
#else
#define GAME_PLATFORM LINUX
#endif // OS_WIN

#endif //DEMONENGINE_GAME_DEF_H
