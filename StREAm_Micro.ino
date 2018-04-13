unsigned long previousKnock;
//unsigned long inRoomTime, outRoomTime; use these if two seperate cycles
static unsigned long avTimeBetweenKnocks;
int knockCounter;
boolean changeInDoorStatus;
boolean roomOccupied = false;
//unsigned long previousMillis = 0;
int piezoPin = A0;
int knockThresh = 25;
int settleTime = 500;

const int ledPin =  10;
void setup() {
  Serial.begin(9600);

  pinMode(ledPin, OUTPUT);
}

void loop() {

  //call to listen for a knock
  unsigned long knockTime = waitForKnock(piezoPin, knockThresh);
  //unsigned long currentTime=millis();
  if (knockTime > 0) { //if a knock is detected
    changeInDoorStatus = true;
  
    //only use this code for different cycles of on and off
    //   inBetweenKnocksTime=knockTime-previousKnock;
    //   previousKnock=knockTime;

    knockCounter++;

    // call to averageTime
    averageTimeBetweenKnocks(knockCounter ,  knockTime);
  }


//THE FOLLOWING COMMENTED CODE WAS MEANT TO BE USED AS THE "AI" ASPECT. WHERE THE USER'S SCHEDULE IS RECORDED AND THE ARDUINO CAN CONTROL THE LED BASED ON TIME
                                      //  if (knockCounter >= 2) { //after first knock, code changes slightly to incorporate time aspect
                                      //    Serial.print("Knock counter is greater than 1 at: ");
                                      //    Serial.println(knockCounter);
                                      //
                                      //    if (isItTime(currentTime, previousMillis, avTimeBetweenKnocks)) {//if rooms empty and someone enters OR if room is empty and time has passed
                                      ////     if (roomOccupied == false) {
                                      ////      roomOccupied = true;
                                      ////      changeInDoorStatus = false;
                                      ////     
                                      ////    }
                                      ////    else {
                                      ////      roomOccupied = false;
                                      ////      changeInDoorStatus = false;
                                      ////   
                                      ////    }
                                      //      roomOccupied = ! roomOccupied;
                                      //     changeInDoorStatus = ! changeInDoorStatus;//true
                                      //     Serial.print("previous Millis: ");
                                      //      Serial.println(previousMillis);
                                      //      previousMillis = knockTime;
                                      //      Serial.print("current Millis: ");
                                      //      Serial.println(knockTime);
                                      //      
                                      //    }
                                      //
                                      //    if (roomOccupied == false && changeInDoorStatus == true) {
                                      //      roomOccupied = true;
                                      //      changeInDoorStatus = false;
                                      //    }
                                      //
                                      //
                                      //    if (roomOccupied == true && changeInDoorStatus == false) { //someone is in room
                                      //      Serial.println("room Occupied");
                                      //      //light up led
                                      //      digitalWrite(ledPin, HIGH);
                                      //    }
                                      //
                                      //    //checks for person leaving the room
                                      //    if (roomOccupied == true && changeInDoorStatus == true) {
                                      //      roomOccupied = false;
                                      //      changeInDoorStatus = false;
                                      //    }
                                      //
                                      //    if (roomOccupied == false && changeInDoorStatus == false) { //no one in room
                                      //      Serial.println("room unoccupied");
                                      //      //turn off led
                                      //      digitalWrite(ledPin, LOW);
                                      //    }
                                      //
                                      //  }
                                        //else {//this is the first knock where time has no effect on the lights

    if (roomOccupied == false && changeInDoorStatus == true) {
      roomOccupied = true;
      changeInDoorStatus = false;
    }

    if (roomOccupied == true && changeInDoorStatus == false) { //someone is in room
      Serial.println("room Occupied");
      //light up led
      digitalWrite(ledPin, HIGH);
    }

    //checks for person leaving the room
    if (roomOccupied == true && changeInDoorStatus == true) {
      roomOccupied = false;
      changeInDoorStatus = false;
    }

    if (roomOccupied == false && changeInDoorStatus == false) { //no one in room
      Serial.println("room unoccupied");
      //turn off led
      digitalWrite(ledPin, LOW);
    }
  //}//end else

  waitForSettle(piezoPin, knockThresh, settleTime);
  Serial.print("average time between knocks: ");
  Serial.println(avTimeBetweenKnocks);



}//end loop()



//utility methods
unsigned long waitForKnock(int pin, int thresh) {
  while (true) {
    if (analogRead(pin) > thresh) {
      return millis();//time of the knock
    }
  }
}

void waitForSettle(int pin, int thresh, int settleTime) {
  unsigned long start = millis();
  while (true) {
    if (analogRead(pin) > thresh) {
      start = millis();
    }
    if (millis() - start > settleTime) {
      return;
    }
  }
}

//computes average time between any knocks
void averageTimeBetweenKnocks(int knockCounter, unsigned long knockTime) {

  avTimeBetweenKnocks = knockTime / (unsigned long)knockCounter;

  //would use this if wanting 2 seprate cycles of on and off
  //timeInRoomAverage = timeInRoomAverage*(unsigned long)(counter) + timeInRoom/(unsigned long)(counter+1);
}

//boolean isItTime(unsigned long knockTime, unsigned long  previousMillis, unsigned long avTimeBetweenKnocks) {
//  if (knockTime - previousMillis >= avTimeBetweenKnocks) {//if rooms empty and someone enters OR if room is empty and time has passed
//     return true;
//    }
//    else {
//      return false;
//    }
//}


