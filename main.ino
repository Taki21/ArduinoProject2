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
int buzzer;
int g4 = 392;
int e4 = 329;
int c4 = 261;
int d4 = 293;
int a4 = 440;
int b4 = 493;
int f4 = 349;

byte newChar0[8] = {
        B01110,
        B01110,
        B01110,
        B00100,
        B11111,
        B00100,
        B01010,
        B10001
};

byte newChar1[8] = {
        B01110,
        B01110,
        B01110,
        B10100,
        B01110,
        B00101,
        B11010,
        B00010
};

byte newChar2[8] = {
        B01110,
        B01110,
        B01110,
        B00101,
        B01110,
        B10100,
        B01011,
        B01000
};

//initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {  
  Serial.begin(115200);                                                                              
  led1 = A0;
  led2 = A1;
  led3 = A2;
  led4 = A3;
  button1 = 13;
  button2 = 8;
  button3 = 7;
  toggle = false;
  future = 0;
  currLED = 0;
  initLED = true;
  buzzer = 9;
  pinMode(buzzer,OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  pinMode(button3, INPUT);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);  
  lcd.createChar(0,newChar0);
  lcd.createChar(1,newChar1);
  lcd.createChar(2,newChar2);
  // Print a message to the LCD.
  //lcd.print("hi");
  //Serial.print("no"); 
}




void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  
  lcd.setCursor(1, 1);
  
  lcd.setCursor(1, 0);
  // print the number of seconds since reset:
  //lcd.print(millis() / 1000);

  manualCycle();
  //allOn();
  //playLondonBridge();

  loopCharacter();
}

void loopCharacter() {
  long time = millis();
  long timeUnit = time / 500;
  switch(timeUnit % 2) {
    case 0:
      lcd.write(byte(1));
    break;

    case 1:
      lcd.write(byte(2));
    break;
  }
}

void jump() {
  int bs = digitalRead(button2);
  long currTime = millis();
  bool coolDown = currTime >= future;
  
  if(bs == 1 && coolDown) {
    if(initLED) initLED = false;
  	if(currLED == 2) currLED = 0;
    else currLED++;
    future = currTime + 1000;
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
  
  Serial.print("Button State 1 = ");
  Serial.println(buttonState1);
  Serial.print("Button State 2 = ");
  Serial.println(buttonState2);
  Serial.print("Button State 3 = ");
  Serial.println(buttonState3);
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
    future = currTime + 1000;
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

void playLondonBridge()
{
  int tone[] = {g4,  a4,  g4,  f4,  e4,  f4,  g4,  0};
  int durs[] = {500, 500, 500, 500, 500, 500, 500, 500};
  
  playTones(buzzer, tone, durs, 8);
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
    for(musicI = 0; millis()%sum>=sumUntil(durations,musicI,length);musicI++)//super bananas
    {    }
    if(millis()%sum<sumUntil(durations,musicI,length))    //bananas
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
