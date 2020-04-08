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
//------------------------------------------------------------------------------

//--- MAC Address BLE di questo dispositivo-------------------------------------
String MyMAC;

//---Variabile sul ritrovamento di altri device BUBBLEBOX-----------------------
bool BubbleBoxTrovato = false;

//---------- Caratteristiche Sleep&Wake-----------------------------------------
RTC_DATA_ATTR int bootCount = 0;      // Conteggio boot device
int ControlTimeWake = 0;              // Controllo tempo di Wake
const int RF_Nano = 25;               // Segnale di Controllo "RITROVAMENTO" device in zona --> Tiene accceso il BLE se si accorge della presenza di altri BUBBLEBOX

//------- Caratteristiche Display OLED 0.96"------------------------------------
const int statoDisplay = 14;          // Bottone per cambiare il display --> 4 tipologie di display
int numeroDisplay = 0;                // Numero del display corrispondente

//------- Caratteristiche Connessione BLT Seriale ------------------------------------
const int connBLT = 12;               // Bottone per la connessione BLT seriale
bool connessioneSerialBLT = false;

//------- Librerie realizzate "AD HOC" per la gestione dell'intero programma---------------------------------
#include "Display_IconeDisplay.h"    // Libreria per la gestione delle icone del display 
#include "Display_GestioneDisplay.h" // Libreria per la gestione del sisplay
#include "SD_GestioneSDCard.h"       // Libreria per la gestione dell'SD Card
#include "ESP32_BLEDevice.h"         // Libreria per il device Bluetooth ESP32
#include "ESP32_SleepWake.h"         // Libreria per lo Sleep&Wake ESP32
#include "ESP32_SerialBLT.h"         // Libreria per la connessione Bluetooth Seriale


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

  //------------ Settaggio INPUT proveniente dall'RFNano per i dispositivi nei dintorni----
  pinMode(RF_Nano, INPUT);
  ControlTimeWake = 0;              // Time clock del programma (per tenere acceso lo schermo) settato a zero(0)
  
  Serial.begin(115200);
  
  enableBLE();                      // Abilitare il Bluetooth Low Energy
}

//-------------------------- LOOP ----------------------------------------------------------
void loop() 
{
  ++ControlTimeWake;                // Tempo di accensione del display va avanti di +1 ad ogni inizio loop
  
  accendiDisplay();                 // ACCENDO IL DISPLAY --> Viene accesa la schermata in base al numero di tocchi del bottone
  
  scanArea();                       // Scansione area per trovare i device con BLE nelle vicinanze
  
  disconnectedDeviceBLE();          // Disconnesione dei dispositivi che si connettono --> Mantenere il device BLE esp32 sempre disponibile alla ricerca da parte di tutti gli altri device

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
