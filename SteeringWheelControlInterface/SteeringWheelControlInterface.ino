/*
  Original code: https://www.instructables.com/Digital-Potentiometer-MCP41100-and-Arduino/
  and https://forum.arduino.cc/t/steering-wheel-remote-audio-control/223878/6
  
  This code used to control the digital potentiometer
  MCP41100-E/P connected to  arduino Board.
  CS    >>> D10
  SCLK  >>> D13
  DI    >>> D11
  PBO, Audio jack GND   >>> GND
  PW0   >>> Pioneer radio over jack tip.
  5v    >>> Car's steering wheel input cable.
  A4    >>> Car's steering wheel output cable.
*/





#include <SPI.h>
byte address = 0x11;
int CS= 10;
int i=0;
int prevButton=0;
int steeringWheelPin=A4;  

//Button layout
const int VOL_UP=1;
const int VOL_DN=2;
const int PREV_TR=3;
const int NEXT_TR=4;
const int MODE=5;
const int MUTE=6;


void setup()
{
  pinMode (CS, OUTPUT);
  pinMode (steeringWheelPin, INPUT);

  Serial.begin(9600);
  SPI.begin();

delay(100);
}

void loop()
{
 int currButton=getR(); // get current pressed button code
  if (currButton!=prevButton) { // if it has changed since last reading
    delay(10);
    currButton=getR(); // wait 10ms and read again to make sure this is not just some noise
    if (currButton!=prevButton) { 
      //Serial.println(currButton);
      prevButton=currButton; 

      // send command to car stereo, below values are for Pioneer head units
     digitalPotWrite(0);
      switch(currButton) {
       case VOL_UP: digitalPotWrite(42); Serial.println("Volume Up"); break;  // 16kOhm
       case VOL_DN: digitalPotWrite(62); Serial.println("Volume Down"); break;  // 24k    
       case PREV_TR: digitalPotWrite(28); Serial.println("Previous Track"); break;  // 11k
       case NEXT_TR: digitalPotWrite(21); Serial.println("Next Track"); break;  // 8k
       case MODE: digitalPotWrite(4); Serial.println("Mode"); break;  // 1.6k
       case MUTE: digitalPotWrite(8); Serial.println("Mute"); break;  // 3k
       default: digitalPotWrite(0); break; // nothing
     }
   }
  }
  delay(25);
}


int getR(){
  int r=analogRead(steeringWheelPin);
// Serial.println(r);

  
   // Below values are for Mazda 6 gg steering wheel controls and 10 kOhms known resistor
  if (r>=507 && r<=509) return(VOL_UP);
  if (r>=510 && r<=511) return(VOL_DN);
  if (r>=496 && r<=498) return(PREV_TR);
  if (r>=502 && r<=505) return(NEXT_TR);
  if (r>=487 && r<=487) return(MODE);
  if (r>=465 && r<=465) return(MUTE); 
  return (0);
}




int digitalPotWrite(int value) //Function for MCP41100
{
  digitalWrite(CS, LOW);
  SPI.transfer(address);
  SPI.transfer(value);
  digitalWrite(CS, HIGH);
}
