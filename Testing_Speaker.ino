#include "test_tones.h"
int noteDuration = 1000/4; //note duration: QUARTER NOTE
int PIN = 8; //Digital Pin

void setup() {
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
  //Uncomment each line to test the respective frequency
  tone(PIN,NOTE_A2,noteDuration);
  //tone(PIN,NOTE_B2,noteDuration);
  //tone(PIN,NOTE_C3,noteDuration);
  //tone(PIN,NOTE_D3,noteDuration);
  //tone(PIN,NOTE_E3,noteDuration);
  //tone(PIN,NOTE_F3,noteDuration);
  //tone(PIN,NOTE_G3,noteDuration);
  //tone(PIN,NOTE_Gsharp3,noteDuration); 
  
  }
