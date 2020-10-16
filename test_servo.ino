#include <Servo.h>
#define button_60deg 2
#define button_120deg 4
#define button_180deg 7

Servo test_servo;  // create servo object to control a servo

volatile int curr_pos; 
bool inDefault;        // bool that stores whether if the Servo is in default positon or not.

void default_pos(){ //function that makes the servo goes back to default position
  test_servo.write(0);
  delay(10); //stay in default pos for 10seconds
}

bool if_default(){ //Is the motor back to default postion
  if(test_servo.read() != 0){
    inDefault = false; //if not in defalut postion then FALSE
  }
  else{
    inDefault = true; //if in defalut position then TRUE
  }
  return inDefault; //return variable
}

void printCurrentPosition(){
  curr_pos = test_servo.read(); //Read the current postion
  delay(5);  //delay 5ms
  Serial.print("Current Position:");  
  Serial.println(curr_pos);  //print out current position
  delay(5);
}

void setup() {
  Serial.begin(9600);
  pinMode(button_60deg,INPUT);
  pinMode(button_120deg,INPUT);
  pinMode(button_180deg,INPUT);
  test_servo.attach(9);  // attaches the servo on pin 9 to the servo object
  default_pos(); //go to default position at start
}

void loop() {

  printCurrentPosition(); //read and print current position

  if(digitalRead(button_60deg)){
    if(!if_default()){
      Serial.println("Not in DEFAULT");
      default_pos(); //if not in default -> go to default 
      Serial.println("In DEFAULT");
    }
    test_servo.write(60); //go to assigned degree
    delay(15); //USER cannot put an input after 15ms of motor going to assigned degree
  }

  if(digitalRead(button_120deg)){
    if(!if_default()){
      Serial.println("Not in DEFAULT");
      default_pos();
      Serial.println("In DEFAULT");
    }
    test_servo.write(120);
    delay(15);
  }

  if(digitalRead(button_180deg)){
    if(!if_default()){
      Serial.println("Not in DEFAULT");
      default_pos();
      Serial.println("In DEFAULT");
    }
    test_servo.write(180);
    delay(15);
  }

}
