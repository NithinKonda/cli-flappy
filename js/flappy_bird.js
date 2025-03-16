const blessed = require("blessed");

const screen = blessed.screen({
  smartCSR: true,
  title: "Flappy Bird CLI",
});

class FlappyBird {
  constructor(screen) {
    this.screen = screen;
    this.width = screen.width;
    this.height = screen.height;
    this.birdX = Math.floor(this.width / 4);
    this.birdChars = [">", "^", ">"];
    this.birdFrame = 0;
    this.obstacleChar = "║";
    this.obstacleTop = "╦";
    this.obstacleBottom = "╩";
    this.gapSize = 6;
    this.gravity = 0.07;
    this.flapPower = -0.2;
    this.setupInput();
    this.resetGame();
  }

  setupInput() {
    this.screen.key(["escape", "q", "C-c"], () => {
      process.exit(0);
    });
    this.screen.key([" ", "up"], () => {
      if (!this.gameOver) {
        this.flap();
      }
    });
    this.screen.key(["r"], () => {
      if (this.gameOver) {
        this.resetGame();
      }
    });
  }

  resetGame() {
    this.birdY = Math.floor(this.height / 2);
    this.birdVelocity = 0;
    this.obstacles = [];
    this.newObstacle();
    this.score = 0;
    this.gameOver = false;
    this.lastUpdate = Date.now();
    this.animationCounter = 0;
  }

  newObstacle() {
    const gapStart =
      3 + Math.floor(Math.random() * (this.height - this.gapSize - 6));
    this.obstacles.push({
      x: this.width - 1,
      gapStart: gapStart,
      gapEnd: gapStart + this.gapSize,
      passed: false,
    });
  }

  flap() {
    this.birdVelocity = this.flapPower;
  }

  updateBird(dt) {
    this.birdVelocity += this.gravity * dt * 10;
    this.birdY += this.birdVelocity;
    if (this.animationCounter % 5 === 0) {
      this.birdFrame = (this.birdFrame + 1) % this.birdChars.length;
    }
    if (this.birdY < 1) {
      this.birdY = 1;
      this.birdVelocity = 0;
    } else if (this.birdY >= this.height - 1) {
      this.birdY = this.height - 1;
      this.gameOver = true;
    }
  }

  updateObstacles(dt) {
    const speed = 15 * dt;
    for (const obstacle of this.obstacles) {
      obstacle.x -= speed;
      if (!obstacle.passed && obstacle.x < this.birdX) {
        this.score++;
        obstacle.passed = true;
      }
      if (
        this.birdX >= Math.floor(obstacle.x) &&
        this.birdX <= Math.floor(obstacle.x) + 1 &&
        (Math.floor(this.birdY) < obstacle.gapStart ||
          Math.floor(this.birdY) >= obstacle.gapEnd)
      ) {
        this.gameOver = true;
      }
    }
    this.obstacles = this.obstacles.filter((o) => o.x > 0);
    if (
      this.obstacles.length === 0 ||
      this.obstacles[this.obstacles.length - 1].x < this.width - 20
    ) {
      this.newObstacle();
    }
  }

  drawBackground() {
    for (let y = 0; y < this.height; y++) {
      for (let x = 0; x < this.width; x += 4) {
        if (x < this.width && y < this.height) {
          this.screen.fillRegion(
            this.getColorAttrs({ fg: "cyan" }),
            "·",
            x,
            x + 1,
            y,
            y + 1
          );
        }
      }
    }
  }

  getColorAttrs(attrs = {}) {
    return Object.assign(
      {
        bg: "black",
      },
      attrs
    );
  }

  draw() {
    this.screen.clearRegion(0, this.width, 0, this.height);
    this.drawBackground();
    const birdChar = this.birdChars[this.birdFrame];
    const birdY = Math.floor(this.birdY);
    if (
      birdY >= 0 &&
      birdY < this.height &&
      this.birdX >= 0 &&
      this.birdX < this.width
    ) {
      this.screen.fillRegion(
        this.getColorAttrs({ fg: "yellow" }),
        birdChar,
        this.birdX,
        this.birdX + 1,
        birdY,
        birdY + 1
      );
    }
    for (const obstacle of this.obstacles) {
      const x = Math.floor(obstacle.x);
      if (x >= 0 && x < this.width) {
        for (let y = 0; y < obstacle.gapStart; y++) {
          if (y >= 0 && y < this.height) {
            const char =
              y === obstacle.gapStart - 1
                ? this.obstacleTop
                : this.obstacleChar;
            this.screen.fillRegion(
              this.getColorAttrs({ fg: "green" }),
              char,
              x,
              x + 1,
              y,
              y + 1
            );
          }
        }
        for (let y = obstacle.gapEnd; y < this.height; y++) {
          if (y >= 0 && y < this.height) {
            const char =
              y === obstacle.gapEnd ? this.obstacleBottom : this.obstacleChar;
            this.screen.fillRegion(
              this.getColorAttrs({ fg: "green" }),
              char,
              x,
              x + 1,
              y,
              y + 1
            );
          }
        }
      }
    }
    const scoreText = `Score: ${this.score}`;
    for (let i = 0; i < scoreText.length; i++) {
      if (i < this.width) {
        this.screen.fillRegion(
          this.getColorAttrs({ fg: "white" }),
          scoreText[i],
          i,
          i + 1,
          0,
          1
        );
      }
    }
    this.screen.render();
  }

  run() {
    const gameLoop = () => {
      const currentTime = Date.now();
      const dt = (currentTime - this.lastUpdate) / 1000;
      this.lastUpdate = currentTime;
      if (!this.gameOver) {
        this.updateBird(dt);
        this.updateObstacles(dt);
      }
      this.draw();
      this.animationCounter++;
      setTimeout(gameLoop, 10);
    };
    gameLoop();
  }
}

const game = new FlappyBird(screen);
game.run();