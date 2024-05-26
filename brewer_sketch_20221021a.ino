#include "max6675.h"

int ktcSO = 11;
int ktcCS = 13;
int ktcCLK = 12;

MAX6675 thermo(ktcCLK, ktcCS, ktcSO);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(500);
  // controll the pump
  pinMode(6,OUTPUT);
  // controll the heater
  pinMode(5,OUTPUT);
  // start the temp readings and the heater
  int temp = getTemp();
  heatOn();
}

void pumpOn() {
  Serial.println("pump on\n");
  digitalWrite(6,HIGH);
}

void pumpOff() {
  Serial.println("pump off\n");
  digitalWrite(6,LOW);
}

void heatOn() {
  Serial.println("heat on\n");
  digitalWrite(5,HIGH);
}

void heatOff() {
  Serial.println("heat off\n");
  digitalWrite(5,LOW);
}

int getTemp() {
  int temp = thermo.readFarenheit();
  Serial.println(temp);
  return temp;
}


int stabilize = 0;
bool brewing = false;

void loop() {
  // put your main code here, to run repeatedly:

  if(brewing == true) {
    Serial.println("now brewing...");
    delay(120000); // wait 2 minutes
    Serial.println("Done brewing. Good bye.\n");
    exit(0); // stop the program
  } else {
     
    // check the water temp
    int temp = getTemp();
  
    // if water is too hot, turn off heat and wait
    if(temp > 206) { 
      heatOff();
      stabilize = 0;
    }
  
    // if water temp is in the acceptable range,
    // turn off heat and let it stabilize. 
    if(temp > 185 && temp < 206 && stabilize < 10 && brewing == false) {
      Serial.println("Water in range, stabilizing...");
      Serial.print("stabilize = ");
      Serial.println(stabilize);
      heatOff();
      stabilize++;
    }// if water isn't hot enough, keep heat on
    else if(temp < 202 && brewing == false) {
      Serial.println("water too cool");
      heatOn(); 
      stabilize = 0;
    }
  
    // if the temp has been stable for 10 seconds between 203 and 205 degrees
    // start the brew
    if(temp > 202 && temp < 206 && stabilize >= 10 && brewing == false){  
      Serial.println("Good to go, start brewing!!\n");
      heatOff(); 
      pumpOn(); 
      brewing = true;
    }
  }
  delay(1000);
  
}
