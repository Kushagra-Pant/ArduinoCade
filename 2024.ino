#include "LedControl.h"
#include "EEPROM.h"     // EEprom management include file


LedControl lc = LedControl(5, 3, 4, 2); // Set up a LeadControl object, which controls the LED Matrix. Connect DIN to Arduino pin 5, CLK to pin 3, and CS to pin 4. There are 2 LED Matrices, the ID of the first one is 0 and the ID of the second is 1.


// Below are the variables used in both games
const byte btnLeft = 12; // The button labeled ◀ connects to input 12 on the Arduino board
const byte btnRight = 11; // The button labeled ▶ connects to input 11 on the Arduino board
const byte SELECT = 10; // The button labeled ▲ connects to input 10 on the Arduino board
int game = 0; // An integer that represents which of the 2 games are being selected during the home screen stage
const int varXPin = A0; // X Value from Joystick
const int varYPin = A1; // Y Value from Joystick
bool run = true; // If false, this ends the game running and sets the screen back to home


// Below is the logo for Slither
byte screen1[] = {
  B00000000,
  B01111110,
  B00000010,
  B00111010,
  B00101010,
  B00100010,
  B00111110,
  B00000000,
};


// Below is the logo for Dot Defenders
byte screen2[] = {
  B00000000,
  B00000000,
  B00000010,
  B01000110,
  B00100110,
  B00000010,
  B00000000,
  B00000000,
};


// Slither variable initialization
float oldTime = 0;
float timer = 0;
float updateRate = 3;
byte pic[8] = {0, 0, 0, 0, 0, 0, 0, 0}; // The 8 rows of the LED Matrix in Slither


// Dot Defenders variable initialization
long Score = 0;          // Global Variable for Actual Score
long HighScore;          // Global Variable for HiScore
long AlienSpeed = 80000; // Global Variable for Aliens speed
long AlienLevel = 0;     // Global Variable for Aliens speed on the level
long AlienMul = 5000;    // Global Variable for step speed increasing when shooting alien
long AlienDelay = 0;     // Global Variable for Aliens Delay
char AllKilled = 0;      // Global Variable that count active alien on screen
long HiscoreDelay =0;    // Global Variable used to flip score with hiscore
long CannonDelay = 0;    // Global Variable for Cannon Delay
byte CannonPos = 8;      // Global Variable for Cannon position
long Explode = 0;        // Global Variable for Explosion Marker
byte Ex = 0;             // Global Variable for Explosion X
byte Ey = 0;             // Global Variable for Explosion Y
byte AlienXo = 16;       // Global Variable for Alien X previous (to remove aliens before redraw new position)
byte AlienYo = 0;        // Global Variable for Alien Y previous (to remove aliens before redraw new position)
byte AlienX = 16;        // Global Variable for Alien X
byte AlienY = 0;         // Global Variable for Alien Y
byte MaxAlienX = 0;      // Global Variable for Maximum Alien X position
byte MaxAlienY = 0;      // Global Variable for Maximum Alien Y position
byte MinAlienX = 0;      // Global Variable for Minimum Alien X position
boolean AlienXs = false; // Global Variable for define alien direction march
long FireDelay = 0;      // Global Variable for Missile Delay
byte FireXPos = 8;       // Global Variable for Missile X
byte FireYPos = 1;       // Global Variable for Missile Y




const long CannonSpeed = 7000;    // Cannon speed
const long FireSpeed = 5000;      // Missile shooted speed
const byte FirstMax = 0;          // Address of First DotMatrix display
const byte SecondMax = 1;         // Address of Second DotMatrix display
const long ExplodeSpeed = 3000;   // Explosions Speed
const long HiscoreSpeed = 100000; // Speed of flip from score to hiscore
const byte MaxAliens = 16;        //  Number of aliens on screen (see following array)


// Arrays for define X, Y and status (on off) of alien pattern
byte AliensY[MaxAliens] = {       // Y coordinate of lighted led
  0,0,0,0, // row 0
  1,1,1,1, // row 1
  2,2,2,2, // row 2
  3,3,3,3, // row 3
  };
 
byte AliensX[MaxAliens] = {       // X coordinate of lighted led
  0,3,6,9,  // row 0
  1,4,7,10, // row 1
  2,5,8,11, // row 2
  0,3,6,9,  // row 3
  };
 
 boolean AliensStat[MaxAliens] = {// status (true=ON    false=OFF)
  true,true,true,true, // row 0
  true,true,true,true, // row 1
  true,true,true,true, // row 2
  true,true,true,true, // row 3
  };


// define a graphic array for draw number on dot matrix used with pointer multiplied by 5
byte Numbers[] =
{
   B00000111,  // Number 0
   B00000101,
   B00000101,
   B00000101,
   B00000111,
   B00000010,  // Number 1
   B00000010,
   B00000010,
   B00000010,
   B00000010,
   B00000111,  // Number 2
   B00000100,
   B00000111,
   B00000001,
   B00000111,
   B00000111,  // Number 3
   B00000100,
   B00000110,
   B00000100,
   B00000111,
   B00000101,  // Number 4
   B00000101,
   B00000111,
   B00000100,
   B00000100,
   B00000111,  // Number 5
   B00000001,
   B00000111,
   B00000100,
   B00000111,
   B00000111,  // Number 6
   B00000001,
   B00000111,
   B00000101,
   B00000111,
   B00000111,  // Number 7
   B00000100,
   B00000100,
   B00000100,
   B00000100,
   B00000111,  // Number 8
   B00000101,
   B00000111,
   B00000101,
   B00000111,
   B00000111,  // Number 9
   B00000101,
   B00000111,
   B00000100,
   B00000111,
};


// Array space needed to be filled with graphic data for print the score
byte Score1[] =
{
   B00000000,  // Score 1
   B00000000,
   B00000000,
   B00000000,
   B00000000,
   B00000000,
   B00000000,
   B00000000,
};


byte Score2[] =
{
   B00000000,  // Score 2
   B00000000,
   B00000000,
   B00000000,
   B00000000,
   B00000000,
   B00000000,
   B00000000,
};


// Game Over Alien animation graphics array
byte invader1[] =
{
  B00011000,  // Invader 1
  B00111100,
  B01111110,
  B11011011,
  B11111111,
  B00100100,
  B01011010,
  B10100101
};
 
byte invader2[] =
{
  B00011000, // Invader 2
  B00111100,
  B01011010,
  B11111111,
  B11111111,
  B00100100,
  B01011010,
  B01000010
};


// Main Screen animation graphics array
byte SH1[] =
{
  B00100000,  // Main Screen 1
  B01000000,
  B11110011,
  B10111001,
  B11111011,
  B11101010,
  B00101011,
  B01100000,
};


byte SH2[] =
{
  B00000010,  // Main Screen 2
  B10100001,
  B10100111,
  B11101110,
  B10101111,
  B10101011,
  B00001010,
  B00000011,
};


byte SH3[] =
{
  B00110000,  // Main Screen 3
  B01000011,
  B11111001,
  B11111011,
  B10111010,
  B11101011,
  B00100000,
  B00110000,
};


byte SH4[] =
{
  B00000110,  // Main Screen 4
  B00000001,
  B10101111,
  B10101111,
  B11101110,
  B10101011,
  B10100010,
  B00000110,
};


// Constructors (both for Slither)
typedef struct Snake {		  // Define the Snake as a Struct
  int head[2];     // the (row, column) of the snake head
  int body[40][2]; // An array that contains the (row, column) coordinates
  int len;         // The length of the snake
  int dir[2];      // A direction to move the snake along
} Snake;


typedef struct Apple {		  // Define the Apple as a Struct
  int rPos; // The row index of the apple
  int cPos; // The column index of the apple
} Apple;


// Snake and Apple objects created
Snake snake;
Apple apple;


// Displays both the Slither and Dot Defenders screens (homepage)
void homeScreen(){
  for (int i = 0; i < 8; i++) {
    lc.setRow(0, i, screen1[i]);
    lc.setRow(1, i, screen2[i]);
  }
}


void setup() {
  //Set the buttons and joysticks as their respective input
  pinMode(btnLeft, INPUT_PULLUP);  
  pinMode(btnRight, INPUT_PULLUP); 
  pinMode(SELECT, INPUT_PULLUP);
  pinMode(varXPin, INPUT);
  pinMode(varYPin, INPUT);


  Serial.begin(9600); // Allow the Arduino to communicate with the computer


  // Turn on the LED Matrices and set their brightness to max
  lc.shutdown(0, false);
  lc.setIntensity(0, 8);
  lc.shutdown(1, false);
  lc.setIntensity(1, 8);


  //Call the homeScreen() function, which sets the lights on the LED Matrix to the homescreen
  homeScreen();


  delay(1000); // Wait 1 second


  // Initialize random seed
  randomSeed(analogRead(0));
}


void loop() {
  //Create 3 local variables to read the button values. Each time the loop runs, the values of these variables change accordingly.
  int left = digitalRead(btnLeft);
  int right = digitalRead(btnRight);
  int select = digitalRead(SELECT);


  // Check if btnLeft is clicked
  if (left == LOW) {
    //Create a border around the left Matrix, and remove the border from the right matrix
    lc.setRow(0, 0, B11111111);
    lc.setRow(0, 7, B11111111);
    lc.setColumn(0, 0, B11111111);
    lc.setColumn(0, 7, B11111111);
    lc.setRow(1, 0, B00000000);
    lc.setRow(1, 7, B00000000);
    lc.setColumn(1, 0, B00000000);
    lc.setColumn(1, 7, B00000000);


    game = -1;  // Set game to -1; that is the value for Slither
    delay(200); // Delay to debounce and avoid rapid changes
  }


  // Check if btnRight is clicked
  if (right == LOW) {
    //Create a border around the right Matrix, and remove the border from the left matrix
    lc.setRow(1, 0, B11111111);
    lc.setRow(1, 7, B11111111);
    lc.setColumn(1, 0, B11111111);
    lc.setColumn(1, 7, B11111111);
    lc.setRow(0, 0, B00000000);
    lc.setRow(0, 7, B00000000);
    lc.setColumn(0, 0, B00000000);
    lc.setColumn(0, 7, B00000000);


    game = 1;   // Set game to 1; that is the value for Dot Defenders
    delay(200); // Delay to debounce and avoid rapid changes
  }


  // Check if the select button is clicked
  if (select == LOW) {
    if (game == 1) {
      // Clear the display, then play Dot Defenders. When Dot Defenders is over, set the screen back to the home screen.
      lc.clearDisplay(0);
      DotDefenders();
      homeScreen();
    }
    if (game == -1) {
      // Clear the display, then play Slither. When Slither is over, set the screen back to the home screen.
      lc.clearDisplay(0);
      SnakeGame();
      homeScreen();
    }
  }


  // Delay to avoid fast looping
  delay(100);
}


void SnakeGame() {
  run = true;
  // Initialize a snake object
  snake = {{1, 5}, {{0, 5}, {1, 5}}, 2, {1, 0}};
  // Initialize an apple object
  apple = {(int)random(0, 8), (int)random(0, 8)};




  while (run) {
    // put your main code here, to run repeatedly:
    float deltaTime = calculateDeltaTime();
    timer += deltaTime;




    // Check For Inputs
    int xVal = analogRead(varXPin);
    int yVal = analogRead(varYPin);


    if (xVal < 100 && snake.dir[1] == 0) {
      snake.dir[0] = 0;
      snake.dir[1] = -1;
    } else if (xVal > 920 && snake.dir[1] == 0) {
      snake.dir[0] = 0;
      snake.dir[1] = 1;
    } else if (yVal < 100 && snake.dir[0] == 0) {
      snake.dir[0] = -1;
      snake.dir[1] = 0;
    } else if (yVal > 920 && snake.dir[0] == 0) {
      snake.dir[0] = 1;
      snake.dir[1] = 0;
    }


    // Update
    if (timer > 1000 / updateRate) {
      timer = 0;
      Update();
    }


    // Render
    Render();
  }
}


float calculateDeltaTime() {
  float currentTime = millis();
  float dt = currentTime - oldTime;
  oldTime = currentTime;
  return dt;
}


void reset() {
  for (int j = 0; j < 8; j++) {
    pic[j] = 0;
  }
}


void Update() {
  reset(); // Reset (Clear) the 8x8 LED matrix
  int newHead[2] = {snake.head[0] + snake.dir[0], snake.head[1] + snake.dir[1]};


  // Handle Borders
  if (newHead[0] == 8) {
    newHead[0] = 0;
  } else if (newHead[0] == -1) {
    newHead[0] = 7;
  } else if (newHead[1] == 8) {
    newHead[1] = 0;
  } else if (newHead[1] == -1) {
    newHead[1] = 7;
  }


  // Check If The Snake hits itself
  for (int j = 0; j < snake.len; j++) {
    if (snake.body[j][0] == newHead[0] && snake.body[j][1] == newHead[1]) {
      // Pause the game for 1 sec then Reset it
      delay(1000);
      run = false;
      return;
    }
  }


  // Check if The snake ate the apple
  if (newHead[0] == apple.rPos && newHead[1] == apple.cPos) {
    snake.len++;
    apple.rPos = (int)random(0, 8);
    apple.cPos = (int)random(0, 8);
  } else {
    removeFirst(); // Shifting the array to the left
  }




  snake.body[snake.len - 1][0] = newHead[0];
  snake.body[snake.len - 1][1] = newHead[1];


  snake.head[0] = newHead[0];
  snake.head[1] = newHead[1];


  // Update the pic Array to Display(snake and apple)
  for (int j = 0; j < snake.len; j++) {
    pic[snake.body[j][0]] |= 128 >> snake.body[j][1];
  }
  pic[apple.rPos] |= 128 >> apple.cPos;
}


void Render() {
  for (int i = 0; i < 8; i++) {
    lc.setRow(0, i, pic[i]);
  }
}


void removeFirst() {
  for (int j = 1; j < snake.len; j++) {
    snake.body[j - 1][0] = snake.body[j][0];
    snake.body[j - 1][1] = snake.body[j][1];
  }
}




void DotDefenders() {
  run = true;
  Serial.println("Dot Defenders");                // Debug string


  lc.shutdown(FirstMax,false);                    // turn off power saving, enables display
  lc.setIntensity(FirstMax,8);                    // sets brightness (0~15 possible values)
  lc.clearDisplay(FirstMax);                      // clear screen
  lc.shutdown(SecondMax,false);                   // turn off power saving, enables display
  lc.setIntensity(SecondMax,8);                   // sets brightness (0~15 possible values)
  lc.clearDisplay(SecondMax);                     // clear screen




  MainScreen();                                   // Print Main Screen
  GameInit();                                     // Initialize Variables    
  HighScore=EEPROMReadlong(0);                    // Read Hiscore From EEprom
  PrintCannon(CannonPos);                         // Print initial cannon on screen
  PrintAliens(AlienX,AlienY,AlienXo,AlienYo);     // Print initial aliens


while (run)	 // the game loop
{
  if (digitalRead(btnRight) == false) {                  // If Right button pressed
    if (CannonDelay==0) {                                // check cannon delay counter to decide if cannon need be moved
      CannonPos++;                                       // move the cannon
      if (CannonPos>16) CannonPos = 16;                  // check if position is outside display (right)
      PrintCannon(CannonPos);                            // draw cannon in updated position
    }
    CannonDelay++;                                       // if button continue to be pressed increase cannon delay counter
    if (CannonDelay > CannonSpeed) CannonDelay = 0 ;     // if cannon delay reach cannon speed value, clear it to allow to move cannon again
  } else if (digitalRead(btnLeft) == false) {            // If Left button pressed
    if (CannonDelay==0) {                                // check cannon delay counter to decide if cannon need be moved
      CannonPos--;                                       // move the cannon
      if (CannonPos < 1) CannonPos = 1;                  // check if position is outside display (right)
      PrintCannon(CannonPos);                            // draw cannon in updated position
    }
    CannonDelay++;                                       // if button continue to be pressed increase cannon delay counter
    if (CannonDelay > CannonSpeed) CannonDelay = 0 ;     // if cannon delay reach cannon speed value, clear it to allow to move cannon again
  } else {                                               // If left and right are released
    CannonDelay=0;
  }




  if (digitalRead(SELECT) == false) {                   // Press fire button
    if (FireYPos==0) {                                   // check if fire is already shooted
      FireXPos = CannonPos-1;                            // if not shooted, assign currect cannon X position to missile
      FireYPos = 7;                                      // set Missile Y position (Y>1 = shooting in progress)  
    }
  }


// explosion management
  if (Explode != 0) {                                   // Check if active Explosion to Ex and Ey Position
    Explode++;
    if (Explode > ExplodeSpeed * 5) {
     SetLed(Ex,Ey,false);                               // Clear explosion
     Explode =0;
    } else if (Explode == (ExplodeSpeed * 4)) {
     SetLed(Ex,Ey,true);                                // Set explosion
    } else if (Explode == (ExplodeSpeed * 3)) {
     SetLed(Ex,Ey,false);                               // Clear explosion
    } else if (Explode == (ExplodeSpeed * 2)) {
     SetLed(Ex,Ey,true);                                // Set explosion
    } else if (Explode == ExplodeSpeed) {
     SetLed(Ex,Ey,false);                               // Clear explosion
    }
  }


// move aliens with typical pattern
  if (AlienDelay > AlienSpeed) {                       // speed checking function                
    AlienDelay = 0;
    if (AlienXs == false) {                            // check direction
      AlienX++;                                        // direction R
    } else {
      AlienX--;                                        // direction L
    }


    if (MaxAlienX+AlienX > 31) {                       // if last alien on screen reach right edge of screen, invert direction and step down  
      AlienXs = true;                                  // invert direction
      AlienX--;                                        
      AlienY++;                                        // scrolling down aliens
    } else if (MinAlienX+AlienX < 16) {                // if last alien on screen reach left edge of screen, invert direction and step down
      AlienXs = false;                                 // invert direction
      AlienX++;
      AlienY++;                                        // scrolling down aliens
    }


    if (MaxAlienY+AlienY >= 6) {                       // If aliens reached cannon GAME OVER
      GameOver();                                      // call gameover function
run = false;    
    }
   
    PrintAliens(AlienX, AlienY, AlienXo, AlienYo);     // draw updated aliens on screen


    AlienXo = AlienX;                                   // store previous alien position offset, used to clear it when moving
    AlienYo = AlienY;
  } else {
    AlienDelay++;
  }
 
// missile management
  if (FireYPos != 0) {                                   // Fire active and check for collisions
    if (FireDelay==0) {                                  // Wait until firedelay was 0 to do next fire step
      FireYPos--;                                        // decrease missile position
      if (FireYPos < 1) {                                // if missile reached top
        SetLed(FireXPos,FireYPos,false);                 // Clear missile shadow when reach top
        FireYPos = 0;
      }
      SetLed(FireXPos,FireYPos-1,true);                  // Draw missile


      for (int nal = 0; nal < MaxAliens; nal++) {        // Check for missile collided Alien
        if ((((AliensX[nal]+AlienX)-16) == FireXPos) && (AliensY[nal]+AlienY == FireYPos-1) && (AliensStat[nal] == true)){
          SetLed(FireXPos,FireYPos,false);               // clear Alien fired
          Ex=FireXPos;                                   // Set X of explosion
          Ey=FireYPos-1;                                 // Set y of explosion
          Explode = 1;                                   // Activate Explosion
          Score += (6-FireYPos)*4;                       // Increase score
          if (Score >= 9999) Score = 9999;               // Set 9999 limit to High Score due limitation to 4 digit
          if (Score >= HighScore) HighScore = Score;     // Set HiScore
          EEPROMWritelong(0, HighScore);                 // Write HighScore on EEprom
          Serial.print ("Score=");
          Serial.println (Score);
          FireYPos = 0;                                  // Reset Fire status  
          AliensStat[nal] = false;                       // Set alien as fired in the array
          CalcMaxAlien();                                // Calculate new alien edges on screen
          AlienSpeed = (AllKilled * AlienMul) + AlienMul;// set alien speed
        }      
      }


      if ((FireYPos > 0) && (FireYPos < 6)) {
        SetLed(FireXPos,FireYPos,false);              // clear missile shadow
      }
    }
    FireDelay++;                                          // increase delay variable
    if (FireDelay > FireSpeed) FireDelay = 0 ;           // check if fredelay reached firespeed setting
  }
}
}


// read number (highscore) from eeprom
long EEPROMReadlong(long address) {
  long four = EEPROM.read(address);
  long three = EEPROM.read(address + 1);
  long two = EEPROM.read(address + 2);
  long one = EEPROM.read(address + 3);
  return ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);
}




// write number (highscore) to eeprom
void EEPROMWritelong(int address, long value) {
  byte four = (value & 0xFF);
  byte three = ((value >> 8) & 0xFF);
  byte two = ((value >> 16) & 0xFF);
  byte one = ((value >> 24) & 0xFF);
 
  EEPROM.write(address, four);
  EEPROM.write(address + 1, three);
  EEPROM.write(address + 2, two);
  EEPROM.write(address + 3, one);
}


// Show main screen animation (Press fire to start)
void MainScreen() {
long MainAni = 0;
  while (digitalRead(SELECT) == true) {
    MainAni++;
    if (MainAni == 30000) {
      for (int i = 0; i < 10; i++) {
        lc.setColumn(FirstMax,i,SH1[i]);
        lc.setColumn(SecondMax,i,SH2[i]);
      }
    } else if (MainAni == 60000) {
      for (int i = 0; i < 10; i++) {
        lc.setColumn(FirstMax,i,SH3[i]);
        lc.setColumn(SecondMax,i,SH4[i]);
      }
      MainAni=0;
    }    
  }
  while (digitalRead(SELECT) == false);                    // wait release fire key to continue
}


// Calculate max/min of X & Y coordinates of active aliens on screen, and detect if all aliens are killed
// In this function I add an offset of 16 to fix a negative calculation, next when this result is used.
void CalcMaxAlien() {
  MaxAlienX=0;
  MaxAlienY=0;
  MinAlienX=999;
  AllKilled=0;
  for (int nal = 0; nal < MaxAliens; nal++) {                      // circle on all aliens in array
    if (AliensStat[nal] == true) {                                 // if alien is enabled, check coordinates
      AllKilled++;
      if (AliensX[nal] >= MaxAlienX) MaxAlienX = AliensX[nal];     // set new maximum X  
      if (AliensY[nal] >= MaxAlienY) MaxAlienY = AliensY[nal];     // set new maximum Y  
      if (MinAlienX >= AliensX[nal]) MinAlienX = AliensX[nal];     // set new minimum X  
    }
  }
  if (AllKilled==0) {                                              // if all alien are disabled mean you killed all aliens !!!
    PrintCannon(CannonPos);                                        // Print cannon on screen to refresh at last kill
    NextLevel();                                                   // go to next level
  }
}


// Play WIN melody and set parameters for next level
void NextLevel() {
  delay(500);
  // PlayMelody();                                                   // play WIN melody  
  AlienLevel+=500;                                                // increase alien speed
  Explode =ExplodeSpeed*4;                                        // set variable to clear last explosion starting new level
  GameInit();  
}


// Set Default Variable for initial Game values
void GameInit() {
  lc.clearDisplay(FirstMax);                                     // clear screen
  lc.clearDisplay(SecondMax);                                    // clear screen
  for (int nal = 0; nal < MaxAliens; nal++) {                    // Enable all aliens
    AliensStat[nal] = true;
  }  
  AlienX =16;                                                    // reset alien position on top (16 means position 0)
  AlienY =0;
  AlienXo =16;
  AlienYo =0;
  AlienXs = false;                                               // Se flag alien go to left direction    
  Explode = 0;
  PrintCannon(CannonPos);                                        // Print initial cannon on screen
  PrintAliens(AlienX,AlienY,AlienXo,AlienYo);                    // Print initial aliens
  CalcMaxAlien();                                                // calculate new aliens edges
  AlienSpeed = (AllKilled * AlienMul) + AlienMul;                // set alien speed
  AlienSpeed -= AlienLevel;                                      // Add more speed trough level
  Serial.print("Speed=");
  Serial.println(AlienSpeed);
}


// Draw cannon on screen position, also clear all pixel around cannon to avoid shadows
void PrintCannon(byte pos)
{
  pos--;                      // fix offset due position variable go from 1 to 16, bud display go from 0 to 15
  SetLed(pos,6,true);         // draw cannon head
  SetLed(pos,7,true);         // draw cannon base1
  if (pos>0) {                // check if outside the screen
    SetLed(pos-1,6,false);    // remove head shadow left
    SetLed(pos-1,7,true);     // draw cannon base2
  }
  if (pos>1) {                // check if outside the screen
    SetLed(pos-2,7,false);    // remove base shadow left
  }
  if (pos<16) {               // check if outside the screen  
    SetLed(pos+1,6,false);    // remove head shadow right
    SetLed(pos+1,7,true);     // draw cannon base3
  }
  if (pos<15) {               // check if outside the screen  
    SetLed(pos+2,7,false);    // remove base shadow right
  }
}


// Print Aliens giving values from array with X, Y offset and clear old alien with oX and oY offset
void PrintAliens(byte x, byte y,byte ox, byte oy){
  for (int nal = 0; nal < MaxAliens; nal++) {                // Remove old Aliens
    if (AliensStat[nal] == true) {                           // check if enabled    
      SetLed( (AliensX[nal]+ox)-16, AliensY[nal]+oy,false);    
    }
  }
  for (int nal = 0; nal < MaxAliens; nal++) {                // Draw Aliens if enabled  
    if (AliensStat[nal] == true) {                           // check if enabled    
      SetLed((AliensX[nal]+x)-16, AliensY[nal]+y,true);      // draw
    }
  }
}


// Game Over animation e scoring
void GameOver() {
int mel = 2600;                                             // setup local var
long InvaderAni = 0;
int AniCount = 0;
 
  while (AniCount <= 10) {                                  // 10 cycle animation
    InvaderAni++;
    mel--;
    if (InvaderAni == 100) {
      for (int i = 0; i < 10; i++)                          // show invaders
      {
        lc.setColumn(FirstMax,i,invader1[i]);
        lc.setColumn(SecondMax,i,invader1[i]);
      }
    } else if (InvaderAni == 200) {
      for (int i = 0; i < 10; i++)                          // show invaders
      {
        lc.setColumn(FirstMax,i,invader2[i]);
        lc.setColumn(SecondMax,i,invader2[i]);
      }
      InvaderAni=0;
      AniCount++;
    }    
  }
  ShowScore();                                              // show score and wait for a keypress
}


// Show final Score an High score
void ShowScore() {
  int waitk = 0;                                            // Set waikey flag
  PrintNumber(Score,false);                                 // Show score
  while (waitk == 0) {                                      // wait fire key to continue
    if (digitalRead(SELECT) == false) {                    // If Fire button pressed
      while (digitalRead(SELECT) == false);                // wait release fire key to continue
      MainScreen();                                         // Show Main Screen
      Score=0;                                              // reset score
      GameInit();                                           // Initialize game                                      
      waitk =1;                                             // reset waitkey flag  
    }
    HiscoreDelay++;
    if (HiscoreDelay >= HiscoreSpeed * 2) {
      PrintNumber(HighScore,true);                          // Show Hi score
      HiscoreDelay = 0;
    } else if (HiscoreDelay == HiscoreSpeed) {
      PrintNumber(Score,false);                             // Show score
    }
  }
}


// Function to set pixel on 16x8 matrix addressed by column, row and status 0,0 = top left
void SetLed(byte X,byte Y, boolean OnOff)
{
  if (X>7) {
    lc.setLed(SecondMax,8-(X-7),Y,OnOff);                // turns on/off LED at X, Y
  } else {
    lc.setLed(FirstMax,7-X,Y,OnOff);                     // turns on/off LED at X, Y
  }
}


// Function to print decimal number from 0000 to 9999 on screen with underline flag
void PrintNumber(long num, boolean underline) {
  byte Digit1;                                           // convert long to 4 digit BCD
  byte Digit2;
  byte Digit3;
  byte Digit4;


  Digit1 = num/1000;                                     // convert long to 4 digit BCD
  Digit2 = (num-(Digit1*1000))/100;
  Digit3 = (num-((Digit2*100)+(Digit1*1000)))/10;
  Digit4 = num-((Digit3*10)+(Digit2*100)+(Digit1*1000));
 
  for (int i = 0; i < 5; i++)                            // create bitmap graphics
  {
    Score1[i] = (Numbers[(Digit2*5)+i] << 4) + (Numbers[(Digit1*5)+i] & B00001111);
    Score2[i] = (Numbers[(Digit4*5)+i] << 4) + (Numbers[(Digit3*5)+i] & B00001111);
  }
  if (underline == true) {                               // add underline
    Score1[6] = 255;
    Score2[6] = 255;
  } else {                                               // remove underline
    Score1[6] = 0;
    Score2[6] = 0;
  }


  for (int i = 0; i < 8; i++)                            // show score
  {
    lc.setColumn(FirstMax,i,Score1[i]);
    lc.setColumn(SecondMax,i,Score2[i]);
  }
}
