#include <Ethernet.h>
#include <SPI.h>
#include <UbidotsEthernet.h>
#include "cactus_io_SHT31.h"      //Temp & Humidity better library, DP already calculated
#include "Adafruit_CCS811.h"      //Air Quality: Carbon Dioxide(Co2) & Total Volatile Organic Compound
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

cactus_io_SHT31 sht31;
Adafruit_CCS811 ccs;
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); 

/********************************
 * Constants and objects
 *******************************/
/* Assigns the Ubidots parameters */
char const * TOKEN = "BBFF-pVeeegGakR6goT57w1zM9qz3Oy5Avn"; // Assign your Ubidots TOKEN
char const * VARIABLE_LABEL_1 = "Temperature"; // Assign the unique variable label to send the data
char const * VARIABLE_LABEL_2 = "Humidity"; // Assign the unique variable label to send the data
char const * VARIABLE_LABEL_3 = "Dew Point"; // Assign the unique variable label to send the data
char const * VARIABLE_LABEL_4 = "CO2"; // Assign the unique variable label to send the data
char const * VARIABLE_LABEL_5 = "VOC"; // Assign the unique variable label to send the data

/* Enter a MAC address for your controller below */
/* Newer Ethernet shields have a MAC address printed on a sticker on the shield */
byte mac[] = { 0xA8, 0x61, 0x0A, 0xAE, 0x88, 0x7C };

/* initialize the instance */
Ubidots client(TOKEN);

/********************************
 *        Main Functions
 *******************************/
void setup()
{
  Serial.begin(9600);
  lcd.begin(20,4); 

  Serial.print(F("Starting ethernet..."));
  if (!Ethernet.begin(mac))
  {
    Serial.println(F("failed"));
  } 
  else
  {
    Serial.println(Ethernet.localIP());
  }
  delay(1000);

  Serial.println("Environmental Sensor Test");

  if(!ccs.begin() || !sht31.begin()){
    Serial.println("Failed to start environmental sensor! Please check your wiring.");
    while(1);
  }
 
  while(!ccs.available());

}

/********************************
 *            Loop
 *******************************/

void loop()
{
  unsigned int dp = (sht31.getDewPoint() * 9/5) + 32;
  ccs.available();
  !ccs.readData();
  Ethernet.maintain();


  /* Sending values to Ubidots */
  client.add(VARIABLE_LABEL_1, sht31.getTemperature_F());
  client.add(VARIABLE_LABEL_2, sht31.getHumidity());
  client.add(VARIABLE_LABEL_3, (sht31.getDewPoint() * 9/5) + 32);
  client.add(VARIABLE_LABEL_4, ccs.geteCO2());
  client.add(VARIABLE_LABEL_5, ccs.getTVOC());
  client.sendAll();

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

      lcd.setCursor(0,0);                 //liquid crystal 20x4
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
      
  delay(60000);                     //Air Quality Sensor covers 250 ms, 1 sec, 10 sec, 60 sec samples
}
