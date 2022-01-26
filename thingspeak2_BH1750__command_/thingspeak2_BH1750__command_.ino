//Solar panel monitoring system with ThingSpeak
#include <Wire.h>        
#include <BH1750.h>      

BH1750 lightMeter;

#include <ESP8266WiFi.h>
#include "ThingSpeak.h"
#include "SDL_Arduino_INA3221.h"

char ssid[] = "Parametrik RnD";   // your network SSID (name)
char pass[] = "tabassam";   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

unsigned long myChannelNumber = 1564227;
const char * myWriteAPIKey = "COEIRTKGTY2TJ72X";

int number = 0;
int urut = 0;
int x;
float lux = 0;
int luxi = 0;
 
unsigned long lastMsg = 0;
#define SOLAR_CELL_CHANNEL 2
#define OUTPUT_CHANNEL 3

SDL_Arduino_INA3221 ina3221;

void setup() {
  Wire.begin();
  Serial.begin(9600);
  lightMeter.begin();
  while (!Serial) {
    ;
  }

  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);


 //Serial.println("SDA_Arduino_INA3221_Test");

  //Serial.println("Measuring voltage and current with ina3221 ...");
  ina3221.begin();

 // Serial.print("Manufactures ID=0x");
  int MID;
  MID = ina3221.getManufID();
  //Serial.println(MID, HEX);
}

void loop() {

 lux = lightMeter.readLightLevel();
 luxi = lux;
 // Serial.print("Light Meter: ");
 // Serial.print(lux);
 // Serial.println(" lx");
  delay(1000);
  
 //Serial.println("------------------------------");
  float shuntvoltage2 = 0;
  float busvoltage2 = 0;
  float current_mA2 = 0;
  float loadvoltage2 = 0;
  float solarcellpower2 = 0;

  busvoltage2 = ina3221.getBusVoltage_V(SOLAR_CELL_CHANNEL);
  shuntvoltage2 = ina3221.getShuntVoltage_mV(SOLAR_CELL_CHANNEL);
  current_mA2 = ina3221.getCurrent_mA(SOLAR_CELL_CHANNEL);
  loadvoltage2 = busvoltage2 + (shuntvoltage2 / 1000);
  solarcellpower2 = current_mA2 * busvoltage2;

  //Serial.print("Solar Cell Current 2:       "); Serial.print(current_mA2); Serial.println(" mA");
  //Serial.print("Solar Cell Power 2:       "); Serial.print(solarcellpower2); Serial.println(" W");
  //Serial.println("");

  float shuntvoltage3 = 0;
  float busvoltage3 = 0;
  float current_mA3 = 0;
  float loadvoltage3 = 0;

  busvoltage3 = ina3221.getBusVoltage_V(OUTPUT_CHANNEL);
  shuntvoltage3 = ina3221.getShuntVoltage_mV(OUTPUT_CHANNEL);
  current_mA3 = ina3221.getCurrent_mA(OUTPUT_CHANNEL);
  loadvoltage3 = busvoltage3 + (shuntvoltage3 / 1000);

  //Serial.print("Output Bus Voltage 3:   "); Serial.print(busvoltage3); Serial.println(" V");
 // Serial.print("Output Load Voltage 3:  "); Serial.print(loadvoltage3); Serial.println(" V");
 // Serial.print("Output Current 3:       "); Serial.print(current_mA3); Serial.println(" mA");

  if(WiFi.status() != WL_CONNECTED){
   // Serial.print("Attempting to connect to SSID: ");
  //  Serial.println("Parametrik_Prod");
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);
   //   Serial.print(".");
      delay(5000);
      Serial.print("t7.txt=\"Connecting\"");
      Serial.write(0xFF);
      Serial.write(0xFF);
      Serial.write(0xFF);
    }
   // Serial.println("\nConnected.");
   Serial.print("t7.txt=\"Connected\"");
   Serial.write(0xFF);
   Serial.write(0xFF);
   Serial.write(0xFF);

   Serial.print("t9.txt=\"Thingspeak\"");
   Serial.write(0xFF);
   Serial.write(0xFF);
   Serial.write(0xFF);

   
  }

unsigned long now = millis();
  if (now - lastMsg > 20000) {
    lastMsg = now;
    switch(urut){
      case 0:
        x = ThingSpeak.writeField(myChannelNumber, 1, current_mA2, myWriteAPIKey);
        if(x == 200){
        //Serial.println("Channel update successful.");
        luxi = current_mA2*100;
          Serial.print("x0.val=");
          Serial.print(luxi);
          Serial.write(0xFF);
          Serial.write(0xFF);
          Serial.write(0xFF);

          Serial.print("t11.txt=\"Success0\"");
          Serial.write(0xFF);
          Serial.write(0xFF);
          Serial.write(0xFF);
        }else{
          //Serial.println("Problem updating channel. HTTP error code " + String(x));
          Serial.print("t11.txt=\"Fail\"");
          Serial.write(0xFF);
          Serial.write(0xFF);
          Serial.write(0xFF);
        }
        break;
      case 1:
        x = ThingSpeak.writeField(myChannelNumber, 2, solarcellpower2, myWriteAPIKey);
        if(x == 200){
        //Serial.println("Channel update successful.");
        luxi = solarcellpower2*100;
          Serial.print("x1.val=");
          Serial.print(luxi);
          Serial.write(0xFF);
          Serial.write(0xFF);
          Serial.write(0xFF);
          
          Serial.print("t11.txt=\"Success1\"");
          Serial.write(0xFF);
          Serial.write(0xFF);
          Serial.write(0xFF);
        }else{
          //Serial.println("Problem updating channel. HTTP error code " + String(x));
          Serial.print("t11.txt=\"Fail\"");
          Serial.write(0xFF);
          Serial.write(0xFF);
          Serial.write(0xFF);
        }
        break;
      case 2:
       x = ThingSpeak.writeField(myChannelNumber, 3, busvoltage3, myWriteAPIKey);
       if(x == 200){
        //Serial.println("Channel update successful.");
        luxi = busvoltage3*100;
          Serial.print("x2.val=");
          Serial.print(luxi);
          Serial.write(0xFF);
          Serial.write(0xFF);
          Serial.write(0xFF);
          
      Serial.print("t11.txt=\"Success2\"");
          Serial.write(0xFF);
          Serial.write(0xFF);
          Serial.write(0xFF);
        }else{
          //Serial.println("Problem updating channel. HTTP error code " + String(x));
          Serial.print("t11.txt=\"Fail\"");
          Serial.write(0xFF);
          Serial.write(0xFF);
          Serial.write(0xFF);
        }
        break;
      case 3:
       x = ThingSpeak.writeField(myChannelNumber, 4, loadvoltage3, myWriteAPIKey);
       if(x == 200){
        //Serial.println("Channel update successful.");
        luxi = loadvoltage3*100;
          Serial.print("x3.val=");
          Serial.print(luxi);
          Serial.write(0xFF);
          Serial.write(0xFF);
          Serial.write(0xFF);

          Serial.print("t11.txt=\"Success3\"");
          Serial.write(0xFF);
          Serial.write(0xFF);
          Serial.write(0xFF);
        }else{
          //Serial.println("Problem updating channel. HTTP error code " + String(x));
          Serial.print("t11.txt=\"Fail\"");
          Serial.write(0xFF);
          Serial.write(0xFF);
          Serial.write(0xFF);
        }
        break;
      case 4:
        x = ThingSpeak.writeField(myChannelNumber, 5, current_mA3, myWriteAPIKey);
        if(x == 200){
        //Serial.println("Channel update successful.");
        luxi = current_mA3*100;
          Serial.print("x4.val=");
          Serial.print(luxi);
          Serial.write(0xFF);
          Serial.write(0xFF);
          Serial.write(0xFF);

          Serial.print("t11.txt=\"Success4\"");
          Serial.write(0xFF);
          Serial.write(0xFF);
          Serial.write(0xFF);
        }else{
          //Serial.println("Problem updating channel. HTTP error code " + String(x));
          Serial.print("t11.txt=\"Fail\"");
          Serial.write(0xFF);
          Serial.write(0xFF);
          Serial.write(0xFF);
        }
        break;
     case 5:
        x = ThingSpeak.writeField(myChannelNumber, 6, lux, myWriteAPIKey);
        if(x == 200){
        //Serial.println("Channel update successful.");
        luxi = lux*100;
          Serial.print("x5.val=");
          Serial.print(luxi);
          Serial.write(0xFF);
          Serial.write(0xFF);
          Serial.write(0xFF);

          Serial.print("t11.txt=\"Success5\"");
          Serial.write(0xFF);
          Serial.write(0xFF);
          Serial.write(0xFF);
        }else{
          //Serial.println("Problem updating channel. HTTP error code " + String(x));
          Serial.print("t11.txt=\"Fail\"");
          Serial.write(0xFF);
          Serial.write(0xFF);
          Serial.write(0xFF);
        }
        break;
      default :
      break;
    }

    if(urut > 4){
      urut = 0;
    }else{
      urut++;
    }
  }

  delay(20000);
}
