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


    def reset_game(self):

        self.bird_x = self.width // 4
        self.bird_y = self.height // 2
        self.bird_velocity = 0
        
        self.obstacles = []
        self.new_obstacle()
        
        self.score = 0
        self.game_over = False
        self.last_update = time.time()


    def new_obstacle(self):
        gap_start = random.randint(3, self.height - self.gap_size - 3)
        obstacle = {
            'x': self.width - 1,
            'gap_start': gap_start,
            'gap_end': gap_start + self.gap_size,
            'passed': False
        }
        self.obstacles.append(obstacle)