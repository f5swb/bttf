#include "SevenSegmentTM1637.h"
#include "SevenSegmentExtended.h"
#include "RTClib.h"
#define showTime 1000 // Delay affichage Heure -> millis


// Sortie horloge Rouge
const byte PIN_CLK_Red = A0;   // define CLK pin  
// Sortie horloge Verte
const byte PIN_CLK_Green = A1;   // define CLK pin 
// Sortie horloge Orange
const byte PIN_CLK_Orange = A2;   // define CLK pin 

//RED Displays
const byte PIN_DIO_R1 = 3;
SevenSegmentExtended      red1(PIN_CLK_Red, PIN_DIO_R1);
const byte PIN_DIO_R2 = 4;
SevenSegmentTM1637       red2(PIN_CLK_Red, PIN_DIO_R2);
const byte PIN_DIO_R3 = 5;
SevenSegmentExtended     red3(PIN_CLK_Red, PIN_DIO_R3);

//GREEN Displays
const byte PIN_DIO_G1 = 6;
SevenSegmentExtended      green1(PIN_CLK_Green, PIN_DIO_G1);
const byte PIN_DIO_G2 = 7;
SevenSegmentTM1637       green2(PIN_CLK_Green, PIN_DIO_G2);
const byte PIN_DIO_G3 = 8;
SevenSegmentExtended     green3(PIN_CLK_Green, PIN_DIO_G3);
int greenAM = 12;
int greenPM = 13;

// ORANGE Displays
const byte PIN_DIO_O1 = 9;   // define DIO pin (any digital pin)
SevenSegmentExtended      orange1(PIN_CLK_Orange, PIN_DIO_O1); // SegmentExtended pour utiliser la methode "PrintTime"
const byte PIN_DIO_O2 = 10;   
SevenSegmentTM1637        orange2(PIN_CLK_Orange, PIN_DIO_O2); //SegmentTM1637 pour utiliser la methode "print" permet d'enlever les ":" entre les chiffres
const byte PIN_DIO_O3 = 11;   
SevenSegmentExtended       orange3(PIN_CLK_Orange, PIN_DIO_O3); 

bool parse=false;
bool config=false;
int Hour=0;
RTC_DS3231 rtc;
int backlight = 40;
int year_red = 2024;
int time_red = 0501;
int time_orange = 0101; 
int year_orange = 1988;
//// bouttons 
int buttonPinHour=2; // Heure +
///int buttonPinMin=3; // Min +

//// variables année mois date heure 
int yearupg;
int monthupg;
int dayupg;
int hourupg;
int minupg;
int secupg;


void setup() {

  pinMode(PIN_CLK_Red, OUTPUT);
  pinMode(PIN_CLK_Green, OUTPUT);
  pinMode(PIN_CLK_Orange, OUTPUT);
  pinMode(PIN_DIO_O1, OUTPUT);
  pinMode(PIN_DIO_O2, OUTPUT);
  pinMode(PIN_DIO_O3, OUTPUT);
  pinMode(PIN_DIO_G1, OUTPUT); 
  pinMode(PIN_DIO_G2, OUTPUT);
  pinMode(PIN_DIO_G3, OUTPUT); 
  pinMode(PIN_DIO_R1, OUTPUT);
  pinMode(PIN_DIO_R2, OUTPUT);
  pinMode(PIN_DIO_R3, OUTPUT);
  pinMode(greenAM, OUTPUT);
  pinMode(greenPM, OUTPUT);

  Serial.begin(9600);       // initializes the Serial connection @ 9600 baud
  orange1.begin();            // initializes the display
  orange2.begin();
  orange3.begin();              
  green1.begin();
  green2.begin();
  green3.begin();            
  red1.begin();
  red2.begin();
  red3.begin();            
  orange1.setBacklight(backlight);  // set the brightness to 100 %
  orange2.setBacklight(backlight);
  orange2.setColonOn(0); // Switch off ":" for orange "year"
  orange3.setBacklight(backlight);  
  green1.setBacklight(backlight); 
  green2.setBacklight(backlight);
  green3.setBacklight(backlight); 
  red1.setBacklight(backlight);
  red2.setBacklight(backlight);
  red3.setBacklight(backlight);  
  red2.setColonOn(0); // Switch off ":" for red "year"
  red1.setColonOn(0); // Switch off ":" for red "date"
  green1.setColonOn(0); // Switch off ":" for green "year"

 /* 
#ifndef ESP8266
  while (!Serial); // for Leonardo/Micro/Zero
#endif
*/

  Serial.begin(9600);
  delay(1000); // wait for console opening
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
  }
  //rtc.adjust(DateTime(2023, 3, 18, 11, 19, 30));
  
  
  // buttons 
  pinMode(buttonPinHour,INPUT);
  digitalWrite(buttonPinHour,HIGH);
  //pinMode(buttonPinMin,INPUT);
  //digitalWrite(buttonPinMin,HIGH);
}

void loop() {

  //Red displays -  Destination TIME 11.05 2028 10:04
  //red1.printTime(11, 9, true);
  red1.print(time_red, true);
  red2.print(year_red, true);
  red3.printTime(14, 07, true); 

  // Orange Displays - Last time departed 10.26 1985 01:24                      
  //orange1.printTime(11, 9, true);                     
  orange1.print(time_orange, true);      
  orange2.print(year_orange, true);
  orange3.printTime(17, 00, true);
                     

  // variables upgrade     
  DateTime now = rtc.now();
  yearupg=now.year();
  monthupg=now.month();
  dayupg=now.day();
  hourupg=now.hour();
  minupg=now.minute();
  secupg=now.second();


  if(now.hour()>=13){
      Hour=now.hour();
      digitalWrite(greenAM,0);
      digitalWrite(greenPM,1);}
  else if(now.hour()==12)
     {Hour=now.hour();
     digitalWrite(greenAM,0);
     digitalWrite(greenPM,1);}
  else{
      Hour=now.hour();
      digitalWrite(greenAM,1);
      digitalWrite(greenPM,0);
  }

// bouton heure    
  if(digitalRead(buttonPinHour)==0) {
      hourupg=hourupg+1;
      if(hourupg>=24){
          hourupg=00;
      }
  rtc.adjust(DateTime(yearupg, monthupg, dayupg, hourupg, minupg, secupg));
  }             

/*
// bouton minute    
     if(digitalRead(buttonPinMin)==0) {
         minupg=minupg+1;
         secupg=00;
         if(minupg>59){
            minupg=00;
         }         
         rtc.adjust(DateTime(yearupg, monthupg, dayupg, hourupg, minupg, secupg));
    }     
*/


//Green Displays - Present TIME - Heure actuelle
  green1.printTime(now.month(), now.day(), true);  
  green2.print(now.year());
//green3.printTime(Hour, now.minute(), true);

  green3.printTime(now.hour(), now.minute(), true);
}
