#include <ncurses.h>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <iostream>
#include <ostream>
#include <vector>
#include <string>
#include <algorithm>

class Obstacle
{
public:
    float x;
    int gapStart;
    int gapEnd;
    bool passed;

    Obstacle(float x, int gapStart, int gapSize)
        : x(x), gapStart(gapStart), gapEnd(gapStart + gapSize), passed(false) {}
};

class FlappyBird
{
private:
    int width;
    int height;
    int birdX;
    float birdY;
    float birdVelocity;
    std::vector<std::string> birdChars = {">", "^", ">"};
    int birdFrame;
    std::string obstacleChar;
    std::string obstacleTop;
    std::string obstacleBottom;
    std::vector<Obstacle> obstacles;
    int gapSize;
    float gravity;
    float flapPower;
    int score;
    bool gameOver;
    std::chrono::time_point<std::chrono::steady_clock> lastUpdate;
    int animationCounter;

public:
    FlappyBird()
    {
        getmaxyx(stdscr, height, width);

        birdX = width / 4;
        birdChars = {">", "^", ">"};
        birdFrame = 0;
        obstacleChar = "║";
        obstacleTop = "╦";
        obstacleBottom = "╩";
        gapSize = 6;
        gravity = 0.05;
        flapPower = -0.3;

        resetGame();
        setupColors();
    }

    void setupColors()
    {
        if (has_colors())
        {
            start_color();
            init_pair(1, COLOR_YELLOW, COLOR_BLACK); // Bird
            init_pair(2, COLOR_GREEN, COLOR_BLACK);  // Obstacles
            init_pair(3, COLOR_WHITE, COLOR_BLACK);  // Score
            init_pair(4, COLOR_RED, COLOR_BLACK);    // Game over
            init_pair(5, COLOR_CYAN, COLOR_BLACK);   // Background
        }
    }

    void resetGame()
    {
        birdY = height / 2;
        birdVelocity = 0;
        obstacles.clear();
        newObstacle();
        score = 0;
        gameOver = false;
        lastUpdate = std::chrono::steady_clock::now();
        animationCounter = 0;
    }

    void newObstacle()
    {
        int gapStart = 3 + rand() % (height - gapSize - 6);
        obstacles.emplace_back(width - 1, gapStart, gapSize);
    }

    float getDeltaTime()
    {
        auto currentTime = std::chrono::steady_clock::now();
        float dt = std::chrono::duration<float>(currentTime - lastUpdate).count();
        lastUpdate = currentTime;
        return dt;
    }

    void updateBird(float dt)
    {
        birdVelocity += gravity * dt * 10;
        birdY += birdVelocity;

        if (animationCounter % 5 == 0)
        {
            birdFrame = (birdFrame + 1) % birdChars.size();
        }

        if (birdY < 1)
        {
            birdY = 1;
            birdVelocity = 0;
        }
        else if (birdY >= height - 1)
        {
            birdY = height - 1;
            gameOver = true;
        }
    }

    void updateObstacles(float dt)
    {
        float speed = 15.0 * dt;

        for (auto &obstacle : obstacles)
        {
            obstacle.x -= speed;

            if (!obstacle.passed && obstacle.x < birdX)
            {
                score++;
                obstacle.passed = true;
            }

            if (birdX >= static_cast<int>(obstacle.x) &&
                birdX <= static_cast<int>(obstacle.x) + 1 &&
                (static_cast<int>(birdY) < obstacle.gapStart || static_cast<int>(birdY) >= obstacle.gapEnd))
            {
                gameOver = true;
            }
        }

        // Remove obstacles that have gone off screen
        obstacles.erase(
            std::remove_if(obstacles.begin(), obstacles.end(),
                           [](const Obstacle &o)
                           { return o.x <= 0; }),
            obstacles.end());

        // Add new obstacle if needed
        if (obstacles.empty() || obstacles.back().x < width - 20)
        {
            newObstacle();
        }
    }

    void flap()
    {
        birdVelocity = flapPower;
    }

    void drawBackground()
    {
        if (has_colors())
        {
            attron(COLOR_PAIR(5));
            for (int y = 0; y < height; y++)
            {
                for (int x = 0; x < width; x += 4)
                {
                    if (x < width && y < height)
                    {
                        mvaddch(y, x, '·');
                    }
                }
            }
            attroff(COLOR_PAIR(5));
        }
    }

    void draw()
    {
        clear();

        // Draw background
        drawBackground();

        // Draw bird
        const std::string &birdChar = birdChars[birdFrame];
        if (has_colors())
            attron(COLOR_PAIR(1));
        mvaddstr(static_cast<int>(birdY), birdX, birdChar.c_str());
        if (has_colors())
            attroff(COLOR_PAIR(1));

        // Draw obstacles
        if (has_colors())
            attron(COLOR_PAIR(2));
        for (const auto &obstacle : obstacles)
        {
            int x = static_cast<int>(obstacle.x);
            if (x >= 0 && x < width)
            {
                // Draw top part
                for (int y = 0; y < obstacle.gapStart; y++)
                {
                    const std::string &charToUse = (y == obstacle.gapStart - 1) ? obstacleTop : obstacleChar;
                    mvaddstr(y, x, charToUse.c_str());
                }

                // Draw bottom part
                for (int y = obstacle.gapEnd; y < height; y++)
                {
                    const std::string &charToUse = (y == obstacle.gapEnd) ? obstacleBottom : obstacleChar;
                    mvaddstr(y, x, charToUse.c_str());
                }
            }
        }
        if (has_colors())
            attroff(COLOR_PAIR(2));

        // Draw score
        std::string scoreText = "Score: " + std::to_string(score);
        if (has_colors())
            attron(COLOR_PAIR(3));
        mvaddstr(0, 0, scoreText.c_str());
        if (has_colors())
            attroff(COLOR_PAIR(3));

        // Draw game over message
        if (gameOver)
        {
            std::string gameOverText = "GAME OVER - Press 'r' to restart or 'q' to quit";
            int x = std::max(0, (width - static_cast<int>(gameOverText.length())) / 2);

            if (has_colors())
                attron(COLOR_PAIR(4));
            mvaddstr(height / 2, x, gameOverText.c_str());
            if (has_colors())
                attroff(COLOR_PAIR(4));
        }

        refresh();
    }

    void run()
    {
        bool running = true;
        while (running)
        {
            // Process input
            int ch = getch();
            switch (ch)
            {
            case 'q':
                running = false;
                break;
            case 'r':
                if (gameOver)
                {
                    resetGame();
                }
                break;
            case ' ':
            case KEY_UP:
                if (!gameOver)
                {
                    flap();
                }
                break;
            }

            // Get time delta
            float dt = getDeltaTime();

            // Update game state
            if (!gameOver)
            {
                updateBird(dt);
                updateObstacles(dt);
            }

            // Draw everything
            draw();

            animationCounter++;

            // Cap the frame rate
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
};

int main()
{
    // Initialize ncurses
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0); // Hide cursor
    timeout(0);  // Non-blocking getch

    // Initialize random
    std::srand(std::time(nullptr));

    try
    {
        FlappyBird game;
        game.run();
    }
    catch (const std::exception &e)
    {
        endwin();
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    // Clean up
    endwin();
    return 0;
}