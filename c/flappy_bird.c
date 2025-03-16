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

typedef struct {
    int width;
    int height;
    int bird_x;
    float bird_y;
    float bird_velocity;
    char* bird_chars[3];
    int bird_frame;
    const char* obstacle_char;
    const char* obstacle_top;
    const char* obstacle_bottom;
    Obstacle obstacles[MAX_OBSTACLES];
    int num_obstacles;
    int gap_size;
    float gravity;
    float flap_power;
    int score;
    int game_over;
    struct timespec last_update;
    int animation_counter;
} GameState;
