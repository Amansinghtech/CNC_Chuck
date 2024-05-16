// importing libraries
#include <Servo.h>
#include <Wire.h>

#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F,16,2);  // set the LCD address to 0x3F for a 16 chars and 2 line display
// reference guide for this ibrary https://lastminuteengineers.com/i2c-lcd-arduino-tutorial/


// Variable Declarations
Servo servo_jaw_1;
Servo servo_jaw_2;
Servo servo_jaw_3;
Servo servo_jaw_4;
Servo servo_clamp;

unsigned int SINGLE_ROTATION_DELAY = 1000;
unsigned int BUFFER_DELAY = 10;
unsigned int MOTOR_SPEED = 38; // MAX Motor rotation speed

bool VARIABLE_MOTOR_SPEED = false;

#define MOVE_FORWARD -1
#define MOVE_BACKWARD 1
#define STOP 0

#define potpin  A0

#define PUSH_BTN_1 12
#define PUSH_BTN_2 11
#define PUSH_BTN_3 8
#define PUSH_BTN_4 7

#define JAW_PIN_1 10
#define JAW_PIN_2 9
#define JAW_PIN_3 6
#define JAW_PIN_4 5

#define LIMIT_SWITCH_1 0
#define LIMIT_SWITCH_2 13
#define LIMIT_SWITCH_3 2
#define LIMIT_SWITCH_4 4


// 1cm equals 2 rotations
#define ROTATIONS_PER_UNIT 2


#define CLAMP_PIN 3

String unit = "mm";


int unit_length = 0;
int unit_breadth = 0;


void setup() {
  // put your setup code here, to run once:
  servo_jaw_1.attach(JAW_PIN_1);
  servo_jaw_2.attach(JAW_PIN_2);
  servo_jaw_3.attach(JAW_PIN_3);
  servo_jaw_4.attach(JAW_PIN_4);
  servo_clamp.attach(CLAMP_PIN);

  lcd.init();
  lcd.clear();         
  lcd.backlight();      // Make sure backlight is on

  pinMode(PUSH_BTN_1, INPUT_PULLUP);
  pinMode(PUSH_BTN_2, INPUT_PULLUP);
  pinMode(PUSH_BTN_3, INPUT_PULLUP);
  pinMode(PUSH_BTN_4, INPUT_PULLUP);

  pinMode(LIMIT_SWITCH_1, INPUT_PULLUP);
  pinMode(LIMIT_SWITCH_2, INPUT_PULLUP);
  pinMode(LIMIT_SWITCH_3, INPUT_PULLUP);
  pinMode(LIMIT_SWITCH_4, INPUT_PULLUP);
  
  Serial.begin(9600);

  openClamp();
  lcdSayHello();
  resetAllJaws();
}


void lcdSayHello() {
    // Print a message on both lines of the LCD.
  lcd.setCursor(2,0);   //Set cursor to character 2 on line 0
  lcd.print("Hello world!");
  
  lcd.setCursor(2,1);   //Move cursor to character 2 on line 1
  lcd.print("LCD Tutorial");
  delay(5000);
  lcd.clear();
}

void rotateMotor(Servo motor, int rotations, int rotation_direction) {

  Serial.print("Rotating Motor ");
  Serial.print(" count: ");
  Serial.println(rotations); 
  
  for (int rotation_count = 1; rotation_count <= rotations; rotation_count++) {

      Serial.print("rotation: ");
      Serial.print(rotation_count);
      Serial.print(" speed: ");
      Serial.println(MOTOR_SPEED);
    
      if (rotation_direction > 0) {
          motor.write(90 + MOTOR_SPEED); // rotate clockwise direction
        }
      else if (rotation_direction < 0) {
          motor.write(90 - MOTOR_SPEED); // rotate counter clockwise direction
        }
      else {
        motor.write(90); // stop motor
      }
      
      delay(SINGLE_ROTATION_DELAY);
      motor.write(90); 
      delay(BUFFER_DELAY);
      Serial.println("Motor stopped");
    }
}

void readButtons() {
    bool btn_1 = digitalRead(PUSH_BTN_1);
    bool btn_2 = digitalRead(PUSH_BTN_2);
    bool btn_3 = digitalRead(PUSH_BTN_3);
    bool btn_4 = digitalRead(PUSH_BTN_4);

    Serial.print("Buttons:");
    Serial.print(" btn-1: ");
    Serial.print(btn_1);
    Serial.print(" btn-2: ");
    Serial.print(btn_2);
    Serial.print(" btn-3: ");
    Serial.print(btn_3);
    Serial.print(" btn-4: ");
    Serial.println(btn_4);
  }

void openClamp() {
    servo_clamp.write(180);
    delay(1000);  
}

void closeClamp() {
    servo_clamp.write(0);
    delay(1000);  
}

void resetJawPosition(String name, Servo motor, int limitPin) {
  lcd.clear();
  while(true) {  
    lcd.setCursor(0,0);
    lcd.print("Setting ");
    lcd.print(name);
    
    int limit_not_reached = digitalRead(limitPin);
    
    if (limit_not_reached){
        motor.write(90 - MOTOR_SPEED); // motor will go backward
    }
    else {
        motor.write(90); // put the motor to stop
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print(name);
        lcd.print(" SET!!!");
        break; 
    }
  }
  delay(500);
}

void resetAllJaws() {
  lcd.clear();
  // Servo motor = servo_jaw_1;

  lcd.setCursor(0,0);
  lcd.print("Resetting JAWs!!");
  delay(2000);

// resetting jaw_1
  resetJawPosition("JAW_1", servo_jaw_1, LIMIT_SWITCH_1);
  resetJawPosition("JAW_2", servo_jaw_2, LIMIT_SWITCH_2);
  resetJawPosition("JAW_3", servo_jaw_3, LIMIT_SWITCH_3);
  resetJawPosition("JAW_4", servo_jaw_4, LIMIT_SWITCH_4);
  lcd.clear();
}

void resetInputValues () {
  unit_length = 0;
  unit_breadth = 0;
  resetAllJaws();
  lcd.clear();
}

void printValuesToScreen() {
  lcd.setCursor(0, 0);   //Set cursor to character 0 on line 0
  lcd.print("length: ");
  lcd.print(unit_length);
  lcd.print(" ");
  lcd.print(unit);
  lcd.setCursor(0, 1);
  lcd.print("breadth: ");
  lcd.print(unit_breadth);
  lcd.print(" ");
  lcd.print(unit);
}

void takeUserInput() {
  bool values_set = false;
  
  while (!values_set) {
    int length_btn = digitalRead(PUSH_BTN_1);    
    int breadth_btn = digitalRead(PUSH_BTN_2);    
    int submit = digitalRead(PUSH_BTN_4);    
    int reset = digitalRead(PUSH_BTN_3);    
  
  //      set all the values to 0
    if (reset == 0) {
        resetInputValues();
    }

//  break out of the loop and close the function
    if (submit == 0) {
      break;
    }

//  increment unit length if btn1 is pressed
    if (length_btn == 0) {
        unit_length++;
    }

//  increment unit breadth if btn2 is pressed
    if (breadth_btn == 0) {
        unit_breadth++;
    }
    
    printValuesToScreen();
    delay(200);
  }
}

void setJawsPosition() {
  lcd.clear();
  lcd.setCursor(0, 0);   //Set cursor to character 0 on line 0
  lcd.print("Setting JAWs");
  lcd.setCursor(0,1);
  lcd.print("Please Wait!!!");
  
  openClamp();
  // if movement is 5 cm and 1 cm = 10 rotations the motor will rotate 5 * 10 = 50 rotations in counter clockwise direction
  rotateMotor(servo_jaw_1, unit_length * ROTATIONS_PER_UNIT, MOVE_FORWARD); 
  rotateMotor(servo_jaw_2, unit_breadth * ROTATIONS_PER_UNIT, MOVE_FORWARD);
  rotateMotor(servo_jaw_3, unit_length * ROTATIONS_PER_UNIT, MOVE_FORWARD);  
  rotateMotor(servo_jaw_4, unit_breadth * ROTATIONS_PER_UNIT, MOVE_FORWARD); 
  closeClamp();
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Positions Set");
  delay(2000);
  lcd.clear();
}

void loop() {

  if (VARIABLE_MOTOR_SPEED){
      int val = analogRead(potpin);
      MOTOR_SPEED = map(val, 0, 1023, 0, 90);     // scale it for time delay of one rotation
    }
    
    takeUserInput();
    setJawsPosition();
}
