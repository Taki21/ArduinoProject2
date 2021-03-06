#include <LiquidCrystal.h>

int led1;
int led2;
int led3;
int led4;
int button1;
int button2;
int button3;
bool toggle;
int future;
int currLED;
bool initLED;
long timeLeft;
int buzzer;
int c5 = 523;
int g5 = 783;
int d5 = 587;
int e5 = 659;
int g4 = 392;
int e4 = 329;
int c4 = 261;
int d4 = 293;
int a4 = 440;
int b4 = 493;
int f4 = 349;
boolean setRand;
long randTime;
int mode;
long time;
long mil;
long timeElapsed = 0;
boolean gameEnd;
int score = 0;
boolean pointsGiven = false;
boolean jumping = false;

long jumpTime = 0;
boolean newPress = false; 

long gameOverTimeout = 0;
int gamesPlayed = 1;

// obstacle postion
int obstacle1Pos = 15;

// empty frame
int emptyFrame[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};

// character animation frames

byte charFrame0[8] = {
        B01110,
        B01110,
        B01110,
        B00100,
        B11111,
        B00100,
        B01010,
        B10001
};

byte charFrame1[8] = {
        B01110,
        B01110,
        B01110,
        B10100,
        B01110,
        B00101,
        B11010,
        B00010
};

byte charFrame2[8] = {
        B01110,
        B01110,
        B01110,
        B00101,
        B01110,
        B10100,
        B01011,
        B01000
};

// obstacles frames

byte obsFrame0[8] = {
      B11111,
      B11111,
      B11111,
      B11111,
      B11111,
      B11111,
      B11111,
      B11111
};

//initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); 

void setup() {  
  Serial.begin(9600);                                                                              
  led1 = 8;
  led2 = 7;
  led3 = 6;
  button2 = 13;
  toggle = false;
  future = 0;
  currLED = 0;
  initLED = true;
  buzzer = 9;
  pinMode(buzzer,OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(button2, INPUT);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);  
  lcd.createChar(0, charFrame0);
  lcd.createChar(1, charFrame1);
  lcd.createChar(2, charFrame2);
  lcd.createChar(3, obsFrame0); // byte(3)
  randTime = 200;
  setRand = false;
  mode = 0;
  time = 0;
  mil = 0;
  gameEnd = false;
  lcd.setCursor(0,1);
  lcd.print("xxxxxxxxxxxxxxx");
  lcd.setCursor(10,0);
  lcd.print(0);
  //Serial.println("dfsfksdf");
}

void loop() {
  digitalWrite(led1, HIGH);
  //manualCycle();
  //allOn();
  //playSong2();
  checkButtons();

  if(gameEnd == false) {
    jump();
    playSong();
    gameOverTimeout = millis() + 1000;
    beginnerObstacleMove();
  } else {
    playSong2();
  }
  
  gameOver();
}

void loopCharacter(int animationFrame) {
  long time = millis();
  long timeUnit = time / 500;
  lcd.setCursor(2, animationFrame);
  switch(timeUnit % 2) {
    case 0:
      lcd.write(byte(1));
    break;

    case 1:
      lcd.write(byte(2));
    break;
  }
}

void beginnerObstacleMove() {
  mil = millis() - timeElapsed;

  time = mil / randTime;
  int i = (int) (time % 16);
    
  if(obstacle1Pos < 2 && pointsGiven == false) {  
    score += 10;
    pointsGiven = true;
    //score += 1;  
    lcd.setCursor(10,0);
    lcd.print(score);
  }

  if(obstacle1Pos > 2) pointsGiven = false;
  
  if(16 - i == 16) {
    lcd.setCursor(0,1);
    lcd.print("x");
    if(time > 0) {
      randTime = 100;//random(50, 100);
      timeElapsed = millis();
    }
  } else {
    lcd.setCursor(16-i,1);
    lcd.print("x");
    obstacle1Pos = 16 - i;
  }
  lcd.setCursor(15-i,1);
  lcd.print('o');
  
}

void jump() {
  int bs = digitalRead(button2);
  long currTime = millis();      
  if(jumpTime == 0) jumpTime = currTime;

  if (bs == 0) {
    jumping = false;
    lcd.setCursor(2,0);
    lcd.print(" ");
    loopCharacter(1);  
    jumpTime = currTime;
    newPress = true;
    digitalWrite(led2, LOW);
  }

  if(bs == 1) {
    //lcd.setCursor(0,0);
    //lcd.print(bs);    
    if(newPress) {
      jumpTime = currTime + 1000;
      newPress = false;
    }
    if(jumpTime > currTime) {
      jumping = true;
      digitalWrite(led2, HIGH);
      lcd.setCursor(2,1);
      lcd.print("x");
      loopCharacter(0);
    } else {
      jumping = false;
      lcd.setCursor(2,0);
      lcd.print(" ");
      loopCharacter(1);
      digitalWrite(led2, LOW);
    }
  }

}

void gameOver()
{
  if(jumping == false && obstacle1Pos == 2) {
    if(gameEnd == false) lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("GP:");
    lcd.setCursor(4,0);
    lcd.print(gamesPlayed);
    lcd.setCursor(7,0);
    lcd.print("S:");
    lcd.setCursor(9,0);
    lcd.print(score);
    lcd.setCursor(3,1);
    lcd.print("Game Over!");
    gameEnd = true;
    digitalWrite(led3, HIGH);
  } 
  
  if (digitalRead(button2) == 1 && gameEnd == true && gameOverTimeout < millis()) {
    gameOverTimeout = 0;
    lcd.clear();
    lcd.setCursor(0,1);
    score = 0;
    lcd.print("xxxxxxxxxxxxxxx");
    timeElapsed = millis();
    obstacle1Pos = 15;
    gameEnd = false;
    digitalWrite(led3, LOW);  
    gamesPlayed++;
  }
  
}

void allOn()
{
  digitalWrite(led1, HIGH);
  digitalWrite(led2, HIGH);
  digitalWrite(led3, HIGH);
  digitalWrite(led4, HIGH);
}

void checkButtons()
{
  int buttonState1 = digitalRead(button1);
  int buttonState2 = digitalRead(button2);
  int buttonState3 = digitalRead(button3);
  
  if(buttonState1 == 1) Serial.println("1");
  else if(buttonState2 == 1) Serial.println("2");
  else if(buttonState3 == 1) Serial.println("3");
}

void manualCycle() {
  if(initLED) digitalWrite(led1, HIGH);
  int bs = digitalRead(button1);
  long currTime = millis();
  bool coolDown = currTime >= future;
  
  if(bs == 1 && coolDown) {
    if(initLED) initLED = false;
    if(currLED == 2) currLED = 0;
    else currLED++;
    future = currTime + 300;
  }
  
  switch(currLED) {
    case 0:
      digitalWrite(led1, HIGH);
      digitalWrite(led3, LOW);
    break;
    
    case 1:
        digitalWrite(led2, HIGH);
      digitalWrite(led1, LOW);
    break;
    
    case 2:
        digitalWrite(led3, HIGH);
      digitalWrite(led2, LOW);
    break;
  }
}
 
//BUZZER METHODS

void playSong()
{
  int tone[] = {c5,  g5,  c5,  g5,  c5,  g5,  c5,  g5,  g4,  d5,  g4,  d5,  g4,   d5, g4,  d5,  a4,  e5,  a4,  e5,  a4,  e5,  a4,  e5,  f4,  c5,  f4,  c5,  f4,  c5,  f4,  c5,  c5,  g5,  c5,  g5,  c5,  g5,  c5,  g5,  g4,  d5,  g4,  d5,  g4,  d5,  g4,  d5,  a4,  e5,  a4,  e5,  a4,  e5,  a4,  e5,  f4,  c5,  f4,  c5,  f4,  c5,  f4,  c5,  c5};
  int durs[] = {200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200};

  playTones(buzzer, tone, durs, sizeof(tone)/sizeof(int));
}

void playSong2() {
  int tone[] = {0};
  int durs[] = {200};

  playTones(buzzer, tone, durs, sizeof(tone)/sizeof(int));
}

/**
 * playTones:  Plays an array of tones.  
 * Each for a specified duration.
 * The Arduino can do something else while calling this method.
 * 
 * Precondtions:  1)  tones and durations have the same number of elements
 *                2)  elements of durations are in milliseconds
 *                3)  length is the length of tones and durations
 */
void playTones(int buzz, int tones[], int durations[], int length){
  if(sizeof(tones)/sizeof(int) == sizeof(durations)/sizeof(int))
  {
    long sum = 0;
    for(int i = 0; i<length;i++)
    {
      sum += durations[i];
    }
    //calculate which index we should be working with
    int musicI = 0;
    for(musicI = 0; millis()%sum>=sumUntil(durations,musicI,length);musicI++)
    {    }
    if(millis()%sum<sumUntil(durations,musicI,length))
    {
      if(tones[musicI]!= 0)
      {
        tone(buzz, tones[musicI]);
      }
      else
      {
        noTone(buzz);
      }
    }
  }
}
/**
 * sumUntil:  returns the sum of all the values from 0 to index 
 * inside of arr
 * This is a helper method for playTones
 * 
 * Precondtions:  1)  0<=index<=length-1
 *                2)  length is the length of arr
 */
long sumUntil(int arr[], int index,int length)
{
  long sum = 0;
  for(int i = 0;i<=index;i++)
  {
    sum+=arr[i];
  }
  return sum;
}
