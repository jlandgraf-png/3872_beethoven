//---CONFIG_BEGIN---
//#pragma GCC push_options
//#pragma GCC optimize("Os")

//#include <core.h> // Required by cpu
//#include <cpu.h>
//#include <Generic.h>
#include <iostream>
#include <vector>
#include <stdio.h>
using std::vector;
using std::cout;
using std::endl;


// Used when testing output with setup input
#define VERIFY 1
#define DEBUG 0

//#pragma GCC pop_options

// Peripheral Constructors
//CPU & cpu = Cpu;
/*
GenericButton RESET = GenericButton(2);
GenericButton STOP = GenericButton(2);
GenericButton RECORD = GenericButton(2);
GenericButton STORE = GenericButton(2);
GenericButton PLAY_LIVE = GenericButton(2);
GenericButton PLAY_RECORDING = GenericButton(2);
*/

// Here these button booleans will be dynamically defined with a file using a wrapper script
bool RESET;
bool STOP;
bool RECORD;
bool STORE;
bool PLAY_LIVE;
bool PLAY_RECORDING;

// Flowchart Variables
int state;
enum states{idle, stop, record, play_live, play_recording};
long resetTimer;
float motor_1_angle;
float motor_2_angle;
float motor_1_velocity;if
long motor_2_velocity;
float speaker_out;

// Other global variables
vector<int> freqVector;
int vectorPosition;
int playingTimer;
int currentFrequency; // Frequency displayed on dial
int playingFrequency; // Frequency being played
int storeButtonHeldDown;


// Inputs and Variables for verification
struct inputs {
	bool RESET;
	bool STOP;
	bool RECORD;
	bool STORE;
	bool PLAY_LIVE;
	bool PLAY_RECORDING;
	int dial_frequency;
};

inputs verification_inputs[24];
int vic; //Verification Input Counter

// Peripheral Routines

void moveMotor1(int freq) {return;}
void moveMotor2(int freq) {return;}
int readFrequencyInput() {
	if (VERIFY) {
		return currentFrequency;
	}
	return 0;
}
void playTone(int freq) {return;}




// Flowchart Routines

// Parses buttons to change state. Assumes user does not press multiple buttons in one function call.
// If the user were to press multiple buttons, the button closest to the bottom would determine the state after the function returns.
void checkButtons() {


	if (DEBUG) cout << "check buttons func" << endl;
  // Verification Inputs (skipped when verify is not enabled)
  if (VERIFY && vic < sizeof(verification_inputs)/sizeof(inputs)) {
	RESET = verification_inputs[vic].RESET;
	STOP = verification_inputs[vic].STOP;
	RECORD = verification_inputs[vic].RECORD;
	STORE = verification_inputs[vic].STORE;
	PLAY_LIVE = verification_inputs[vic].PLAY_LIVE;
	PLAY_RECORDING = verification_inputs[vic].PLAY_RECORDING;
	currentFrequency = verification_inputs[vic].dial_frequency;
	vic++;
  } else if (VERIFY) { // this case runs if the counter reaches the end of the inputs
	RESET = 1;
	STOP = 0;
	RECORD = 0;
	STORE = 0;
	PLAY_LIVE = 0;
  	PLAY_RECORDING = 0;
  }

  if (RESET) {
  	if (VERIFY) cout << "Reset button asserted. " << endl;
    // check if the reset button is asserted. If the processor is already in idle, incremeet a reset timer
    // Otherwise, just enter the idle state
    if (state == idle) {
      resetTimer++;
    } else {
	  resetTimer = 0;
      state = idle;
    }
  } else {
    // If the processor does not get the reset signal anymore, reset the timer to 0.
    resetTimer = 0;
  }

  if (STOP) {
    if (VERIFY) cout << "Stop button asserted. " << endl;
    state = stop;
  }
  if (RECORD) {
    if (VERIFY) cout << "Record button asserted. " << endl;
    state = record;
    //uLCD_main.print("INPUT NOTE USING DIAL");
  }
  if (PLAY_LIVE) {
  	if (VERIFY) cout << "Play Live asserted." << endl;
    state = play_live;
  }
  if (PLAY_RECORDING) {
  	if (VERIFY) cout << "Play recording asserted." << endl;
    state = play_recording;
  }
  if (VERIFY && !RESET && !STOP && !RECORD && !PLAY_LIVE && !PLAY_RECORDING && !STORE) cout << "No button asserted." << endl;
}




// This function is called while the reset button is asserted.
void idle_event() {

  if (DEBUG) cout << "idle func" << endl;
  motor_1_angle = 0, motor_2_angle = 0, motor_1_velocity = 0, motor_2_velocity = 0, speaker_out = 0;
  vectorPosition = 0;
  playingTimer = 0;
  currentFrequency = 0;
  storeButtonHeldDown = 0;
  // Above several variables are set to initial values (motors must stop, speaker must stop producing sound)

  if (resetTimer > 2) { // Number here is arbitrary, would need to be tested to find a suitable number
    // Alternatively, find a way to use a real timer and have it run for 3 seconds
    freqVector.clear();
    resetTimer = 0;
  }

}




// This function runs when stop button is asserted. Simply stops the playback/dancing exactly where it is
void stop_event() {
  // Does not reset motors to initial angles, nor does it reset playback state
  motor_1_velocity = 0, motor_2_velocity = 0, speaker_out = 0;
  playingFrequency = 0;

}



// Function occurs while in record state
void record_event() {
  if (DEBUG) cout << "record func" << endl;
  //if (STORE && !storeButtonHeldDown) {
  if (STORE) {
    storeButtonHeldDown = 1;
    freqVector.push_back(readFrequencyInput());
	if (VERIFY) cout << "Store button asserted. Stored Frequency: " << currentFrequency << "." << endl;
  } else {
    storeButtonHeldDown = 0;
  }
}




// Function occurs while in the play live state
void play_live_event() {

  if (DEBUG) cout << "play live func" << endl;
  // If the store button is asserted, robot should begin to produce the selected frequency, and move the motors to the correspodnding position
  if (STORE) {
    playingFrequency = readFrequencyInput();
	if (VERIFY) cout << "Store button asserted (frequency dial = " << currentFrequency << ")." << endl;

    // Set frequency set to speaker
    // TODO Define these functions
    playTone(currentFrequency);
    // Motor instructions
    moveMotor1(currentFrequency);
    moveMotor2(currentFrequency);

  }
}




// Function occurs while in the play recording state
void play_recording_event() {
  
  if (DEBUG) cout << "play record func" << endl;

  //If note has been playing too long, move onto the next one
  // Just as with the other timer, this is just a placeholder value for now (will likely need to implement a real timer later)
  if (playingTimer > 200 || VERIFY) { 
  	playingFrequency = freqVector[vectorPosition];
    if (freqVector.size() - 1 == vectorPosition) {
      vectorPosition = 0; // Don't try to read past the size, just loop back to start
    } else {
      vectorPosition++;
    }  
    playingTimer = 0;
  }
  
  // Move accordingly
  playTone(freqVector[vectorPosition]);
  moveMotor1(freqVector[vectorPosition]);
  moveMotor2(freqVector[vectorPosition]);
}





void printState() {
	// Print state
	// Print frequency vector
	// If speaker is playing state its frequency
	cout << vic << " - ";
	cout << "State: ";
	switch (state) {
		case idle:
			cout << "idle. ";
			break;
		case stop:
			cout << "stop. ";
			break;
		case record:
			cout << "record. ";
			break;
		case play_live:
		    if (playingFrequency) {
				cout << "playing live with frequency " << playingFrequency;
			} else {
				cout << "play live state (no given frequency yet)";
			}
			break;
		case play_recording:
			cout << "playing recording with frequency " << playingFrequency;
			break;
	}
	cout << endl;
	if (freqVector.size()) {
		cout << "Frequency Vector Memory: [" << freqVector[0];
		for (int i = 1; i < freqVector.size(); ++i) {
			cout << ", " << freqVector[i];
		}
		cout << "]. " << endl;
	}
}






int main() {

  // Initial variable declarations
  state = idle, resetTimer = 0, motor_1_angle = 0, motor_2_angle = 0, motor_1_velocity = 0, motor_2_velocity = 0, speaker_out = 0;
  vectorPosition = 0;
  playingTimer = 0;
  currentFrequency = 0;
  storeButtonHeldDown = 0;
  playingFrequency = 0;

	verification_inputs[0] = {0,0,0,0,0,0,1};
	verification_inputs[2] = {0,0,1,0,0,0,1}; // Recording
	verification_inputs[2] = {0,0,0,1,0,0,1}; // Store 1
	verification_inputs[4] = {0,0,0,0,0,0,1};
	verification_inputs[5] = {0,0,0,1,0,0,2}; // Store 2
	verification_inputs[6] = {0,0,0,0,0,0,1};
	verification_inputs[6] = {0,0,0,1,0,0,3}; // Store 3
	verification_inputs[7] = {0,1,0,0,0,0,1}; // Stop
	verification_inputs[8] = {0,0,0,0,0,0,1};
	verification_inputs[9] = {0,0,0,0,0,1,1}; // Play recording
	verification_inputs[10] = {0,0,0,0,0,0,1};
	verification_inputs[11] = {0,0,0,0,0,0,1};
	verification_inputs[12] = {0,0,0,0,0,0,1};
	verification_inputs[13] = {0,0,0,0,0,0,1};
	verification_inputs[14] = {0,0,1,0,0,0,3}; // Recording
	verification_inputs[15] = {0,0,0,1,0,0,3}; // Store 3
	verification_inputs[16] = {0,0,0,1,0,0,2}; // Store 2
	verification_inputs[17] = {0,1,0,0,0,0,1}; // Stop
	verification_inputs[18] = {0,0,0,0,1,0,4}; // Play Live
	verification_inputs[19] = {0,0,0,1,0,0,4}; // Play 4
	verification_inputs[20] = {0,0,0,0,0,0,4}; 
	verification_inputs[21] = {0,0,0,1,0,0,8}; // Play 8
	verification_inputs[22] = {0,0,0,0,0,0,8};
	verification_inputs[23] = {0,0,0,0,0,0,8};



  vic = 0;
  while (true) {
  //cout << "vic: " << vic << endl;

    // Parse the buttons each to determine state, then switch based on state
    checkButtons();
    


    if (state != idle) {
      resetTimer = 0;
    }

    switch (state) {
    case idle:
      idle_event();
      break;
    case stop:
      stop_event();
      break;
    case record:
      record_event();
      break;
    case play_live:
      play_live_event();
      break;
    case play_recording:
      play_recording_event();
      break;
    }

	if (VERIFY) {
		printState();
		getchar();
	}
  }

  return 0;
}
