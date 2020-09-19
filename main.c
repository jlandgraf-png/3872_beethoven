// Generated by Proteus Visual Designer for Arduino

// Peripheral Configuration Code (Do Not Edit)
//---CONFIG_BEGIN---
#pragma GCC push_options
#pragma GCC optimize("Os")

#include <core.h> // Required by cpu

#include <cpu.h>

#include <Generic.h>

#include <iostream>

#include <vector>

#pragma GCC pop_options

// Peripheral Constructors
CPU & cpu = Cpu;
GenericButton RESET = GenericButton(2);
GenericButton STOP = GenericButton(2);
GenericButton RECORD = GenericButton(2);
GenericButton STORE = GenericButton(2);
GenericButton PLAY_LIVE = GenericButton(2);
GenericButton PLAY_RECORDING = GenericButton(2);

void peripheral_setup() {}

void peripheral_loop() {}

//---CONFIG_END---
// Flowchart Variables
String var_state;
long var_resetTimer;
float var_motor_1_angle;
float var_motor_2_angle;
float var_motor_1_velocity;
long var_motor_2_velocity;
float var_speaker_out;

// Other global variables
vector < double > freqVector;
int vectorPosition;
int playingTimer;
double currentFrequency;

// Flowchart Routines
void chart_SETUP() {

  // Initial variable declarations
  var_state = "idle", var_resetTimer = 0, var_motor_1_angle = 0, var_motor_2_angle = 0, var_motor_1_velocity = 0, var_motor_2_velocity = 0, var_speaker_out = 0;
  vectorPosition = 0;
  playingTimer = 0;
  currentFrequency = 0;

  while (true) {

    // Parse the buttons each to determine state, then switch based on state
    chart_checkButtons();

    switch (var_state) {
    case "idle":
      chart_idle_event();
      break;
    case "stop":
      chart_stop_event();
      break;
    case "record":
      chart_record_event();
      break;
    case "play live":
      chart_play_live_event();
      break;
    case "play recording":
      chart_play_recording_event();
      break;
    }
  }
}

// Parses buttons to change state. Assumes user does not press multiple buttons in one function call.
// If the user were to press multiple buttons, the button closest to the bottom would determine the state after the function returns.
void chart_checkButtons() {
  if (RESET()) {

    // check if the reset button is asserted. If the processor is already in idle, incremeet a reset timer
    // Otherwise, just enter the idle state
    if (var_state == "idle") {
      var_resetTimer++;
    } else {
      var_state = "idle";
    }
  } else {
    // If the processor does not get the reset signal anymore, reset the timer to 0.
    var_resetTimer = 0;
  }

  if (STOP()) {
    var_state = "stop";
  }
  if (RECORD()) {
    var_state = "record";
    uLCD_main.print("INPUT NOTE USING DIAL");
  }
  if (PLAY_LIVE()) {
    var_state = "play live";
  }
  if (PLAY_RECORDING()) {
    var_state = "play recording";
  }
}

// This function is called while the reset button is asserted.
void chart_idle_event() {
  var_resetTimer = var_resetTimer + 1, var_motor_1_angle = 0, var_motor_2_angle = 0, var_motor_1_velocity = 0, var_motor_2_velocity = 0, var_speaker_out = 0;
  vectorPosition = 0;
  playingTimer = 0;
  currentFrequency = 0;
  // Above several variables are set to initial values (motors must stop, speaker must stop producing sound)

  if (var_resetTimer > 200) { // Number here is arbitrary, would need to be tested to find a suitable number
    // Alternatively, find a way to use a real timer and have it run for 3 seconds
    freqVector.clear();
  }

}

// This function runs when stop button is asserted. Simply stops the playback/dancing exactly where it is
void chart_stop_event() {
  // Does not reset motors to initial angles
  var_motor_1_velocity = 0, var_motor_2_velocity = 0, var_speaker_out = 0;

  // Must also save current position in recording
}

// Function occurs while in record state
void chart_record_event() {
  // If the store button is asserted (positive edge only, must not activate several times for one press)
  // TODO check for store only on rising clock edge
  if (STORE()) {
    freqVector.pushback(readFrequencyInput());
  }
}

// Function occurs while in the play live state
void chart_play_live_event() {
  // If the store button is asserted, robot should begin to produce the selected frequency, and move the motors to the correspodnding position
  // (rising clock edge unimportant here)
  if (STORE()) {
    currentFrequency = readFrequencyInput();

    // Set frequency set to speaker
    // TODO Define these functions
    playTone(currentFrequency);
    // Motor instructions
    moveMotor1(currentFrequency);
    moveMotor2(currentFrequency);

  }
}

// Function occurs while in the play recording state
void chart_play_recording_event() {
  // Must do several things here
  // Processor must read the first value in the recording, begin playing through speaker and move the motors to the associated positions
  // Processor must have a timer for how long the recording has been playing. After some time, move to the next entry in the recording, and make the appropriate changes.
}

// Entry Points and Interrupt Handlers
void setup() {
  peripheral_setup();
  chart_SETUP();
}
void loop() {
  peripheral_loop();
}
