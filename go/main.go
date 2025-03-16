package main

import (
	"fmt"
	"math/rand"
	"os"
	"time"

	"github.com/gdamore/tcell/v2"
)

type Obstacle struct {
	X        float64
	GapStart int
	GapEnd   int
	Passed   bool
}

type FlappyBird struct {
	screen           tcell.Screen
	width            int
	height           int
	birdX            int
	birdY            float64
	birdVelocity     float64
	birdChars        []rune
	birdFrame        int
	obstacleChar     rune
	obstacleTop      rune
	obstacleBottom   rune
	obstacles        []Obstacle
	gapSize          int
	gravity          float64
	flapPower        float64
	score            int
	gameOver         bool
	lastUpdate       time.Time
	animationCounter int
}

func NewFlappyBird(screen tcell.Screen) *FlappyBird {
	width, height := screen.Size()

	fb := &FlappyBird{
		screen:         screen,
		width:          width,
		height:         height,
		birdX:          width / 4,
		birdY:          float64(height / 2),
		birdChars:      []rune{'>', '^', '>'},
		birdFrame:      0,
		obstacleChar:   '║',
		obstacleTop:    '╦',
		obstacleBottom: '╩',
		gapSize:        6,
		gravity:        0.1,
		flapPower:      -0.7,
		lastUpdate:     time.Now(),
	}

	fb.ResetGame()
	return fb
}

func (fb *FlappyBird) ResetGame() {
	fb.birdY = float64(fb.height / 2)
	fb.birdVelocity = 0
	fb.obstacles = []Obstacle{}
	fb.NewObstacle()
	fb.score = 0
	fb.gameOver = false
	fb.lastUpdate = time.Now()
	fb.animationCounter = 0
}

func (fb *FlappyBird) NewObstacle() {
	gapStart := rand.Intn(fb.height-fb.gapSize-6) + 3

	obstacle := Obstacle{
		X:        float64(fb.width - 1),
		GapStart: gapStart,
		GapEnd:   gapStart + fb.gapSize,
		Passed:   false,
	}

	fb.obstacles = append(fb.obstacles, obstacle)
}

func (fb *FlappyBird) UpdateBird(dt float64) {
	fb.birdVelocity += fb.gravity * dt * 10
	fb.birdY += fb.birdVelocity

	if fb.animationCounter%5 == 0 {
		fb.birdFrame = (fb.birdFrame + 1) % len(fb.birdChars)
	}

	if fb.birdY < 1 {
		fb.birdY = 1
		fb.birdVelocity = 0
	} else if fb.birdY >= float64(fb.height-1) {
		fb.birdY = float64(fb.height - 1)
		fb.gameOver = true
	}
}

func (fb *FlappyBird) UpdateObstacles(dt float64) {
	speed := 15.0 * dt

	for i := range fb.obstacles {
		fb.obstacles[i].X -= speed

		if !fb.obstacles[i].Passed && fb.obstacles[i].X < float64(fb.birdX) {
			fb.score++
			fb.obstacles[i].Passed = true
		}

		if fb.birdX >= int(fb.obstacles[i].X) &&
			fb.birdX <= int(fb.obstacles[i].X)+1 &&
			(int(fb.birdY) < fb.obstacles[i].GapStart || int(fb.birdY) >= fb.obstacles[i].GapEnd) {
			fb.gameOver = true
		}
	}
	var newObstacles []Obstacle
	for _, o := range fb.obstacles {
		if o.X > 0 {
			newObstacles = append(newObstacles, o)
		}
	}
	fb.obstacles = newObstacles

	if len(fb.obstacles) == 0 || fb.obstacles[len(fb.obstacles)-1].X < float64(fb.width-20) {
		fb.NewObstacle()
	}
}

func (fb *FlappyBird) Flap() {
	fb.birdVelocity = fb.flapPower
}

func (fb *FlappyBird) DrawBackground() {
	bgStyle := tcell.StyleDefault.Foreground(tcell.NewRGBColor(0, 255, 255))

	for y := 0; y < fb.height; y++ {
		for x := 0; x < fb.width; x += 4 {
			fb.screen.SetContent(x, y, '·', nil, bgStyle)
		}
	}
}

func (fb *FlappyBird) Draw() {
	fb.screen.Clear()

	fb.DrawBackground()

	birdChar := fb.birdChars[fb.birdFrame]
	birdStyle := tcell.StyleDefault.Foreground(tcell.ColorYellow)
	fb.screen.SetContent(fb.birdX, int(fb.birdY), birdChar, nil, birdStyle)

	obstacleStyle := tcell.StyleDefault.Foreground(tcell.ColorGreen)

	for _, obstacle := range fb.obstacles {
		x := int(obstacle.X)
		if x >= 0 && x < fb.width {
			// Draw top part
			for y := 0; y < obstacle.GapStart; y++ {
				char := fb.obstacleChar
				if y == obstacle.GapStart-1 {
					char = fb.obstacleTop
				}
				fb.screen.SetContent(x, y, char, nil, obstacleStyle)
			}

			for y := obstacle.GapEnd; y < fb.height; y++ {
				char := fb.obstacleChar
				if y == obstacle.GapEnd {
					char = fb.obstacleBottom
				}
				fb.screen.SetContent(x, y, char, nil, obstacleStyle)
			}
		}
	}

	scoreText := fmt.Sprintf("Score: %d", fb.score)
	scoreStyle := tcell.StyleDefault.Foreground(tcell.ColorWhite)

	for i, r := range scoreText {
		fb.screen.SetContent(i, 0, r, nil, scoreStyle)
	}

	if fb.gameOver {
		gameOverText := "GAME OVER - Press 'r' to restart or 'q' to quit"
		gameOverStyle := tcell.StyleDefault.Foreground(tcell.ColorRed)

		x := (fb.width - len(gameOverText)) / 2
		if x < 0 {
			x = 0
		}

		for i, r := range gameOverText {
			if x+i < fb.width {
				fb.screen.SetContent(x+i, fb.height/2, r, nil, gameOverStyle)
			}
		}
	}

	fb.screen.Show()
}

func (fb *FlappyBird) Run() {

	eventChan := make(chan tcell.Event)
	go func() {
		for {
			eventChan <- fb.screen.PollEvent()
		}
	}()

	for {
		currentTime := time.Now()
		dt := currentTime.Sub(fb.lastUpdate).Seconds()
		fb.lastUpdate = currentTime

		select {
		case ev := <-eventChan:
			switch ev := ev.(type) {
			case *tcell.EventKey:
				switch ev.Key() {
				case tcell.KeyEscape, tcell.KeyCtrlC:
					return
				case tcell.KeyRune:
					switch ev.Rune() {
					case 'q':
						return
					case 'r':
						if fb.gameOver {
							fb.ResetGame()
						}
					case ' ':
						if !fb.gameOver {
							fb.Flap()
						}
					}
				case tcell.KeyUp:
					if !fb.gameOver {
						fb.Flap()
					}
				}
			case *tcell.EventResize:
				fb.width, fb.height = fb.screen.Size()
			}
		default:

		}

		if !fb.gameOver {
			fb.UpdateBird(dt)
			fb.UpdateObstacles(dt)
		}

		fb.Draw()

		fb.animationCounter++

		sleep := time.Millisecond*10 - time.Since(currentTime)
		if sleep > 0 {
			time.Sleep(sleep)
		}
	}
}

func main() {

	rand.Seed(time.Now().UnixNano())

	screen, err := tcell.NewScreen()
	if err != nil {
		fmt.Fprintf(os.Stderr, "Error creating screen: %v\n", err)
		os.Exit(1)
	}

	if err := screen.Init(); err != nil {
		fmt.Fprintf(os.Stderr, "Error initializing screen: %v\n", err)
		os.Exit(1)
	}

	screen.SetStyle(tcell.StyleDefault.Background(tcell.ColorBlack).Foreground(tcell.ColorWhite))
	screen.Clear()

	game := NewFlappyBird(screen)

	defer func() {
		screen.Fini()
	}()

	game.Run()
}
