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