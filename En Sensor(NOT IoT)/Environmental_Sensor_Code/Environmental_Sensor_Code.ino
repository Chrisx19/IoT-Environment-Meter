#include "cactus_io_SHT31.h"      //Temp & Humidity
#include "Adafruit_CCS811.h"      //Air Quality: Carbon Dioxide(Co2) & Total Volatile Organic Compound
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

cactus_io_SHT31 sht31;            // or cactus_io_SHT31 sht31(0x45); // for the alternative address
Adafruit_CCS811 ccs;

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); 

void setup(){

Serial.begin(9600);
Serial.println("Environmental Sensor Test");
  lcd.begin(20,4); 

if(!ccs.begin() ||  !sht31.begin()){
  Serial.println("Failed to start environmental sensor! Please check your wiring.");
  while(1) delay(1);
}

  while(!ccs.available());
}

void loop(){
      int dp = (sht31.getDewPoint() * 9/5) + 32;

      ccs.available();
      !ccs.readData();

      Serial.print("Humidity: ");
      Serial.print(sht31.getHumidity());
      
      Serial.print("%\tTemp: ");
      Serial.print(sht31.getTemperature_F());
      Serial.print("*F\t   Dew Point: ");
      Serial.print((sht31.getDewPoint() * 9/5) + 32);
      Serial.print(" *F\t   ");
      
      Serial.print("CO2: ");
      Serial.print(ccs.geteCO2());
      Serial.print("ppm\t");
      Serial.print(" TVOC: ");
      Serial.print(ccs.getTVOC());
      Serial.println(" ppb");

      lcd.setCursor(0,0);
      lcd.print("T: ");
      lcd.print(sht31.getTemperature_F()); lcd.print((char) 223);
      lcd.print("F DP:  ");
      lcd.print(dp); lcd.print((char) 223); lcd.print("F");
      
      lcd.setCursor(0,1);
      lcd.print("Humid:    ");
      lcd.print(sht31.getHumidity());
      lcd.print("%");

      lcd.setCursor(0,2);      
      lcd.print("CO2  :    ");
      lcd.print(ccs.geteCO2());
      lcd.print(" ppm");
      
      lcd.setCursor(0,3); 
      lcd.print("TVOC :    ");
      lcd.print(ccs.getTVOC());
      lcd.print("   ppb");
      
delay(1000);
}
