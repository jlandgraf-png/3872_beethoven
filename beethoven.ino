
#include <Vector.h>
#include <LiquidCrystal.h>
#include <Servo.h>

// Tie the right pins to the LCD
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
const int resetPin = 6;
const int stopPin = 7;
const int recordPin = 8;
const int storePin = 13;
const int playLivePin = 18;
const int playRecordingPin = 19;
const int freqPin = A3;

// These will eventually be properly defined as the pushbuttons we use.
bool RESET = false;
bool STOP = false;
bool RECORD = false;
bool STORE = false;
bool PLAY_LIVE = false;
bool PLAY_RECORDING = false;


Servo servo1, servo2; // Attach to pins 9 and 10 respectively in setup later

float freq = 0.0; //associated with frequency pin
int freqi = 0;
int prevFreqi = 0;

// State enumeration.
enum states{idle, stop0, record, play_live, play_recording};

// Variable Declarations
float resetTimer = 0;
int state = idle;
int prevState = idle;
Vector<int> freqVector;
int vectorPosition = 0;
int playingTimer = 0;
int currentFrequency = 0;
int storeButtonHeldDown = 0;

// Declarations for output signals.
int motor1_angle = 0;
int motor2_angle = 0;
float speaker_out = 0;

// Helper functions to reduce redundancy
void moveMotors(int frequency) {
  // Add switch statement for frequency
  servo1.write(90);
  servo2.write(180);
  return;
}

void playFrequency(int frequency) {
  // Add functionality to play tones
  return;
}

void updateLCD() {

  // Update state if it has changed
  if (prevState != state) {
    prevState = state;

    // Clear the top row
    lcd.setCursor(0, 0);
    lcd.print("                ");

    // Print the current state
    lcd.setCursor(0, 0);
    switch (state) {
      case idle:
        lcd.print("IDLE");
        break;
      case stop0:
        lcd.print("STOPPED");
        break;
      case record:
        lcd.print("RECORDING");
        break;
      case play_live:
        lcd.print("PLAYING LIVE");
        break;
      case play_recording:
        //lcd.print("PLAYING RECORDING");
        lcd.print("PLAYBACK MODE");
        break;
    }
  }
  
  // Update the frequency if it has changed
  if (freqi != prevFreqi) {
    prevFreqi = freqi;
    lcd.setCursor(0, 1);
    lcd.print("                ");
    lcd.setCursor(0, 1);
    lcd.print("Frequency: ");
    lcd.print(freqi);
  }
  return;
}

void setup() {
  // Setup for the LCD
  lcd.begin(16, 2);

  // Attach servos (pins 9 and 10)
  servo1.attach(9);
  servo2.attach(10);
  servo1.write(0);
  servo2.write(0);

  pinMode(resetPin, INPUT);
  pinMode(stopPin, INPUT);
  pinMode(recordPin, INPUT);
  pinMode(storePin, INPUT);
  pinMode(playLivePin, INPUT);
  pinMode(playRecordingPin, INPUT);
  pinMode(freqPin, INPUT);
}

void loop() {

  // Check frequency first
  freqi = int(round(float(analogRead(freqPin))/1024.0 * 7.0)+1);

  // Check the buttons to determine the state
  // Priority increases the further you go
  if (PLAY_RECORDING = digitalRead(playRecordingPin)) state = play_recording;
  if (PLAY_LIVE = digitalRead(playLivePin)) state = play_live;
  if (RECORD = digitalRead(recordPin)) state = record;
  if (STOP = digitalRead(stopPin)) state = stop0;
  if (RESET = digitalRead(resetPin)) {
    if (state == idle) {
      // Increment the reset timer.
      resetTimer++;
    } else {
      // The idle state is being entered.
      resetTimer = 0;
      state = idle;
    }
  } else {
    resetTimer = 0;
    // Bring the reset Timer back to 0 when RESET is no longer being asserted.
  }
  
  // Perform different functionality depending on the current state.
  switch (state) {
    
    case idle:
      // First reset some variables and motors
      moveMotors(0);
      playFrequency(0);
      vectorPosition = 0;
      playingTimer = 0;
      currentFrequency = 0;
      storeButtonHeldDown = 0;
      //TODO update LCD

      // Reset the memory if held down
      if (resetTimer > 2) {
        freqVector.clear();
        resetTimer = 0;
      }
      break;


    case stop0:

      //Update LCD
      
      playFrequency(0);
      storeButtonHeldDown = 0;
      break;


     case record:
      
      //Update LCD
      
      if (STORE && !storeButtonHeldDown) {
        storeButtonHeldDown = 1;
        freqVector.push_back(freqi);
      } else {
        storeButtonHeldDown = 0;
      }
      break;


     case play_live:
      
      //Update LCD
      
      if (STORE) {
        int newFreq = freqi;
        playFrequency(newFreq);
        moveMotors(newFreq);
      }
      break;


     case play_recording:

      //Update LCD

      if(freqVector.size() == 0){
        playFrequency(0);
        break;
      }
      
      if (playingTimer > 200 || playingTimer == 0) {
        playingTimer = 0;
        if (freqVector.size() - 1 == vectorPosition) {
          vectorPosition == 0;
        } else {
          vectorPosition++;
        }
        playFrequency(freqVector[vectorPosition]);
        moveMotors(freqVector[vectorPosition]);
      }
      playingTimer++;
      
      break;
  }


  updateLCD();
}
