  //setting up pins 
  const int buzzerPin = 3;
  const int greenPin = 4;                // green LED alarm is disarmed
  const int yellowPin = 5;               // Yellow LED will be the delay between when the system is armed and not
  const int redPin = 6;                  // Red is when the the alarm is armed
  const int pirPin = 7;                  //When PIR motion sensor and when it is triggered the LED light will turn on
  const int togglePin = 8;               //signifies opening and closing the door, one way the door will be shut and the next way the door will be open
  const int armPin = 9;                  // when push button is pressed, the yellow light should blink for the delay needed to get out of the room and then the LED should turn red
  const int keyPadPin[] = {10,11,12,13};  //Keypad pins for the pushbuttons to disarm the system 


  //Initialization of variables 
  bool pir = false;
  bool arm = false;
  bool toggle = false;
  bool keyPad[4];
  
  bool systemArmed = false;
  bool systemArmedWaiting = false;

  bool alarmActive = false;
  
  int armStartTime = 0;
  int alarmStartTime = 0;
  int armWaitTime = 5000;

  int state = 0;
  
  
void setup() {

  //setup code here, to run once:

  pinMode(buzzerPin, OUTPUT);
  pinMode(greenPin,OUTPUT);
  pinMode(yellowPin, OUTPUT);
  pinMode(redPin, OUTPUT);
  pinMode(pirPin, INPUT);
  pinMode(togglePin, INPUT);
  pinMode(armPin, INPUT);
  pinMode(keyPadPin[0],INPUT);
  pinMode(keyPadPin[1],INPUT);
  pinMode(keyPadPin[2],INPUT);
  pinMode(keyPadPin[3],INPUT);

  //This is this initial setup code. Initially when the arduino is turned on, it will flass Green, yellow, red, while the buzzer rings. Once the setup is complete the buzzer will sound off. 
  for(int i = 3; i <=13;i++){
    digitalWrite(i,LOW);
  }
  for(int i = 200; i<10000; i++){
    tone(buzzerPin,i);
    if(i<3500){
      digitalWrite(greenPin,HIGH);
      digitalWrite(yellowPin,LOW);
      digitalWrite(redPin,LOW);
    }else if(i<6500){
      digitalWrite(greenPin,LOW);
      digitalWrite(yellowPin,HIGH);
      digitalWrite(redPin,LOW);
    }else{
      digitalWrite(greenPin,LOW);
      digitalWrite(yellowPin,LOW);
      digitalWrite(redPin,HIGH);
    }
    
  }
  noTone(buzzerPin);
  digitalWrite(redPin,LOW);
}

void loop() {
  
  //reading in the digital data 
  arm = digitalRead(armPin);
  toggle = digitalRead(toggle);
  pir = digitalRead(pir);

  //Sets the green pin to high which signifies that the alarm is not active 
  if(!systemArmed&&!systemArmedWaiting){
    digitalWrite(greenPin,HIGH);
  }else{
    digitalWrite(greenPin,LOW);
  }
  
  //Code here fires when the arm pin is pressed.
  //When the push button is pressed, the green light turns to yellow (signifying a standby mode) and then the LED turns to red which means that the system is armed 
  if(arm && !(systemArmed||systemArmedWaiting)){
    systemArmedWaiting = true;
    armStartTime = millis();
  }

  if(systemArmedWaiting && (millis() - armStartTime)<5000){
    digitalWrite(yellowPin,HIGH);
  }else if(systemArmedWaiting && (millis()-armStartTime)>= 5000){
    systemArmed = true;
    systemArmedWaiting = false;
    digitalWrite(redPin,HIGH);
    digitalWrite(yellowPin,LOW);
  }

  // This part of code checks to see if either the toggle switch is HIGH or if the sensor is reading motion. 
  // If either of these are true the alarm will sound
  if(systemArmed && !alarmActive){
    alarmActive = (digitalRead(pirPin)==HIGH || digitalRead(togglePin)==HIGH);
    alarmStartTime = millis();
  }

  //This if statement calls the "soundAlarm()" method 
  //Sounding the alarm tone if the system reads motion or the toggle switch is HIGH when the alarm system is activated 
  if(alarmActive){
    soundAlarm();
  }

  //This is the disarming piece of code. 
  //Once the correct pin is pushed, the alarm stops and the red LED goes LOW and the system resets 
  if(digitalRead(keyPadPin[3]) == HIGH){
    alarmActive = false;
    systemArmed = false;
    noTone(buzzerPin);
    digitalWrite(redPin,LOW);
  }
}

//The sounding Alarm method. 
//This method produced a high pitch beeping sound which wont stop until the system is disarmed. 
void soundAlarm(){
  if((millis()- alarmStartTime)<500){
    tone(buzzerPin,2500);
    digitalWrite(redPin,LOW);    
  }else if((millis()-alarmStartTime)<1000){
    noTone(buzzerPin);
    digitalWrite(redPin,HIGH);
  }else{
    alarmStartTime = millis();
  }
}


