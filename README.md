# CLI Flappy Bird

A simple command-line implementation of the classic Flappy Bird game built in multiple programming languages.

## Overview

This project implements the same Flappy Bird game in different programming languages:
- Python
- Rust
- Go
- C
- C++

Each implementation is contained in its own directory named after the language.

## How to Play

- Press **Space** or **Up Arrow** key to make the bird fly
- Press **R** to restart the game
- Press **Q** to quit

## Scoring

The game keeps track of your score as you navigate through obstacles. Try to beat your high score!

## Customization

You can modify the game mechanics by changing the following variables in the source code (variable names may vary between implementations):
- Gravity strength
- Flap power
- Gap size between obstacles

## Directory Structure

```
CLI-FLAPPY/
├── c/
│   └── flappy_bird.c
├── cpp/
│   └── flappy_bird.cpp
├── go/
│   ├── go.mod
│   ├── go.sum
│   └── main.go
├── python/
│   └── flappy_bird.py
├── rust/
│   ├── src/
│   │   └── main.rs
│   ├── target/
│   ├── Cargo.lock
│   ├── Cargo.toml
│   └── .gitignore
└── README.md
```

## Running Instructions

### Python
```bash
cd python
python flappy_bird.py
```

### Rust
```bash
cd rust
cargo run
```

### Go
```bash
cd go
go run main.go
```

### C
```bash
cd c
gcc flappy_bird.c -o flappy_bird
./flappy_bird
```

### C++
```bash
cd cpp
g++ flappy_bird.cpp -o flappy_bird
./flappy_bird
```

## Purpose

This project was created for fun and as a learning exercise to compare implementations of the same game across different programming languages.

## License

Feel free to use, modify, and distribute this code as you wish!