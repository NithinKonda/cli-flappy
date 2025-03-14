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
        self.gap_size = 6
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


    def reset_game(self):
        self.bird_y = self.height // 2
        self.bird_velocity = 0
        
        self.obstacles = []
        self.new_obstacle()
        
        self.score = 0
        self.game_over = False
        self.last_update = time.time()
        self.animation_counter = 0


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
        

        if self.animation_counter % 5 == 0:
            self.bird_frame = (self.bird_frame + 1) % len(self.bird_chars)
        

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
            

            if (self.bird_x >= int(obstacle['x']) and 
                self.bird_x <= int(obstacle['x']) + 1 and 
                (int(self.bird_y) < obstacle['gap_start'] or int(self.bird_y) >= obstacle['gap_end'])):
                self.game_over = True
        

        self.obstacles = [o for o in self.obstacles if o['x'] > 0]
        

        if len(self.obstacles) == 0 or self.obstacles[-1]['x'] < self.width - 20:
            self.new_obstacle()

    def flap(self):
        self.bird_velocity = self.flap_power


    def draw_background(self):
        if curses.has_colors():
            for y in range(self.height):
                for x in range(0, self.width, 4):
                    if 0 <= x < self.width and 0 <= y < self.height:
                        try:
                            self.screen.addch(y, x, '·', curses.color_pair(5))
                        except curses.error:
                            pass


    def draw(self):
        self.screen.clear()
        

        self.draw_background()
        

        bird_char = self.bird_chars[self.bird_frame]
        try:
            if curses.has_colors():
                self.screen.addstr(int(self.bird_y), self.bird_x, bird_char, curses.color_pair(1))
            else:
                self.screen.addstr(int(self.bird_y), self.bird_x, bird_char)
        except curses.error:
            pass
        for obstacle in self.obstacles:
            x = int(obstacle['x'])
            if 0 <= x < self.width:

                for y in range(obstacle['gap_start']):
                    if 0 <= y < self.height:
                        try:
                            char = self.obstacle_top if y == obstacle['gap_start'] - 1 else self.obstacle_char
                            if curses.has_colors():
                                self.screen.addstr(y, x, char, curses.color_pair(2))
                            else:
                                self.screen.addstr(y, x, char)
                        except curses.error:
                            pass

                for y in range(obstacle['gap_end'], self.height):
                    if 0 <= y < self.height:
                        try:
                            char = self.obstacle_bottom if y == obstacle['gap_end'] else self.obstacle_char
                            if curses.has_colors():
                                self.screen.addstr(y, x, char, curses.color_pair(2))
                            else:
                                self.screen.addstr(y, x, char)
                        except curses.error:
                            pass
        score_text = f"Score: {self.score}"
        try:
            if curses.has_colors():
                self.screen.addstr(0, 0, score_text, curses.color_pair(3))
            else:
                self.screen.addstr(0, 0, score_text)
        except curses.error:
            pass
        if self.game_over:
            game_over_text = "GAME OVER - Press 'r' to restart or 'q' to quit"
            x = max(0, (self.width - len(game_over_text)) // 2)
            try:
                if curses.has_colors():
                    self.screen.addstr(self.height // 2, x, game_over_text, curses.color_pair(4))
                else:
                    self.screen.addstr(self.height // 2, x, game_over_text)
            except curses.error:
                pass
        
        self.screen.refresh()



    def run(self):
        while True:
            current_time = time.time()
            dt = current_time - self.last_update
            self.last_update = current_time
            key = self.screen.getch()
            if key == ord('q'):
                break
            elif key == ord('r') and self.game_over:
                self.reset_game()
            elif (key == ord(' ') or key == curses.KEY_UP) and not self.game_over:
                self.flap()
            if not self.game_over:
                self.update_bird(dt)
                self.update_obstacles(dt)
            self.draw()
            self.animation_counter += 1
            time.sleep(max(0.01 - (time.time() - current_time), 0))


def main():
    curses.wrapper(lambda screen: FlappyBird(screen).run())

if __name__ == "__main__":
    main()