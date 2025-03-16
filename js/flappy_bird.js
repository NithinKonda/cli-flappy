const blessed = require('blessed');


const screen = blessed.screen({
  smartCSR: true,
  title: 'Flappy Bird CLI'
});


class FlappyBird {
    constructor(screen) {
      this.screen = screen;
      this.width = screen.width;
      this.height = screen.height;
      
      // Game settings
      this.birdX = Math.floor(this.width / 4);
      this.birdChars = ['>', '^', '>'];
      this.birdFrame = 0;
      this.obstacleChar = '║';
      this.obstacleTop = '╦';
      this.obstacleBottom = '╩';
      this.gapSize = 6;
      this.gravity = 0.05;
      this.flapPower = -0.3;
      
      this.setupInput();
      

      this.resetGame();
    }


    setupInput() {

        this.screen.key(['escape', 'q', 'C-c'], () => {
          process.exit(0);
        });
        

        this.screen.key([' ', 'up'], () => {
          if (!this.gameOver) {
            this.flap();
          }
        });
        

        this.screen.key(['r'], () => {
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
        const gapStart = 3 + Math.floor(Math.random() * (this.height - this.gapSize - 6));
        
        this.obstacles.push({
          x: this.width - 1,
          gapStart: gapStart,
          gapEnd: gapStart + this.gapSize,
          passed: false
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
}