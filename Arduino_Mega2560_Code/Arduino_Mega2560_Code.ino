#include <Wire.h>
#include <Adafruit_INA219.h>
#include <LiquidCrystal.h>
const int rs = 10, en = 9, d4 = 8, d5 = 7, d6 = 6, d7 = 5;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int stepPin = 3,dirPin = 4;


volatile byte half_revolutions = 0, number_of_pulses = 0;
unsigned int vibration_value = 0;
unsigned long timeold = 0, PreviousMillis9 = 0, PreviousMillis10 = 0;


Adafruit_INA219 sensor219;

float temp = 0, busVoltage = 0, current = 0, power = 0;
int16_t vib, rpm;

void setup()
{
attachInterrupt(digitalPinToInterrupt(2), Vibration_counter, RISING);
attachInterrupt(digitalPinToInterrupt(18), RPM_counter, RISING);
pinMode(stepPin, OUTPUT);
pinMode(dirPin, OUTPUT);
pinMode(18, INPUT_PULLUP);
lcd.begin(16, 2);
Serial.begin(9600);    
sensor219.begin();
Wire.begin(54);
Wire.onRequest(requestEvent);
}
 
void loop()
{
Current_S();
Temperature_S();
Vibration_S();
RPM_S();
Motor();
}

void Current_S(){
  static unsigned long PreviousMillis0 = 0;
  if(millis() - PreviousMillis0 > 2000){
  
  //busVoltage = sensor219.getBusVoltage_V();
  current = sensor219.getCurrent_mA();
  //power = busVoltage * (current/1000);
  
  
  /* 
  Serial.print("Bus Voltage:   "); 
  Serial.print(busVoltage); 
  Serial.println(" V");  
  
  Serial.print("Current:       "); 
  Serial.print(current); 
  Serial.println(" mA");

  Serial.print("Power:         "); 
  Serial.print(power); 
  Serial.println(" W");  
  Serial.println("");  
*/
  lcd.setCursor(0, 0);
  lcd.print("I=");
  lcd.print(current);
  //lcd.print("mA");
  

  PreviousMillis0 = millis(); 
  }
  }

void Temperature_S(){
    static unsigned long sensortStamp = 0;
  
  if(millis() - sensortStamp > 1000){
    sensortStamp = millis();
    // read the LM35 sensor value and convert to the degrees every 100ms.
    int reading = analogRead(A7); //note that we connect the value end of LM35 to A0 pin
    reading = analogRead(A7);
    temp = reading *0.48828125;
    //Serial.print("Real Time Temp: "); 
    //Serial.println(temp);
    lcd.setCursor(8, 0);
    lcd.print("T=");
    lcd.print(temp); 
  }
  }

  void Vibration_counter()
  {
  number_of_pulses++;
  }

  void Vibration_S(){
  if (number_of_pulses >= 3){ 
     vibration_value = number_of_pulses*10/(millis() - PreviousMillis9);
     PreviousMillis9 = millis();
     number_of_pulses = 0;
     //lcd.setCursor(8, 1);
     //lcd.print(vibration_value);
   }
    if(vibration_value>1 && vibration_value<5){
    lcd.setCursor(8, 1);
    lcd.print("LOW_VIB  ");
    vib=1;
    }
   else if(vibration_value>5){
    lcd.setCursor(8, 1);
    lcd.print("HIGH_VIB  ");
    vib=2;
    }
   
    if(millis() - PreviousMillis9 > 500){
    //Serial.println("0");
    lcd.setCursor(8, 1);
    lcd.print("NON_VIB  ");
    vib=0;
    PreviousMillis9 = millis(); 
    }
  }

  void RPM_counter()
 {
   half_revolutions++;
 }

 void RPM_S(){
  if (half_revolutions >= 3) { 
     rpm = 30*1000/(millis() - timeold)*half_revolutions;
     timeold = millis();
     half_revolutions = 0;
     //Serial.println(rpm);
     lcd.setCursor(0, 1);
     lcd.print("RPM=");
     lcd.print(rpm);
   }
   if(millis() - timeold > 2000){
    //Serial.println("0");
    lcd.setCursor(0, 1);
    rpm = 0;
    lcd.print("RPM=0   ");
    timeold = millis(); 
    }
  }

  void Motor(){

  static unsigned long PreviousMillis11 = 0;  
  static unsigned long PreviousMillis12 = 0;
    
  digitalWrite(dirPin,LOW);
  for(int x=0; x < 200; x++) {
      if(micros() - PreviousMillis11 > 1500){
  PreviousMillis11 = micros();
  digitalWrite(stepPin,HIGH);
  }
      if(micros() - PreviousMillis12 > 1500){
  PreviousMillis12 = micros();
  digitalWrite(stepPin,LOW);
  }
  } 
}

 
void requestEvent()
{
int16_t tempNum = temp*100.0;
int16_t currentNum = current*100.0;
byte tempArray[2];
byte rpmArray[2];
byte currentArray[2];
byte vibArray[2];
 
tempArray[0] = (tempNum >> 8) & 0xFF;
tempArray[1] = tempNum & 0xFF;
rpmArray[0] = (rpm >> 8) & 0xFF;
rpmArray[1] = rpm & 0xFF;
currentArray[0] = (currentNum >> 8) & 0xFF;
currentArray[1] = currentNum & 0xFF;
vibArray[0] = (vib >> 8) & 0xFF;
vibArray[1] = vib & 0xFF;
Wire.write(tempArray, 2);
Wire.write(rpmArray, 2);
Wire.write(currentArray, 2);
Wire.write(vibArray, 2);
}
