#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define WHITE SSD1306_WHITE
Adafruit_SSD1306 oled(128, 64,&SPI, 4, 6, 12);

int upButton = A0;
int rightButton = A1;
int downButton = A3;
int leftButton = A2;
int okButton = 8;

const int UP = 1;
const int RIGHT = 2;
const int DOWN = 3;
const int LEFT = 4;

//------VARIABLES---------
int score = 0, loadProgress, start, time;
int roundTime = 2000;
bool gameOver = 0, game;
int direction, press, offset;


void setup() {
  Serial.begin(9600);
  if(!oled.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println("SSD1306 allocation failed");
    for(;;); // Don't proceed, loop forever
  }

  pinMode(upButton, INPUT);
  pinMode(leftButton, INPUT);
  pinMode(downButton, INPUT);
  pinMode(rightButton, INPUT);
  
  oled.clearDisplay();
  oled.display();
  delay(100);
  randomSeed(millis());
  oled.setTextColor(WHITE);
  oled.cp437(true);
}


// Function that displays the button the player should press on the screen.

void showDirection() {
  oled.clearDisplay();
  oled.display();
  delay(200);                       // time for releasing the button 
  start = millis();                 // resetting the timer for timer bar
  if (score < 55) roundTime = 5000 - (score * 75);  // timer gets shorter each round 
  oled.setTextSize(3);
  direction = random(1, 5);         
  switch (direction) {
    case UP: 
      oled.setCursor(50,20);        // centering each word (optional)
      oled.write("UP"); 
      break;
    case RIGHT: 
      oled.setCursor(20,20);
      oled.write("RIGHT"); 
      break;
    case DOWN: 
      oled.setCursor(30,20);
      oled.write("DOWN"); 
      break;
    case LEFT: 
      oled.setCursor(30,20);
      oled.write("LEFT"); 
      break;
  }
  oled.display();
}


// Function that displays the timer bar for each round.

void timerBar() {
  oled.drawRect(14, 55, 100, 5, WHITE);    // outer shell of the bar (100px wide)
  time = millis() - start;            // time elapsed since round start
  loadProgress = map(time, 0, roundTime, 0, 100);  
  oled.fillRect(15, 56, loadProgress, 3, WHITE);  // inner shell of the bar 
  if (loadProgress > 98) {
    gameOver = 1;
    while (gameOver) gameOverScreen();   // game is stopped until gameOver is false
    score = 0;                
    startGame();
  }
  oled.display();
}


// Function that checks if the player pressed the right button.

void checkDirection() {
  while (game) {
    timerBar();                 
    if (!digitalRead(upButton)) press = UP;      //checking which button was pressed
    if (!digitalRead(rightButton)) press = RIGHT;
    if (!digitalRead(downButton)) press = DOWN;
    if (!digitalRead(leftButton)) press = LEFT;
    if (press != 0) {             // checking only when the player pressed something
      if (direction == press) {  
        score++;
        game = 0; 
     } else {
      gameOver = 1;
       while (gameOver) gameOverScreen();  // game is stopped until gameOver is false
       score = 0;
       startGame();  
      }
    }
  }
}


// Function that calls all the necessary functions to start and play the game.

void startGame() {
  showDirection();
  press = 0; game = 1; // the player didn't press anything, so "press" is being reset
  checkDirection();
}


// Function that shows the score and checks for reset at the end of the game.

void gameOverScreen() {
  oled.clearDisplay();
  oled.setTextSize(3);
  oled.setTextColor(WHITE);
  oled.setCursor(48,20);
  oled.cp437(true);
  oled.println(score);
  oled.setTextSize(1);
  oled.setCursor(5,50);
  oled.println("Press OK to restart.");
  oled.display();
  if (!digitalRead(okButton)) gameOver = 0;
}

// Function that displays a simple menu (optional)

void menu() {
  oled.setTextSize(2);
  oled.setCursor(10,20);
  oled.write("ReAction");
  oled.setTextSize(1);
  oled.setCursor(5,50);
  oled.println("Press OK to start.");
  oled.display();
}

void loop() {
  menu();
  if (!digitalRead(okButton)) while (1) startGame(); // loop the game if started
}
