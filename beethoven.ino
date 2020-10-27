#include <Vector.h>
#include <Array.h>
#include <LiquidCrystal.h>
#include <Servo.h>


//-----------------------------------------------------------------------

// Notes played
// 1 - A2  - 110.000 Hz
// 2 - B2  - 123.471 Hz
// 3 - C3  - 130.813 Hz
// 4 - D3  - 146.832 Hz
// 5 - E3  - 164.814 Hz
// 6 - F3  - 174.614 Hz
// 7 - G3  - 195.998 Hz
// 8 - G#3 - 207.652 Hz




// Variable Declarations

// Tie the right pins to the LCD
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Declaring pins for buttons, frequency dial, and speaker
const int resetPin = 7;
const int stopPin = A2;
const int recordPin = 8;
const int storePin = 13;
const int playLivePin = 18;
const int playRecordingPin = 19;
const int freqPin = A3;
const int speakerPin = 6;

// Values associated with the pushbuttons
bool RESET = false;
bool STOP = false;
bool RECORD = false;
bool STORE = false;
bool PLAY_LIVE = false;
bool PLAY_RECORDING = false;

// To begin setting up servos
Servo servo1, servo2;
const int min = 480;
const int max = 2500;
const int servo1pin = 9;
const int servo2pin = 10;


// Values used in the frequency dial
float freq = 0.0; //associated with frequency pin
int freqi = 0; // interger associated with frequency
int prevFreqi = 0; // interger associated with previous frequency (for displaying on LCD)

// State enumeration.
enum states{idle, stop0, record, play_live, play_recording};

// Misc variable Declarations
float resetTimer = 0;
int state = idle;
int prevState = idle;

const int totalVectorSize = 40;
int freqVector[totalVectorSize];
int vectorPosition = 0;
int vectorSize = 0;
int storeButtonHeldDown = 0;
int startedPlayingRecording = 0;

// Timers for holding reset button and playing notes
long resetStartTime = 0; //Full reset after 3000 millis
long resetCurrentTime = 0;
long playingStartTime = 0; //Change notes after 500 millis
long playingCurrentTime = 0;

// Declarations for output signals.
int motor1_angle = 0;
int motor2_angle = 0;
float speaker_out = 0;


// End of variable declarations
//------------------------------------------------------------------------------
// Helper functions to reduce redundancy


void moveMotors(int frequency) {
  // Add switch statement for frequency
  switch (frequency) {
    case 0:
      servo1.write(0);
      servo2.write(0);
      break;
    case 1:
      servo1.write(180);
      servo2.write(180);
      break;
    case 2: 
      servo1.write(135);
      servo2.write(180);
      break;
    case 3:
      servo1.write(85);
      servo2.write(180);
      break;
    case 4:
      servo1.write(40);
      servo2.write(180);
      break;
    case 5:
      servo1.write(180);
      servo2.write(135);
      break;
    case 6:
      servo1.write(180);
      servo2.write(85);
      break;
    case 7:
      servo1.write(180);
      servo2.write(40);
      break;
    case 8:
      servo1.write(130);
      servo2.write(130);
      break;
  }
  return;
}

void playFrequency(int frequency) {
  
  switch (frequency) {
    case 0:
      noTone(speakerPin);
      break;
    case 1:
      tone(speakerPin, 110);
      break;
    case 2: 
      tone(speakerPin, 123.471);
      break;
    case 3:
      tone(speakerPin, 130.813);
      break;
    case 4:
      tone(speakerPin, 146.832);
      break;
    case 5:
      tone(speakerPin, 164.814);
      break;
    case 6:
      tone(speakerPin, 174.614);
      break;
    case 7:
      tone(speakerPin, 195.998);
      break;
    case 8:
      tone(speakerPin, 207.652);
      break;
  }
  return;
}

void updateLCD() {

    // Clear the top row
    //lcd.setCursor(0, 0);
    //lcd.print("                ");

    // Print the current state
    lcd.setCursor(0, 0);
    
    switch (state) {
      case idle:
        lcd.print("IDLE            ");
        break;
      case stop0:
        lcd.print("STOPPED         ");
        break;
      case record:
        lcd.print("RECORDING       ");
        break;
      case play_live:
        lcd.print("PLAYING LIVE    ");
        break;
      case play_recording:
        //lcd.print("PLAYING RECORDING");
        lcd.print("PLAYBACK MODE   ");
        break;
    }
    /*
    lcd.print(freqVector.size());
    lcd.setCursor(2, 0);
    for (int i = 0; i < freqVector.size() && i < 16; ++i) {
      lcd.print("-");
      lcd.print(freqVector[i]);
      
    }
    lcd.setCursor(0,1);
    lcd.print(storeButtonHeldDown);
    */
  
  // Update the frequency if it has changed
  //if (freqi != prevFreqi) {
    prevFreqi = freqi;
    lcd.setCursor(5, 1);
    lcd.print(freqi);
  //}
  return;
}


// End of helper functions
//----------------------------------------------------------------------------
// Setup and Main Loop


void setup() {
  // Setup for the LCD
  lcd.begin(16, 2);

  // Attach servos (pins 9 and 10)
  servo1.attach(servo1pin, min+150, max-120);
  servo2.attach(servo2pin, min, max);
  servo1.write(0);
  servo2.write(0);

  pinMode(resetPin, INPUT_PULLUP);
  pinMode(stopPin, INPUT_PULLUP);
  pinMode(recordPin, INPUT_PULLUP);
  pinMode(storePin, INPUT_PULLUP);
  pinMode(playLivePin, INPUT_PULLUP);
  pinMode(playRecordingPin, INPUT_PULLUP);
  pinMode(freqPin, INPUT);

  lcd.setCursor(0,0);
  lcd.print("IDLE");
  lcd.setCursor(0,1);
  lcd.print("Dial:");
  lcd.setCursor(6,1);
  lcd.print(", Stored:");


  freqVector[0] = 5;
  freqVector[1] = 2;
  freqVector[2] = 8;
  vectorSize = 3;
}

void loop() {

  // Check frequency first
  freqi = int(round(float(analogRead(freqPin))/1024.0 * 7.0)+1);

  // Check the buttons to determine the state
  // Priority increases the further you go
  if (PLAY_RECORDING = !digitalRead(playRecordingPin)) state = play_recording;
  if (PLAY_LIVE = !digitalRead(playLivePin)) state = play_live;
  if (RECORD = !digitalRead(recordPin)) state = record;
  if (STOP = !digitalRead(stopPin)) state = stop0;
  if (RESET = !digitalRead(resetPin)) {
    if (state != idle) {
      // The idle state is being entered.
      resetStartTime = millis();
      state = idle;
    } else {
      resetCurrentTime = millis();
      if (resetCurrentTime - resetStartTime >= 3000) {
        lcd.setCursor(0, 1);
        lcd.print("Dial:");
        lcd.print(freqi);
        lcd.print(", Stored: ");
        for (int i = 0; i < totalVectorSize; i++) {
          freqVector[i] = 0;
        }
        vectorSize = 0;
      }
    }
  }
  
  // Perform different functionality depending on the current state.
  switch (state) {
    
    case idle:
      // First reset some variables and motors
      moveMotors(0);
      playFrequency(0);
      vectorPosition = 0;
      storeButtonHeldDown = 0;
      //TODO update LCD

      break;


    case stop0:

      //Update LCD
      playFrequency(0);
      storeButtonHeldDown = 0;
      
      break;


     case record:
     noTone(speakerPin);
      STORE = !digitalRead(storePin);
      //Update LCD
      if (STORE) {
        delay(50);
        STORE = !digitalRead(storePin);
      }
      if (STORE && !storeButtonHeldDown) {
        if(vectorSize < totalVectorSize) {
          lcd.setCursor(15, 1);
          lcd.print(freqi);
          storeButtonHeldDown = 1;
          freqVector[vectorSize] = freqi;
          vectorSize++;
        }
      } else if (!STORE) {
        storeButtonHeldDown = 0;
      }
      break;


     case play_live:
      
      //Update LCD
      if (!digitalRead(storePin)) {
        int newFreq = freqi;
        playFrequency(newFreq);
        moveMotors(newFreq);
      }
      break;


     case play_recording:
     /*
     lcd.setCursor(0, 1);
     lcd.print(totalVectorSize);
     lcd.print(" ");
     lcd.print(vectorSize);
     lcd.print(" ");
     lcd.print(vectorPosition);
     lcd.print(" ");
     */
      //Update LCD

      if(vectorSize == 0){
        playFrequency(0);
        break;
      }
      playingCurrentTime = millis(); // Read current time
      if (vectorSize == vectorPosition) vectorPosition = 0;
      if (playingCurrentTime - playingStartTime >= 500 || !startedPlayingRecording) {
        startedPlayingRecording = 1;
        playingStartTime = millis();

        
        playFrequency(freqVector[vectorPosition]);
        moveMotors(freqVector[vectorPosition]);

        vectorPosition++;
      }
      
      break;
  }

  if (state != play_recording) {
    startedPlayingRecording = 0;
  }


  updateLCD();
}
