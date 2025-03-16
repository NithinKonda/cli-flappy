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



float get_delta_time(GameState* game) {
    struct timespec current_time;
    clock_gettime(CLOCK_MONOTONIC, &current_time);
    
    float dt = (current_time.tv_sec - game->last_update.tv_sec) + 
               (current_time.tv_nsec - game->last_update.tv_nsec) / 1000000000.0;
    
    game->last_update = current_time;
    return dt;
}



void update_bird(GameState* game, float dt) {
    game->bird_velocity += game->gravity * dt * 10;
    game->bird_y += game->bird_velocity;
    
    if (game->animation_counter % 5 == 0) {
        game->bird_frame = (game->bird_frame + 1) % 3;
    }
    
    if (game->bird_y < 1) {
        game->bird_y = 1;
        game->bird_velocity = 0;
    } else if (game->bird_y >= game->height - 1) {
        game->bird_y = game->height - 1;
        game->game_over = 1;
    }
}


void update_obstacles(GameState* game, float dt) {
    float speed = 15.0 * dt;
    

    int new_count = 0;
    for (int i = 0; i < game->num_obstacles; i++) {
        game->obstacles[i].x -= speed;
        

        if (!game->obstacles[i].passed && game->obstacles[i].x < game->bird_x) {
            game->score++;
            game->obstacles[i].passed = 1;
        }
        

        if (game->bird_x >= (int)game->obstacles[i].x && 
            game->bird_x <= (int)game->obstacles[i].x + 1 && 
            ((int)game->bird_y < game->obstacles[i].gap_start || (int)game->bird_y >= game->obstacles[i].gap_end)) {
            game->game_over = 1;
        }
        

        if (game->obstacles[i].x > 0) {
            if (i != new_count) {
                game->obstacles[new_count] = game->obstacles[i];
            }
            new_count++;
        }
    }
    
    game->num_obstacles = new_count;
    

    if (game->num_obstacles == 0 || 
        (game->num_obstacles > 0 && game->obstacles[game->num_obstacles-1].x < game->width - 20)) {
        new_obstacle(game);
    }
}



void flap(GameState* game) {
    game->bird_velocity = game->flap_power;
}



void draw_background(GameState* game) {
    if (has_colors()) {
        attron(COLOR_PAIR(5));
        for (int y = 0; y < game->height; y++) {
            for (int x = 0; x < game->width; x += 4) {
                if (x < game->width && y < game->height) {
                    mvaddch(y, x, '·');
                }
            }
        }
        attroff(COLOR_PAIR(5));
    }
}