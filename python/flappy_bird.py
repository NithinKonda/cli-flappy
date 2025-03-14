import curses
import random
import time


class FlappyBird:
    def __init__(self, screen):
        self.screen = screen
        curses.curs_set(0)
        self.screen.timeout(0)
        self.height, self.width = self.screen.getmaxyx()
        self.bird_chars = [">", "^", ">"]
        self.bird_frame = 0
        self.obstacle_char = "║"
        self.obstacle_top = "╦"
        self.obstacle_bottom = "╩"
        self.gap_size = 3
        self.gravity = 0.05
        self.flap_power = -0.3
        self.bird_x = self.width // 4
        self.reset_game()
        self.setup_colors()


    def setup_colors(self):
        if curses.has_colors():
            curses.start_color()
            curses.init_pair(1, curses.COLOR_YELLOW, curses.COLOR_BLACK)  # Bird
            curses.init_pair(2, curses.COLOR_GREEN, curses.COLOR_BLACK)   # Obstacles
            curses.init_pair(3, curses.COLOR_WHITE, curses.COLOR_BLACK)   # Score
            curses.init_pair(4, curses.COLOR_RED, curses.COLOR_BLACK)     # Game over
            curses.init_pair(5, curses.COLOR_CYAN, curses.COLOR_BLACK)