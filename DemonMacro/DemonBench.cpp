//
// Created by simon on 2/6/23.
//
#include "DemonBench.h"

namespace DemonBench{
    std::map<std::string, callState*> _benchmarkState;
#ifdef BENCHMARK
    timePoint getTime() { return SDL_GetTicks(); }
    /*
    uint64_t getNanoseconds(timePoint t1){
        std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();

        return std::chrono::duration_cast<std::chrono::nanoseconds>(t2-t1).count();
    }
     */

    uint32_t getMilliseconds(timePoint t1){
        return SDL_GetTicks() - t1;

    }

    void Benchmark(const char *name, std::function<void()> func){
        auto currentState = _benchmarkState.find(std::string(name));
        if (currentState != _benchmarkState.end()){
            currentState->second->numCalls++;
        } else{
            _benchmarkState.insert(std::pair<std::string, callState*>(std::string(name), new callState));
            currentState = _benchmarkState.find(std::string(name));
            currentState->second->numCalls = 1;
            currentState->second->totalTime = 0;
        }
        auto currentTime = getTime();
        func();
        currentState->second->totalTime += getMilliseconds(currentTime);
        if (currentState->second->numCalls >= 60) {
            printf("%s Average Time: %f!\n", name, (double) currentState->second->totalTime / (double) currentState->second->numCalls);
            currentState->second->totalTime = 0;
            currentState->second->numCalls = 0;
        }
        return;

    }
#else
    timePoint getTime(){ return 0;}

    uint32_t getMilliseconds(timePoint ) { return 0;}

    void Benchmark(const char *, std::function<void()> func) { func();}
#endif
}