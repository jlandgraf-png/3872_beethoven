#define liveButton 2
#define playRecordButton 4
#define recordButton 7
#define stopButton 12
#define resetButton 13
volatile int buttonPress = 0; 
volatile bool buttonPressed = false; 

void setup(){
  Serial.begin(9600);
  pinMode(liveButton,OUTPUT);
  pinMode(playRecordButton,OUTPUT);
  pinMode(recordButton,OUTPUT);
  pinMode(stopButton,OUTPUT);
  pinMode(resetButton,OUTPUT);
  Serial.println("void setup(): Success");
  //attachInterrupt(digitalPinToInterrupt(button),live_event,LOW);
}// end of setup()


void loop (){
if(pushed_button(liveButton)){ live_event(); } //check if the button has been pressed ? execute function
if(pushed_button(playRecordButton)){ playRecord_event(); }
if(pushed_button(recordButton)){ record_event(); }
if(pushed_button(stopButton)) { stop_event(); }
if(pushed_button(resetButton)){ reset_event(); }
delay(10);

buttonPressed = false; //reset the pushed_button function.

}// end of loop()


bool pushed_button(int inputButton){ //takes in an input pin#
buttonPress = digitalRead(inputButton); //reads the pin ? Did it go 'HIGH'
if(buttonPress == 1){
  delay(50); //if it went high : wait 50ms
  buttonPress = digitalRead(inputButton); //read the pin ? Did it go 'LOW'
  if(buttonPress = 0){
    buttonPressed = true; //if so set the boolean to TRUE
  }
}
else{
      buttonPressed = false; //set the boolean to FALSE 
      delay(50); //wait 50ms
  }
  return buttonPressed; //return the boolean
}//end of pushed_button function


void live_event (){
  Serial.println("State: LIVE ");
} //end of live_event function

void playRecord_event (){
  Serial.println("State: PLAY RECORD ");
} //end of playRecord_event function

void record_event (){
  Serial.println("State: RECORD ");
} //end of live_event function

void stop_event (){
  Serial.println("State: STOP ");
} //end of stop_event function

void reset_event (){
  Serial.println("State: RESET ");
} //end of reset_event function
