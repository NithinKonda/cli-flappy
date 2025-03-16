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

void reset_game(GameState* game) {
    game->bird_y = game->height / 2;
    game->bird_velocity = 0;
    game->num_obstacles = 0;
    game->score = 0;
    game->game_over = 0;
    clock_gettime(CLOCK_MONOTONIC, &game->last_update);
    game->animation_counter = 0;
    
    int gap_start = rand() % (game->height - game->gap_size - 6) + 3;
    game->obstacles[0].x = game->width - 1;
    game->obstacles[0].gap_start = gap_start;
    game->obstacles[0].gap_end = gap_start + game->gap_size;
    game->obstacles[0].passed = 0;
    game->num_obstacles = 1;
}

void init_game(GameState* game) {
    getmaxyx(stdscr, game->height, game->width);
    
    game->bird_x = game->width / 4;
    game->bird_chars[0] = ">";
    game->bird_chars[1] = "^";
    game->bird_chars[2] = ">";
    game->bird_frame = 0;
    game->obstacle_char = "║";
    game->obstacle_top = "╦";
    game->obstacle_bottom = "╩";
    game->gap_size = 6;
    game->gravity = 0.05;
    game->flap_power = -0.3;
    
    reset_game(game);
}


void new_obstacle(GameState* game) {
    if (game->num_obstacles < MAX_OBSTACLES) {
        int gap_start = rand() % (game->height - game->gap_size - 6) + 3;
        game->obstacles[game->num_obstacles].x = game->width - 1;
        game->obstacles[game->num_obstacles].gap_start = gap_start;
        game->obstacles[game->num_obstacles].gap_end = gap_start + game->gap_size;
        game->obstacles[game->num_obstacles].passed = 0;
        game->num_obstacles++;
    }
}