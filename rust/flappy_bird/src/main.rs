use crossterm::{
    cursor,
    event::{self, Event, KeyCode, KeyEvent},
    execute,
    style::{self, Color, Stylize},
    terminal::{self, ClearType},
};
use rand::Rng;
use std::{
    io::{self, Write},
    time::{Duration, Instant},
};



struct FlappyBird {
    width: u16,
    height: u16,
    bird_x: u16,
    bird_y: f32,
    bird_velocity: f32,
    bird_chars: Vec<&'static str>,
    bird_frame: usize,
    obstacle_char: &'static str,
    obstacle_top: &'static str,
    obstacle_bottom: &'static str,
    obstacles: Vec<Obstacle>,
    gap_size: u16,
    gravity: f32,
    flap_power: f32,
    score: u32,
    game_over: bool,
    animation_counter: u32,
}


struct Obstacle {
    x: f32,
    gap_start: u16,
    gap_end: u16,
    passed: bool,
}


impl FlappyBird {
    fn new() -> io::Result<Self> {
        let (width, height) = terminal::size()?;
        
        Ok(Self {
            width,
            height,
            bird_x: width / 4,
            bird_y: height as f32 / 2.0,
            bird_velocity: 0.0,
            bird_chars: vec![">", "^", ">"],
            bird_frame: 0,
            obstacle_char: "║",
            obstacle_top: "╦",
            obstacle_bottom: "╩",
            obstacles: Vec::new(),
            gap_size: 6,
            gravity: 0.05,
            flap_power: -0.3,
            score: 0,
            game_over: false,
            animation_counter: 0,
        })
    }



    fn reset_game(&mut self) {
        self.bird_y = self.height as f32 / 2.0;
        self.bird_velocity = 0.0;
        self.obstacles.clear();
        self.new_obstacle();
        self.score = 0;
        self.game_over = false;
        self.animation_counter = 0;
    }

    fn new_obstacle(&mut self) {
        let mut rng = rand::thread_rng();
        let gap_start = rng.gen_range(3..self.height - self.gap_size - 3);
        
        self.obstacles.push(Obstacle {
            x: self.width as f32 - 1.0,
            gap_start,
            gap_end: gap_start + self.gap_size,
            passed: false,
        });
    }



    fn update_bird(&mut self, dt: f32) {
        self.bird_velocity += self.gravity * dt * 10.0;
        self.bird_y += self.bird_velocity;

        if self.animation_counter % 5 == 0 {
            self.bird_frame = (self.bird_frame + 1) % self.bird_chars.len();
        }

        if self.bird_y < 1.0 {
            self.bird_y = 1.0;
            self.bird_velocity = 0.0;
        } else if self.bird_y >= self.height as f32 - 1.0 {
            self.bird_y = self.height as f32 - 1.0;
            self.game_over = true;
        }
    }

    fn update_obstacles(&mut self, dt: f32) {
        let speed = 15.0 * dt;

        for obstacle in &mut self.obstacles {
            obstacle.x -= speed;

            if !obstacle.passed && obstacle.x < self.bird_x as f32 {
                self.score += 1;
                obstacle.passed = true;
            }

            if self.bird_x >= obstacle.x as u16
                && self.bird_x <= obstacle.x as u16 + 1
                && (self.bird_y as u16 < obstacle.gap_start || self.bird_y as u16 >= obstacle.gap_end)
            {
                self.game_over = true;
            }
        }

        self.obstacles.retain(|o| o.x > 0.0);

        if self.obstacles.is_empty() || self.obstacles.last().unwrap().x < self.width as f32 - 20.0 {
            self.new_obstacle();
        }
    }


    fn flap(&mut self) {
        self.bird_velocity = self.flap_power;
    }

    fn draw(&self) -> io::Result<()> {
        execute!(io::stdout(), terminal::Clear(ClearType::All))?;

        // Draw background dots
        for y in 0..self.height {
            for x in (0..self.width).step_by(4) {
                if let Ok(_) = execute!(
                    io::stdout(),
                    cursor::MoveTo(x, y),
                    style::PrintStyledContent("·".with(Color::Cyan))
                ) {}
            }
        }

        
}



fn main() {

}
