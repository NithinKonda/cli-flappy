import curses
import random
import time


class FlappyBird:
    def __init__(self, screen):
        self.screen = screen
        curses.curs_set(0)  
        self.screen.timeout(0)
        
        self.height, self.width = self.screen.getmaxyx()
        
        self.bird_char = ">"
        self.obstacle_char = "|"
        self.gap_size = 6
        self.gravity = 0.5  
        self.flap_power = -2  
        
        self.reset_game()


        