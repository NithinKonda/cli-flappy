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

fn main() {

}
