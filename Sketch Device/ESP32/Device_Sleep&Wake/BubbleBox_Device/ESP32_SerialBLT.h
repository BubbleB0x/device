/*
 * ------------ Serial Bluetooth Comunication ----------------
 * 
 * Library ESP32: https://github.com/espressif/arduino-esp32/tree/master/libraries/BluetoothSerial
 * 
 */

#include "BluetoothSerial.h"

String MACDevice;

BluetoothSerial ESP_BT; 

//----------- Quando viene premenuto il Bottone 2 e ci si trova nella schermata della connessione BLT (Smartphone o station) 
//----------- vine visualizzata la schermata opportuna
void IRAM_ATTR connessioneBLT()
{
  if(numeroDisplay == 3)
  {
    Serial.println();
    Serial.println("--------- INVIO DATI STATION --->");
    ControlTimeWake = 0;
    numeroDisplay = 5;
    statoBLT = true;
  }
  else
  {
    if(numeroDisplay == 4)
    {
      Serial.println();
      Serial.println("--------- INVIO DATI SMARTPHONE --->");
      ControlTimeWake = 0;
      numeroDisplay = 6;
      statoBLT = true;
      smartphoneConnect = true;
    } 
  }
}

/*   -----> Da Abilitare, qualora ne avessimo bisogno!
void closeBLTConnection()
{
  ESP_BT.end();
  statoBLT = false;
  bubbleStation = false;
  smartphoneConnect = false;
}
*/

//----------- Chiusura connessione serial bluetooth -----------
void closeConnectionSerial()
{
  ESP_BT.end();               // Chiusura connessione serial bluetooth
  ControlTimeWake = 14;       // Tempo del device viene portato a termine per farlo ripartire e ricaricare tutto
  numeroDisplay = 1;          // Schermata display viene portata alla principale --> Orologio
  statoBLT = false;           // Stato attivo/disattivo BLT portato a disattivato
  bubbleStation = false;      // Ritrovamento bubblestation portato a disattivato
  smartphoneConnect = false;  // Ritrovamento smartphone portato a disattivato

  //--------------------------> Il device riprende il suo normale funzionamento
}

//---------------------------- Connessione serial bluetooth smartphone -------------
void sendDataSmartphone()
{
  Serial.println("Connessione smartphone in corso...");
  ESP_BT.begin("BubbleBox_Device");                                     // Abilito la connessione serial bluetooth
  while(smartphoneConnect)                                              // Fino a che la schermata è aperto controllo se arrivano dati dagli smartphone collegati
  {
    Serial.println("Bluetooth Device is Ready Smartphone...");          // Il device bubblebox è pronto per ricevere e inviare dati
    if (ESP_BT.available())                                             // Quando viene mandato un dato (qualsiasi dato) dallo smartphone il device comincia ad inviare
    {
      int incoming = ESP_BT.read(); //Read what we recevive             // Dato ricevuto dallo smartphone
      Serial.print("Received:"); 
      Serial.println(incoming);
      ESP_BT.println("00:11:22:33:44|11:22:33:11:22|2020-12-12|12:20"); // Dato da mandare allo smartphone
      if(incoming == 50)
      {
        ESP_BT.println("Chiusura...");
        smartphoneConnect = false;                                      // Connessione smartphone disabilitata
      }
    }
    delay(500);
  }
  closeConnectionSerial();                                              // Chiusura connessione serial bluetooth
}

//----------------------------------------- Invio dati e connessione bubbleStation --------------------------------
void sendDataBLT()
{
  MACDevice = "";
  MACDevice = scanArea();                                               // Scansione area e viene restituito un MAC address qualora questo appartenga ad un BubbleStation
  Serial.println(MACDevice);
  if(MACDevice != "")                                                   // Solamente un BubbleStation restituirà un MAC address
  {
    Serial.println("DEVICE TROVATO -->");
    bubbleStation = true;                                               // Attivato BubbleStation
    Serial.println("STATION TROVATA----INIZIO COMUNICAZIONE!");
    Serial.println(MACDevice);
    ESP_BT.begin("BubbleBox_Device");                                   // Inizializzazione Serial Bluetooth per comunicazione seriale con BubbleStation
    accendiDisplay("", "");                                             // Accensione display con schermata di invio dati
    while(bubbleStation)                                                // Fino a che la bubblestation sarà attivo, la comunicazione avviene
    {
      Serial.println("Bluetooth Device is Ready to Pair");
      if (ESP_BT.available()) 
      {
        int incoming = ESP_BT.read(); //Read what we recevive
        Serial.print("Received:"); 
        Serial.println(incoming);
        ESP_BT.println("Digita 2 per chiudere tutto!");
        if(incoming == 50)                                              // Se la bubble station invia un segnale (un 2) la comunicazione si chiude
        {
          ESP_BT.println("Chiusura...");
          bubbleStation = false;
        }
      }
      delay(500);
    }
    closeConnectionSerial();                                            // Il device continua il suo normale funzionamento
  }
}
