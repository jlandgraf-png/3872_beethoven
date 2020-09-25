#include <iostream>
#include <vector>
#include <string>
#include<string.h>
#include<stdio.h>
using namespace std;

bool RESET;
bool STOP;
bool RECORD;
bool STORE;
bool PLAY_LIVE;
bool PLAY_RECORDING;

//Global String Variables for States


string curr_st; //empty string that stores the current state.


// Flowchart Variables
int state;
enum states { idle, stop, record, play_live, play_recording };
long resetTimer;
float motor_1_angle;
float motor_2_angle;
float motor_1_velocity;
long motor_2_velocity;
float speaker_out;

// Other global variables
vector<int> freqVector(1, 5);
int vectorPosition;
int playingTimer;
int currentFrequency;
int storeButtonHeldDown;


// Peripheral Routines

void moveMotor1(int freq) { cout << "Moving Motor 1" << endl; }
void moveMotor2(int freq) { cout << "Moving Motor 2" << endl; }
int readFrequencyInput() { return 0; }
void playTone(int freq) { cout<<"Playing a tone" << freq <<endl; }

// Function Declrations
void print_currState(string);


// Flowchart Routines

// Parses buttons to change state. Assumes user does not press multiple buttons in one function call.
// If the user were to press multiple buttons, the button closest to the bottom would determine the state after the function returns.
void checkButtons() {
    if (RESET) {

        // check if the reset button is asserted. If the processor is already in idle, incremeet a reset timer
        // Otherwise, just enter the idle state
        if (state == idle) {
            resetTimer++;
        }
        else {
            state = idle;
        }
    }
    else {
        // If the processor does not get the reset signal anymore, reset the timer to 0.
        resetTimer = 0;
    }

    if (STOP) {
        state = stop;
    }
    if (RECORD) {
        state = record;
        //uLCD_main.print("INPUT NOTE USING DIAL");
    }
    if (PLAY_LIVE) {
        state = play_live;
    }
    if (PLAY_RECORDING) {
        state = play_recording;
    }
}

// This function is called while the reset button is asserted.
void idle_event() {
    resetTimer = resetTimer + 1, motor_1_angle = 0, motor_2_angle = 0, motor_1_velocity = 0, motor_2_velocity = 0, speaker_out = 0;
    vectorPosition = 0;
    playingTimer = 0;
    currentFrequency = 0;
    storeButtonHeldDown = 0;
    // Above several variables are set to initial values (motors must stop, speaker must stop producing sound)

    if (resetTimer > 200) { // Number here is arbitrary, would need to be tested to find a suitable number
      // Alternatively, find a way to use a real timer and have it run for 3 seconds
        freqVector.clear();
        resetTimer = 0;
    }

}

// This function runs when stop button is asserted. Simply stops the playback/dancing exactly where it is
void stop_event() {
    // Does not reset motors to initial angles, nor does it reset playback state
    motor_1_velocity = 0, motor_2_velocity = 0, speaker_out = 0;

}

// Function occurs while in record state
void record_event() {
    if (STORE && !storeButtonHeldDown) {
        storeButtonHeldDown = 1;
        freqVector.push_back(readFrequencyInput());
    }
    else {
        storeButtonHeldDown = 0;
    }
}

// Function occurs while in the play live state
void play_live_event() {
    // If the store button is asserted, robot should begin to produce the selected frequency, and move the motors to the correspodnding position
    if (STORE) {
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
void play_recording_event() {

    //If note has been playing too long, move onto the next one
    // Just as with the other timer, this is just a placeholder value for now (will likely need to implement a real timer later)
    if (playingTimer > 200) {
        if (freqVector.size() - 1 == vectorPosition) {
            vectorPosition = 0; // Don't try to read past the size, just loop back to start
        }
        else {
            vectorPosition++;
        }
        playingTimer = 0;
    }

    // Move accordingly
    playTone(freqVector[vectorPosition]);
    moveMotor1(freqVector[vectorPosition]);
    moveMotor2(freqVector[vectorPosition]);
}


int main() {

    // Initial variable declarations
    state = idle, resetTimer = 0, motor_1_angle = 0, motor_2_angle = 0, motor_1_velocity = 0, motor_2_velocity = 0, speaker_out = 0;
    vectorPosition = 0;
    playingTimer = 0;
    currentFrequency = 0;
    storeButtonHeldDown = 0;

    while (true) {

        // Parse the buttons each to determine state, then switch based on state
        checkButtons();

        if (state != idle) {
            resetTimer = 0;
        }

        switch (state) {
        case idle:
            curr_st = "idle";
            print_currState(curr_st);
            idle_event();
            break;
        case stop:
            curr_st = "stop";
            print_currState(curr_st);
            stop_event();
            break;
        case record:
            curr_st = "record";
            print_currState(curr_st);
            record_event();
            break;
        case play_live:
            curr_st = "play live";
            print_currState(curr_st);
            play_live_event();
            break;
        case play_recording:
            curr_st = "play recording";
            print_currState(curr_st);
            play_recording_event();
            break;
        }
    }

    return 0;
}


void print_currState(string currentState) {
    cout << "Currently in state " << currentState << endl;
}//end of Function Definition