while(1){
    if(!playLive_switch){   //if switch is in OFF mode.
    return null;    // do nothing
    }// end of if

    else{
        Frequency current = readFrequencyInput(); //Have a function where it reads in the value from the DIAL
        wait(0.2)
        bool dance = pbDANCE;// Assign the pushbuttton DANCE to dance
        if(current.valid = 0 && playLive_switch == 1) { // if the frequency is NOT valid and the switch is on
            uLCD_smile.printf(SADFACE); // print a sad face to smiley LCD
            wait(0.1);
            uLCD_main.printf("Invalid Input"); //print ERROR MESSAGE to main LCD.
            }// end of no good if
            if(current.valid = 1 && !dance && playLive_switch == 1){ //Everything is active except for the Dance Button
                uLCD_smile.printf(SMILEFACE); //print a SMILE 
                wait(0.1);
                uLCD_main.print("Hold DANCE button \n \n"); //print ERROR MESSAGE to main LCD
                }//end of almost good if
                if(current.valid = 1 && dance = TRUE && playLive_switch == 1){ //Everything is active
                     float input = current.get();
                    switch(input){//Do the switch statements
                        case note1: Motor.move(note1); //take the input and move the motor accordingly
                        break;
                        case note2: Motor.move(note2);
                        break;
                        case note3: Motor.move(note3);
                        break;
                        case note4: Motor.move(note4);
                        break;
                        case note5: Motor.move(note5);
                        break;
                        case note6: Motor.move(note6);
                        break;
                        case note7: Motor.move(note7);
                        break;
                        default: uLCD_main.printf("Impossible");
                    }//end of Switch
                  }//end of good if 
        }// end of else_main
}// end of while_main
