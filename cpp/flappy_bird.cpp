#include <ncurses.h>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <algorithm>

class Obstacle {
public:
    float x;
    int gapStart;
    int gapEnd;
    bool passed;
    
    Obstacle(float x, int gapStart, int gapSize) 
        : x(x), gapStart(gapStart), gapEnd(gapStart + gapSize), passed(false) {}
};