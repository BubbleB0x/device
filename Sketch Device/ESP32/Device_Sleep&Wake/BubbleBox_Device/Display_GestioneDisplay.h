//--------------------------DISPLAY SETUP ON/OFF--------------------------------------------------
//--------------------------------------------------------------------------------

/*
 * Librerie: https://github.com/ThingPulse/esp8266-oled-ssd1306
 * 
 * Connettere Il Display all'ESP32
 *    
 *    DISPLAY   -    ESP32
 *      VCC           3V
 *      GND          GND
 *      SDA        GPIO 22
 *      SCL        GPIO 21
 */

 
// Librerie Displey OLED
#include <Wire.h>                                       // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h"                                // legacy: #include "SSD1306.h"
SSD1306Wire display(0x3c, 21, 22);

//--------------- Visualizzazione sul display principale del contatto avvenuto con un altro device nelle vicinanze -----------------
void displayBubbleBoxContatto()
{
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_10);
  display.drawXbm(0, 0, 13, 12, Bubble_Trovato_bits);
  display.drawString(15, 0, " +");
  display.display();
}

//-------------------- Accensione del display e visualizzazione della data/ora e altre schermate in base alla variabile numeroDisplay
//--- accendiDisplay(Data da inserire | Ora da inserire | Temperatura da inserire | Numero contatti da inserire)
void accendiDisplay(String Data, String Ora, String Temp, String NContatti)
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
    display.drawXbm(18, 5, 95, 50, LogoBubble_bits);
    display.setFont(ArialMT_Plain_10);
    display.display();
    delay(4000);
    ++numeroDisplay;
    ControlTimeWake = 14;
    break;
    
    case 1:                                               // Schermata principale con data e ora
    display.displayOn();
    display.clear();
    display.setFont(ArialMT_Plain_10);
    display.drawString(110, 0, Temp);
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.setFont(ArialMT_Plain_24);
    display.drawRect(25, 15, 80, 35);
    display.drawString(64, 20, Ora);
    display.setFont(ArialMT_Plain_10);
    display.drawString(64, 50, Data);
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
    display.drawString(64, 20, NContatti);
    display.setFont(ArialMT_Plain_10);
    display.drawString(64, 50, "PERSONE INCONTRATE");
    display.display();
    break; 

    case 3:                                                 // Connessione ad una rete WIFI
    display.displayOn();
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.drawXbm(34, 14, 60, 36, WiFi_Logo_bits);
    display.setFont(ArialMT_Plain_10);
    display.drawString(64, 50, "CONNESSIONE WIFI");
    display.display();
    break;

    case 4:                                                // Connessione ad uno smartphone per lo scambio dati
    display.displayOn();
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.drawXbm(45, 2, 40, 50, Smartphone_Logo_bits);
    display.setFont(ArialMT_Plain_10);
    display.drawString(64, 50, "CONNECT SMARTPHONE");
    display.display();
    break;

    case 5:                                                 // Connessione ad una station --> BUBBLE STATION/SMARTPHONE STATION
    display.displayOn();
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.drawXbm(40, 1, 50, 50, Station_Logo_bits);
    display.setFont(ArialMT_Plain_10);
    display.drawString(64, 50, "CONNESSIONE A.P.");
    display.display();
    break;

    case 6:                                                 // QRcode device
    display.displayOn();
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.drawXbm(40, 2, 50, 50, QRCode_bits);
    display.setFont(ArialMT_Plain_10);
    display.drawString(64, 50, "SCAN QR-COde");
    display.display();
    break;

    case 7:                                                 // Connessione WIFI WPS --> Richiesta di attivazione bottone WPS sull'AP
    display.displayOn();
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.drawXbm(40, 1, 50, 53, WPS_Logo_bits);
    display.setFont(ArialMT_Plain_10);
    display.drawString(64, 50, "ATTIVA WPS");
    display.display();
    break;

    case 8:                                                  // Connessione smartphone e le credenziali di accesso e connessione
    display.displayOn();
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.setFont(ArialMT_Plain_10);
    display.drawString(64, 0, "Connetti Smartphone");
    display.drawString(64, 12, "Tramite Bluetooth");
    display.drawString(64, 25, "NOME DEVICE:");
    display.setFont(ArialMT_Plain_10);
    display.drawString(64, 40, "[BubbleBox_Device | BubbleBox]");
    display.display();
    break;

    case 9:                                                  // Schermata di invio dati al server
    display.displayOn();
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.drawXbm(35, 3, 65, 50, WiFiSend_bits);
    display.setFont(ArialMT_Plain_10);
    display.drawString(64, 50, "INVIO DATI...");
    display.display();
    break;

    case 10:                                                 // Connessione ad una station --> BUBBLE STATION/SMARTPHONE STATION
    display.displayOn();
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.drawXbm(40, 1, 50, 50, Station_Logo_bits);
    display.setFont(ArialMT_Plain_10);
    display.drawString(64, 50, "RICERCA...");
    display.display();
    break;

    /* ------- PROBLEMA DI GESTIONE DISPLAY --> IL DISPLAY NON REFRESHA COME SI DEVE PER FAR VISUALIZZARE LA SCHERMATA --> <DA RISOLVERE!>
     *  PER IL MOMENTO NIENTE VISUALIZZAZIONE SCHERMO DURANTE LA CONNESSIONE E L'INVIO DATI ALLO SMARTPHONE
     *  
    case 8:                                                 // Invio dati smartphone
    display.displayOn();
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.setFont(ArialMT_Plain_10);
    display.drawString(64, 0, "< CONNESSO >");
    display.drawString(64, 12, "Tramite Bluetooth");
    display.setFont(ArialMT_Plain_10);
    display.drawString(64, 40, "INVIO DATI >>>");
    display.display();
    break;*/
  }
}

//---------------------------- Spegnimento diplay ---------------------------------------------------
void spegniDisplay()
{
  smartphoneConnect = false;
  bubbleStation = false;
  statoBLT = false;
  numeroDisplay = 1;
  display.displayOff();
}

//---------------- Funzione Interrupt per il cambio dello schermo del displey -------------------------
void IRAM_ATTR controlloCambioDisplay()
{
  smartphoneConnect = false;
  bubbleStation = false;
  statoBLT = false;
  ++numeroDisplay;
  ControlTimeWake = 0; 
  if(numeroDisplay > 6)
   {
     numeroDisplay = 1;  
   }
}
