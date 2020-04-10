/*
 * BubbleBox vers. 1.0 Pietro Rignanese & Andrea Polenta 2020
 * 
 * Contatti: 
 *  - pietro_rignanese@hotmail.it
 *  - andrea.polenta@outlook.it
 * 
 * #####################################---BUBBLEBOX---################################################
 * 
 * UNIVPM --> Progetto di Sistemi Operativi Distribuiti e Tempo Continuo 
 * 
 * Prof: Aldo Franco Dragoni
 * 
 */
 
//--- Nome, MAC Address, Rssi e Manifattura del dispositivo trovato -----------
String MAC;
String Name;
String Manufacturer;
int Rssi_device = 0;
String UUID;
//------------------------------------------------------------------------------

//--- MAC Address BLE di questo dispositivo-------------------------------------
String MyMAC;                         // Mio MAC Address BLE device ESP32

//---Variabile sul ritrovamento di altri device BUBBLEBOX-----------------------
bool BubbleBoxTrovato = false;        // Ritrovamento Device BubbleBox in zona

//---------- Caratteristiche Sleep&Wake-----------------------------------------
RTC_DATA_ATTR int bootCount = 0;      // Conteggio boot device
int ControlTimeWake = 0;              // Controllo tempo di Wake
const int RF_Nano = 25;               // Segnale di Controllo "RITROVAMENTO" device in zona --> Tiene accceso il BLE se si accorge della presenza di altri BUBBLEBOX

//------- Caratteristiche Display OLED 0.96"------------------------------------
const int statoDisplay = 14;          // Bottone per cambiare il display --> 4 tipologie di display
int numeroDisplay = 0;                // Numero del display corrispondente

//------- Caratteristiche Connessione BLT Seriale ------------------------------------
const int connBLT = 26;               // Bottone per la connessione BLT seriale
bool statoBLT = false;                // Bluetooth seriale attivato/disattivato
bool bubbleStation = false;           // BubbleStation rilevata
bool smartphoneConnect = false;       // Connessione Bluetooth Serial tramite smartphone attivata/disattivata

//------- Librerie realizzate "AD HOC" per la gestione dell'intero programma---------------------------------
#include "Display_IconeDisplay.h"    // Libreria per la gestione delle icone del display 
#include "Display_GestioneDisplay.h" // Libreria per la gestione del sisplay
#include "SD_GestioneSDCard.h"       // Libreria per la gestione dell'SD Card
#include "ESP32_BLEDevice.h"         // Libreria per il device Bluetooth ESP32
#include "ESP32_SleepWake.h"         // Libreria per lo Sleep&Wake ESP32
#include "ESP32_SerialBLT.h"         // Libreria per la connessione Bluetooth Seriale
#include "RTC_Clock.h"               // Libreria per ora e data


//------------------------- SETUP ----------------------------------------------------
void setup() 
{
  setSDCard();                      // Setto l'SD card
  
  BubbleBoxTrovato = false;         // setto la variabile del ritrovamento dispositivo a false

  // ------------ Settaggio bottone Interrupt per cambiare il display---------------------
  pinMode(statoDisplay, INPUT_PULLUP);
  attachInterrupt(statoDisplay, controlloCambioDisplay, HIGH);
  display.init();
  display.flipScreenVertically();
  //---------------------------------------------------------------------------------------

  // ------------ Settaggio bottone Interrupt per cambiare il display---------------------
  pinMode(connBLT, INPUT_PULLUP);
  attachInterrupt(connBLT, connessioneBLT, HIGH);
  //---------------------------------------------------------------------------------------

  //------------ Settaggio INPUT proveniente dall'RFNano per i dispositivi nei dintorni----
  pinMode(RF_Nano, INPUT);
  ControlTimeWake = 0;              // Time clock del programma (per tenere acceso lo schermo) settato a zero(0)
  
  Serial.begin(115200);
  
  enableBLE();                      // Abilitare il Bluetooth Low Energy
}

//-------------------------- LOOP ----------------------------------------------------------
void loop() 
{
  
  //---------- Controllo stato connessione Bluetooth Seriale, per connettere smartphone o bubblestation --> Disabilito tutto il resto del device 
  if(statoBLT)
  {
    accendiDisplay("", "", "");
    if(smartphoneConnect)
    {
      // ------ Connessione e invio dati allo smartphone
      sendDataSmartphone();
    }
    else
    {
      //------ Connessione e invio dati alla BubbleStation --->
      sendDataBLT();
    }
  }
  else
  {     
    ++ControlTimeWake;                                // Tempo di accensione del display va avanti di +1 ad ogni inizio loop
  
    accendiDisplay(getData(), getOra(), getTemp());   // ACCENDO IL DISPLAY --> Viene accesa la schermata in base al numero di tocchi del bottone
    
    scanArea();                                       // Scansione area per trovare i device con BLE nelle vicinanze
    
    disconnectedDeviceBLE();                          // Disconnesione dei dispositivi che si connettono --> Mantenere il device BLE esp32 sempre disponibile alla ricerca da parte di tutti gli altri device
  
    // Controllo per accendere o tener acceso il device in base a quanto tempo rimane in attesa o se non riceve nessun segnale dall'RFNANO
    // Controllo tempo di accensione e RF-NANO non trova device nei dintorni
    if(ControlTimeWake == 15 && digitalRead(RF_Nano) == LOW)
    {
      spegniDisplay();
      setSleepWake();
    }
    else
    {
      // Il tempo va avanti ma l'RF-NANO continua a trovare dispositivi nelle vicinanze
      if(digitalRead(RF_Nano) == HIGH)
      {
        ControlTimeWake = 0;
      }
    }
  }
}
