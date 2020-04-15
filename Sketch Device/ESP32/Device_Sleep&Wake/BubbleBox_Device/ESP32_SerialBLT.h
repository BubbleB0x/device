/*
 * ------------ Serial Bluetooth Comunication ----------------
 * 
 * Library ESP32: https://github.com/espressif/arduino-esp32/tree/master/libraries/BluetoothSerial
 * 
 */

#include "BluetoothSerial.h"        // LIBRERIA CONNESSIONE BLUETOOTH SERIALE

#include "FS.h"                     //
#include "SD.h"                     //  LIBRERIE PER LA CONNESSIONE ALLA SD CARD E LA LETTURA DEI FILE CONTENENTE I CONTATTI --> VIENE UTILIZZATO SOLO PER L'INVIO DATI ALLO SMARTPHONE DOPO LA CONNESSIONE!
#include "SPI.h"                    //

String MACDevice;

BluetoothSerial ESP_BT;             // BLUETOOTH SERIALE

//----------- Quando viene premenuto il Bottone 2 e ci si trova nella schermata della connessione BLT (Smartphone o station) 
//----------- vine visualizzata la schermata opportuna
void IRAM_ATTR connessioneBLT()
{
  if(numeroDisplay == 4)
    {
      Serial.println();
      Serial.println("--------- INVIO DATI SMARTPHONE --->");
      ControlTimeWake = 0;
      numeroDisplay = 7;
      statoBLT = true;
      smartphoneConnect = true;
    } 
   if(numeroDisplay == 3)
   {
    Serial.println();
    Serial.println("--------- CONNESSIONE WPS ----->");
    ControlTimeWake = 0;
    numeroDisplay = 6;
    statoBLT = true;
    smartphoneConnect = false;
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

//------------------ Funzione che viene richiamata solamente quando un client si connette -------------------------
void callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param){
  if(event == ESP_SPP_SRV_OPEN_EVT){
    Serial.println("Client Connected");                                 // Client connesso!
    String bufferFile;
    File file = SD.open("/contacts_all.txt");                           // Leggo il file contenente tutti i contatti
    Serial.print("Read from file: ");
    while(file.available())
    {
       bufferFile = file.readStringUntil('\n');                         // veridfico che il file contine delle righe --> ogni riga equivale ad un contatto
       ESP_BT.println(bufferFile);                                      // Invio ogni singolo contatto allo smartphone
       delay(100);
    }
    file.close();                                                       // Chiudo il file una volta terminata la lettura dell'intero file 
    //-----> ELIMINAZIONE DEL FILE UNA VOLTA TERMINATO L'INVIO DI TUTTI I CONTATTI ---------
  }
}

//----------- Chiusura connessione serial bluetooth -----------
void closeConnectionSerial()
{
  ESP_BT.end();               // Chiusura connessione serial bluetooth
  ControlTimeWake = 14;       // Tempo del device viene portato a termine per farlo ripartire e ricaricare tutto
  numeroDisplay = 1;          // Schermata display viene portata alla principale --> Orologio
  statoBLT = false;           // Stato attivo/disattivo BLT portato a disattivato
  // bubbleStation = false;      // Ritrovamento bubblestation portato a disattivato [DEPRECATO]
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
    ESP_BT.register_callback(callback);                                 // Richiamo funzione callback --> Invio dei dati contenente i contatti allo smartphone
    if (ESP_BT.available())                                             // Quando viene mandato un dato (qualsiasi dato) dallo smartphone il device comincia ad inviare
    {
      int incoming = ESP_BT.read(); //Read what we recevive             // Dato ricevuto dallo smartphone
      Serial.print("Received:"); 
      Serial.println(incoming);
      //---------------------------------------------------------------------------------------------------------
      if(incoming == 50)                                                // Quando arriva il numero 2 dallo smartphone chiudo la comunicazione
      {
        ESP_BT.println("Chiusura...");
        smartphoneConnect = false;                                      // Connessione smartphone disabilitata
      }
    }
    delay(500);
  }
  closeConnectionSerial();                                              // Chiusura connessione serial bluetooth
}


//---------------------------------- << DEPRECATO >> -----------------------------------------------------------------------
//----------------------------------------- Invio dati e connessione bubbleStation --------------------------------
/*
 * -------------------- DA COMPLETARE --> IMPLEMETARE ANCHE LA BUBBLE STATION [APPENA POSSIBILE] -----------------
 * 
void sendDataBLT()
{
  MACDevice = "";
  MACDevice = scanArea("", "");                                         // Scansione area e viene restituito un MAC address qualora questo appartenga ad un BubbleStation
  Serial.println(MACDevice);
  if(MACDevice != "")                                                   // Solamente un BubbleStation restituirà un MAC address
  {
    Serial.println("DEVICE TROVATO -->");
    bubbleStation = true;                                               // Attivato BubbleStation
    Serial.println("STATION TROVATA----INIZIO COMUNICAZIONE!");
    Serial.println(MACDevice);
    ESP_BT.begin("BubbleBox_Device");                                   // Inizializzazione Serial Bluetooth per comunicazione seriale con BubbleStation
    accendiDisplay("", "", "", "");                                     // Accensione display con schermata di invio dati
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
*/
