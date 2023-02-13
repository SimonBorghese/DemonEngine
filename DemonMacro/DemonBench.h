//
// Created by simon on 1/4/23.
//

#ifndef DEMONENGINE_DEMONBENCH_H
#define DEMONENGINE_DEMONBENCH_H
#include <chrono>
#include <functional>
#include <SDL2/SDL.h>
#include <map>
#include <string>


namespace DemonBench{
    typedef uint32_t timePoint;
    typedef struct{
        int numCalls;
        uint64_t totalTime;
    } callState;

    extern std::map<std::string, callState*> _benchmarkState;


    timePoint getTime();

    uint32_t getMilliseconds(timePoint t1);

    void Benchmark(const char *, std::function<void()> func);

}

#endif //DEMONENGINE_DEMONBENCH_H
