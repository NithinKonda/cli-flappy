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

}