
/*what's left
 * timeout protection
 * limits for any variable 
 */
// include the library code:
#include <LiquidCrystal.h>
#include <math.h>
int sensorPin = A0;    // select the input pin for the thermistor
int sensorValue = 0;  // variable to store the value coming from the sensor
double myTemperatureCelsius=0.0;
const int btnDec=2;
const int btnInc=3;
const int relayOut=13;
volatile int setPoint=70;
volatile int mode=0; // 0 for pla 1 for abs
int btnRead;
/* LCD RS pin 4 to digital pin 9          12
 * LCD Enable pin 6 to digital pin 8      11
 * LCD D4 pin 11 to digital pin 7          5
 * LCD D5 pin 12 to digital pin 6          4
 * LCD D6 pin 13 to digital pin 5          6
 * LCD D7 pin 14 to digital pin 4          7
 * LCD R/W pin 5 to ground
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
 * initialize the library with the numbers of the interface pins
 */
LiquidCrystal lcd(9, 8, 7, 6, 5, 4);
void setup() {
  lcd.begin(16, 2);
  lcd.print("V:");
  lcd.setCursor(7,0);
  
  // put your setup code here, to run once:
  pinMode(btnInc,INPUT);
  pinMode(btnDec,INPUT);
  pinMode(relayOut,OUTPUT);
  attachInterrupt(digitalPinToInterrupt(btnDec), pin_ISR_Dec, FALLING);
  attachInterrupt(digitalPinToInterrupt(btnInc), pin_ISR_Inc, FALLING); 


  
}
void loop() {
  lcd.setCursor(0, 1);
  lcd.print("T:"); 
  lcd.setCursor(2, 0);
  lcd.print(sensorValue);
  lcd.setCursor(7, 1);
  lcd.print(myTemperatureCelsius);
  lcd.setCursor(14, 1);
  lcd.print(setPoint);
  
  sensorValue = analogRead(sensorPin);
  myTemperatureCelsius = MyThermistorFunction(sensorValue);
  if(mode==0){
    setPoint=70;
  }

  else if(mode==1){
    setPoint=110;
  }
  //aggressive
  if(myTemperatureCelsius<setPoint)        
  {
    digitalWrite(relayOut,HIGH); 
  }
  else if(myTemperatureCelsius>setPoint)
  {
    digitalWrite(relayOut,LOW);
  }
}
double MyThermistorFunction(int RawADC){
    double Temp;
    //the original y intercept was 120, the inverse of the slop was 0.1465798046, was giving temp 32 C in room temperature around 27 "air conditioner reading"
    //Temp=(RawADC+90)*0.15;
    //Temp=(RawADC+120)*0.15; 
    
    if(RawADC>967)
    {
      Temp=(1081-RawADC)/3;  
    }
    else if((RawADC<=967)&&(RawADC>928))
    {
      Temp=(1101.7272727272-RawADC)/3.54545454;
    }
    else if((RawADC<=928)&&(RawADC>880))    
    {
      Temp=(1163.2-RawADC)/4.8;
    }
    else if((RawADC<=880)&&(RawADC>828))
    {
     Temp=(1186.8-RawADC)/5.2; 
    }
    else if((RawADC<=828)&&(RawADC>773))    
    {
      Temp=(1207.5-RawADC)/5.5;
    }
    else if(RawADC<=773)
    {
      Temp=(1262.8-RawADC)/6.2;
    }    
    return Temp;
}
void pin_ISR_Inc()
{
  mode=0;
}
void pin_ISR_Dec()
{
  mode=1;
}


