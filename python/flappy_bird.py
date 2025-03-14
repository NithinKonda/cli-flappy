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



    def update_bird(self, dt):
        self.bird_velocity += self.gravity * dt * 10
        self.bird_y += self.bird_velocity
        if self.bird_y < 1:
            self.bird_y = 1
            self.bird_velocity = 0
        elif self.bird_y >= self.height - 1:
            self.bird_y = self.height - 1
            self.game_over = True


    def update_obstacles(self, dt):
        speed = 15 * dt
        
        for obstacle in self.obstacles:
            obstacle['x'] -= speed
            
            if not obstacle['passed'] and obstacle['x'] < self.bird_x:
                self.score += 1
                obstacle['passed'] = True
            
            if (self.bird_x == int(obstacle['x']) and 
                (self.bird_y < obstacle['gap_start'] or self.bird_y >= obstacle['gap_end'])):
                self.game_over = True
        
        self.obstacles = [o for o in self.obstacles if o['x'] > 0]
        
        if len(self.obstacles) == 0 or self.obstacles[-1]['x'] < self.width - 20:
            self.new_obstacle()



     def flap(self):
        self.bird_velocity = self.flap_power
    
    def draw(self):
        self.screen.clear()
        
        self.screen.addstr(int(self.bird_y), self.bird_x, self.bird_char)
        
        for obstacle in self.obstacles:
            x = int(obstacle['x'])
            if 0 <= x < self.width:
                for y in range(obstacle['gap_start']):
                    self.screen.addstr(y, x, self.obstacle_char)
                
                for y in range(obstacle['gap_end'], self.height):
                    self.screen.addstr(y, x, self.obstacle_char)
        
        self.screen.addstr(0, 0, f"Score: {self.score}")
        
        if self.game_over:
            game_over_text = "GAME OVER - Press 'r' to restart or 'q' to quit"
            x = max(0, (self.width - len(game_over_text)) // 2)
            self.screen.addstr(self.height // 2, x, game_over_text)
        
        self.screen.refresh()