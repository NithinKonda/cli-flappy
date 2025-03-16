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


void init_colors() {
    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_YELLOW, COLOR_BLACK);  // Bird
        init_pair(2, COLOR_GREEN, COLOR_BLACK);   // Obstacles
        init_pair(3, COLOR_WHITE, COLOR_BLACK);   // Score
        init_pair(4, COLOR_RED, COLOR_BLACK);     // Game over
        init_pair(5, COLOR_CYAN, COLOR_BLACK);    // Background
    }
}