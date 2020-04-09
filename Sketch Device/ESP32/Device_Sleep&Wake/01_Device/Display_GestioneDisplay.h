//--------------------------DISPLAY SETUP ON/OFF--------------------------------------------------
//--------------------------------------------------------------------------------

/*
 * Librerie: https://github.com/ThingPulse/esp8266-oled-ssd1306
 */

 
// Librerie Displey OLED
#include <Wire.h>               // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h"        // legacy: #include "SSD1306.h"
SSD1306Wire display(0x3c, 21, 22);

void displayBubbleBoxContatto()
{
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_10);
  display.drawXbm(0, 0, 13, 12, Bubble_Trovato_bits);
  display.drawString(15, 0, " +");
  display.display();
}

//-------------------- Accensione del display e visualizzazione della data/ora e altre schermate in base alla variabile numeroDisplay
void accendiDisplay()
{
  if(bootCount > 0 && numeroDisplay == 0)               // La schermata di accensione non deve essere più impostata una volta che il device è entrato in funzione
  {
    ++numeroDisplay;
  }
  
  switch (numeroDisplay)                                // Controllo numero di schermata da visualizzare
  {
    case 0:                                             // Logo iniziale ----> Solo in accensione
    display.displayOn();
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.drawXbm(42, 2, 50, 50, Logo_Init_bits);
    display.setFont(ArialMT_Plain_10);
    display.drawString(64, 50, "BUBBLE BOX");
    display.display();
    delay(4000);
    ++numeroDisplay;
    ControlTimeWake = 14;
    break;
    
    case 1:                                               // Schermata principale con data e ora
    display.displayOn();
    display.clear();
    // display.drawXbm(95, 0, 19, 13, Wave_Logo_bits);
    display.drawXbm(115, 0, 13, 13, Bluetooth_Logo_bits);
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.setFont(ArialMT_Plain_24);
    display.drawRect(25, 15, 80, 35);
    display.drawString(64, 20, "00:00");
    display.setFont(ArialMT_Plain_10);
    display.drawString(64, 50, "01-01-2000");
    display.display();
    if(BubbleBoxTrovato)
    {
      displayBubbleBoxContatto();
    }
    break;

    case 2:                                                // Numero di persone incontrate in giornata
    display.displayOn();
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.setFont(ArialMT_Plain_24);
    display.drawCircle(64, 20, 30);
    display.drawString(64, 20, "10");
    display.setFont(ArialMT_Plain_10);
    display.drawString(64, 50, "PERSONE INCONTRATE");
    display.display();
    break; 

    case 3:                                                 // Connessione ad una Bubble Station nei dintorni
    display.displayOn();
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.drawXbm(40, 2, 50, 50, BLT_Logo_bits);
    display.setFont(ArialMT_Plain_10);
    display.drawString(64, 50, "CONNESSIONE BLT");
    display.display();
    break;

    case 4:                                                // Connessione ad uno smartphone per lo scambio dati
    display.displayOn();
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.drawXbm(50, 2, 32, 60, Smartphone_Logo_bits);
    display.setFont(ArialMT_Plain_10);
    display.display();
    break;

    case 5:                                                 // Connessione e invio dati BubbleStation
    display.displayOn();
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.drawXbm(40, 2, 50, 50, BLTSend_Logo_bits);
    display.setFont(ArialMT_Plain_10);
    if(bubbleStation)
    {
      display.drawString(64, 50, "INVIO DATI...");
    }
    else
    {
      display.drawString(64, 50, "CONNESSIONE...");
    }
    display.display();
    break;

    case 6:                                                  // Connessione smartphone e le credenziali di accesso e connessione
    display.displayOn();
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.setFont(ArialMT_Plain_10);
    display.drawString(64, 0, "Connetti Smartphone");
    display.drawString(64, 12, "Tramite Bluetooth");
    display.drawString(64, 25, "NOME DEVICE:");
    display.setFont(ArialMT_Plain_10);
    display.drawString(64, 40, "[BubbleBox_Device]");
    display.display();
    break;

    case 7:                                                   // Invio dati smartphone --> DA IMPLEMENTARE!
    display.displayOn();
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.drawXbm(40, 2, 50, 50, SmartphoneSend_Logo_bits);
    display.setFont(ArialMT_Plain_10);
    display.drawString(64, 50, "INVIO DATI...");
    display.display();
    break;
  }
}

//---------------------------- Spegnimento diplay
void spegniDisplay()
{
  smartphoneConnect = false;
  bubbleStation = false;
  statoBLT = false;
  numeroDisplay = 1;
  display.displayOff();
}

//---------------- Funzione Interrupt per il cambio dello schermo del displey
void IRAM_ATTR controlloCambioDisplay()
{
  smartphoneConnect = false;
  bubbleStation = false;
  statoBLT = false;
  ++numeroDisplay;
  ControlTimeWake = 0; 
  if(numeroDisplay > 4)
   {
     numeroDisplay = 1;  
   }
}
