/*
 * ------------ Serial Bluetooth Comunication ----------------
 * 
 * Library ESP32: https://github.com/espressif/arduino-esp32/tree/master/libraries/BluetoothSerial
 * 
 */

#include "BluetoothSerial.h"
#include "FS.h"
#include "SD.h"
#include "SPI.h"

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

      //---------------- INVIO CONTATTI ALLO SMARTPHONE -------------------------------------------------------
      if(incoming == 49)                                                  // Invio dati allo smartphone quando ricevo il carattere "1" --> TEST DI VERIFICA
      {
        String bufferFile;
        File file = SD.open("/contacts_all.txt");                         // Leggo il file contenente tutti i contatti
        Serial.print("Read from file: ");
        while(file.available())
        {
            bufferFile = file.readStringUntil('\n');                      // veridfico che il file contine delle righe --> ogni riga equivale ad un contatto
            ESP_BT.println(bufferFile);                                   // Invio ogni singolo contatto allo smartphone
        }
        file.close();                                                     // Chiudo il file una volta terminata la lettura dell'intero file 
      }
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

//----------------------------------------- Invio dati e connessione bubbleStation --------------------------------
/*
 * -------------------- DA COMPLETARE --> IMPLEMETARE ANCHE LA BUBBLE STATION [APPENA POSSIBILE] -----------------
 * 
 */
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
    accendiDisplay("", "", "", "");                                         // Accensione display con schermata di invio dati
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
