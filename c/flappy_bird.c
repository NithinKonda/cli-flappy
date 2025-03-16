#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <unistd.h>

#define MAX_OBSTACLES 10

typedef struct {
    float x;
    int gap_start;
    int gap_end;
    int passed;
} Obstacle;
