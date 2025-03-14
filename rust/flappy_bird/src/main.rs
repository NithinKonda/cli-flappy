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
}



fn main() {

}
