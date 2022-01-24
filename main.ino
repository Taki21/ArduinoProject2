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

//initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
  Serial.begin(9600);
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
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  pinMode(button3, INPUT);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("hello, world!");
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  lcd.print(millis() / 1000);
  //manualCycle();
  allOn();
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
 
