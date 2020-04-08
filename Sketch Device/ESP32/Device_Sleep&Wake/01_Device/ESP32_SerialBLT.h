/*
 * ------------ Serial Bluetooth Comunication ----------------
 */

#include "BluetoothSerial.h"

String Device;
String MACDevice;

BluetoothSerial ESP_BT; 

 void IRAM_ATTR connessioneBLT()
{
  if(numeroDisplay == 3)
  {
    Serial.println();
    Serial.println("--------- INVIO DATI BLT --->");
    ControlTimeWake = 0;
    numeroDisplay = 4;
    statoBLT = true;
  }
}

void sendDataBLT()
{
  Device = "";
  Device = scanArea();
  Serial.println(Device);
  if(Device != "")
  {
    Serial.println("DOPO SPLIT -->");
    MACDevice = Device.substring(14, 31);
    Device = Device.substring(0, 13);
    Serial.println(Device);
    if(Device == "BubbleStation")
    {
      bubbleStation = true;
      Serial.println("STATION TROVATA----INIZIO COMUNICAZIONE!");
      Serial.println(MACDevice);
      ESP_BT.begin("BubbleBox_Device");
      accendiDisplay();
      while(bubbleStation)
      {
        Serial.println("Bluetooth Device is Ready to Pair");
        if (ESP_BT.available()) //Check if we receive anything from Bluetooth
        {
          int incoming = ESP_BT.read(); //Read what we recevive
          Serial.print("Received:"); 
          Serial.println(incoming);
          ESP_BT.println("Digita 2 per chiudere tutto!");
          if(incoming == 50)
          {
            ESP_BT.println("Chiusura...");
            bubbleStation = false;
          }
        }
        delay(500);
      }
      ESP_BT.end();
      ControlTimeWake = 14;
      numeroDisplay = 1;
      statoBLT = false;
    }
  }
}
