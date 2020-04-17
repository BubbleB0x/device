/*
 *        Connessione tramite WIFI ad una BUBBLEBOX STATION oppure un WIFI semplice tramite WPS
 *        
 *        ----> La connessione alla BUBBLESTATION può essere di 2 tipi:
 *        1. La bubblestation è un device a se collegato alla rete di casa
 *        2. Lo smartphone può simulare una bubblestation segeundo le indicazioni riportate sull'app device
 *        
 *        
 *        Library github: https://github.com/espressif/arduino-esp32/tree/master/libraries/WiFi/examples/WPS
 *        
 */

#include "WiFi.h"
#include "esp_wps.h"
#include "HTTPClient.h"
#include <base64.h>                                             // Libreria Decode/Encode in Base64
#include <ArduinoJson.h>                                        // Libreria per la gestione dei file JSON

#include "time.h"                                               // Libreria per gestire data e ora attravero un server NTP

//--------------- definizione credenziali di accesso WPS ----------------------------------------------------------------------------------
#define ESP_WPS_MODE      WPS_TYPE_PBC
#define ESP_MANUFACTURER  "BUBBLEBOX"
#define ESP_MODEL_NUMBER  "ESP32"
#define ESP_MODEL_NAME    "BUBBLE IOT"
#define ESP_DEVICE_NAME   "BUBBLEBOX DEVICE"

DynamicJsonDocument doc(1024);                                  // Creazione della variabile per poter leggere i JSON che arrivano dal server durante le chiamate REST

static esp_wps_config_t config;                                 // Configurazione WPS - WIFI

//------------------------- Connessione alla WIFI tramite WPS -----------------------------------------------------------------------------
bool connessoWPS = false;

//------------------------- Credenziali di connessione alla BubbleStation --------------------------------------------------------------
const char* ssid = "Linkem1_05B76F";
const char* password =  "Casa1234";

//--------------- Caratteristiche server BubbleBox ---------------------
const char* serverNamePost = "http://37.77.97.144:9200/devices/blast/";
const char* serverNameGetToken = "http://37.77.97.144:9200/loginDevice/";
const char* pass = "prova2";
HTTPClient http;

//-------------- Caratteristiche server NTP Clock -----------------------
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0;
const int   daylightOffset_sec = 3600;

//------------------------------------------------------------------------------------
void wpsInitConfig(){
  config.crypto_funcs = &g_wifi_default_wps_crypto_funcs;
  config.wps_type = ESP_WPS_MODE;
  strcpy(config.factory_info.manufacturer, ESP_MANUFACTURER);
  strcpy(config.factory_info.model_number, ESP_MODEL_NUMBER);
  strcpy(config.factory_info.model_name, ESP_MODEL_NAME);
  strcpy(config.factory_info.device_name, ESP_DEVICE_NAME);
}

String wpspin2string(uint8_t a[]){
  char wps_pin[9];
  for(int i=0;i<8;i++){
    wps_pin[i] = a[i];
  }
  wps_pin[8] = '\0';
  return (String)wps_pin;
}

void WiFiEvent(WiFiEvent_t event, system_event_info_t info){
  switch(event){
    case SYSTEM_EVENT_STA_START:
      Serial.println("Station Mode Started");
      break;
    case SYSTEM_EVENT_STA_GOT_IP:
      Serial.println("Connected to :" + String(WiFi.SSID()));
      Serial.print("Got IP: ");
      Serial.println(WiFi.localIP());
      connessoWPS = true;
      break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
      Serial.println("Disconnected from station, attempting reconnection");
      WiFi.reconnect();
      break;
    case SYSTEM_EVENT_STA_WPS_ER_SUCCESS:
      Serial.println("WPS Successfull, stopping WPS and connecting to: " + String(WiFi.SSID()));
      esp_wifi_wps_disable();
      delay(10);
      WiFi.begin();
      break;
    case SYSTEM_EVENT_STA_WPS_ER_FAILED:
      Serial.println("WPS Failed, retrying");
      esp_wifi_wps_disable();
      esp_wifi_wps_enable(&config);
      esp_wifi_wps_start(0);
      break;
    case SYSTEM_EVENT_STA_WPS_ER_TIMEOUT:
      Serial.println("WPS Timedout, retrying");
      esp_wifi_wps_disable();
      esp_wifi_wps_enable(&config);
      esp_wifi_wps_start(0);
      break;
    case SYSTEM_EVENT_STA_WPS_ER_PIN:
      Serial.println("WPS_PIN = " + wpspin2string(info.sta_er_pin.pin_code));
      break;
    default:
      break;
  }
}

//-------------------------------------------------------------------------------------------------------

//---------------- Ottenimento Data e Ora da un server NTP ----------------------------------------------
void setLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo))
  {
    Serial.println("Failed to obtain time");
    return;
  }
  char anno[5];
  char mese[3];
  char giorno[3];
  char ora[3];
  char minuti[3];
  char secondi[3];
  strftime(anno, sizeof(anno), "%Y", &timeinfo);
  strftime(mese, sizeof(mese), "%m", &timeinfo);
  strftime(giorno, sizeof(giorno), "%d", &timeinfo);
  strftime(ora, sizeof(ora), "%H", &timeinfo);
  strftime(minuti, sizeof(minuti), "%M", &timeinfo);
  strftime(secondi, sizeof(secondi), "%S", &timeinfo);

  setRTC(atoi(anno), atoi(mese), atoi(giorno), atoi(ora), atoi(minuti), atoi(secondi)); // RTC_CLock.h
}

//---------------- Ottenimento token per effettuare le POST al server -----------------------------------
String getToken()
{
  http.begin(serverNameGetToken);                                                  // Inizializzazione chiamata server per ottenere il token
  String MAC_PASS = MyMAC + ":::" + pass;                                          // Stringa da mandare durante la chiamata POST per ricevere il token
  MAC_PASS = base64::encode(MAC_PASS);                                             // Conversione in base64
  http.addHeader("authorization", "Basic " + MAC_PASS);                            // Autorizzazione per poter effettuare la POST --> MioMAC:::pass in BASE64
  http.addHeader("content-length", "0");                                           // La POST deve essere effetuata selza alcun contenuto
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int httpResponseCode = http.POST("");                                            // Effettuo la POST al server
  Serial.println(httpResponseCode);                                                // Risposta alla POST
  if(httpResponseCode == 200)                                                      // Controllo se il messaggio di risposta è OK
  {
    String payload = http.getString();                                             // Ottengo il messaggio di risposta dal server dopo la POST
    deserializeJson(doc, payload);                                                 // Trasformo la risposat in un JSON --> Al suo interno è presente il TOKEN per effettuare le rihieste di aggiunta contatti
    JsonObject obj = doc.as<JsonObject>();
    String access_token = obj["access_token"];
    Serial.println(access_token);
    http.end();
    return access_token;                                                           // Restituisco l'access token per effettuare le future richieste
  }
  http.end();
  return "";
}

//---------------- Invio dati al server con tutti i contatti ----------------
void sendDataServer()
{
  numeroDisplay = 9;
  accendiDisplay("", "", "", "");                                                 // Visualizzo schermata di INVIO dati al server
  delay(2000);
  String bufferFile;
  String bodyContacts = "";                                                       // Variabile dove andranno a finire tutti i contatti
  File file = SD.open("/contacts_all.txt");                                       // Leggo il file contenente tutti i contatti
  Serial.print("Read from file: ");
  while(file.available())
  {
     bufferFile = file.readStringUntil('\n');                                     // veridfico che il file contine delle righe --> ogni riga equivale ad un contatto
     bufferFile = base64::encode(bufferFile);                                     // Conversione in base64
     if(bodyContacts != "")
     {
        bodyContacts = bodyContacts + "," + "\"" + bufferFile + "\"";             // Construzione contatti in base64 e concatenamemto per l'invio tramite REST
     }
     else
     {
        bodyContacts = "\"" + bufferFile + "\"";
     }
     delay(100);
   }
  file.close();
  Serial.println(bodyContacts);                                                   // Stampa dei contatti convertiti in base64
  
  String Token = getToken();                                                      // Ottenimento Token per effettuare le richieste
  //------------------------- POST -----------------------------------------------------------------------------------------
  http.begin(serverNamePost);                                                     // Inizializzazione chiamata server
  http.addHeader("authorization", "Bearer " + Token);                             // Header per effettuare la POST al server --> Autorizzazione | ContentType 
  http.addHeader("Content-Type", "application/json");                             // Invio del JSON tramite POST 
  int httpResponseCode = http.POST("{\"blast\":[" + bodyContacts + "]}");         // Body JSON POST HTTP Client
  Serial.println(httpResponseCode);                                               // Codice di risposta della richiesta POST 
  http.end();                                                                     // Chiusura HTTP REST
  if(httpResponseCode == 200)
  {
    // -----------> Inserimento avvenuto con successo all'interno del server!
    //------------> Bisogna cancellare il file contacts_all.txt con tutti i contatti al suo interno [DA FARE IL PRIMA POSSIBILE!]
  }
  //--------------------------------------------------------------------------------------------------------------------------
  delay(2000);

  //----------------------------- Inizializzazione richiesta Server NTP per aggiornare Data e Ora RTC ----------------------------
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  setLocalTime();
}

//------------------------------ Funzione di connessione alla rete WIFI tramite WPS -------------------------------------------
void connectWPS()
{
  if(WiFi.status() != WL_CONNECTED)                                 // Verifico, per prima cosa, se il device è già connesso ad una rete locale in zona
  {
    Serial.println("Connessione WPS in corso...");                  // Se non è connesso ad alcuna rete allora abilito la connessione WPS

    WiFi.onEvent(WiFiEvent);
    WiFi.mode(WIFI_MODE_STA);
  
    Serial.println("Starting WPS");
  
    wpsInitConfig();                                                // inizializzazione WPS
    esp_wifi_wps_enable(&config);
    esp_wifi_wps_start(0);
  
    int contWPS = 0;                                                // Variabile contatore per verificare il tempo di non avvenuta connessione al WIFI
  
    while(!connessoWPS && contWPS < 150)                            // Verifico se la connessione è avvenuta o sono scaduti i 20/30 secondi per far avvenire la connessione
    {
      Serial.println("CONNESSIONE...");
      delay(200);
      ++contWPS;                                                    // Incremento tempo per la non avvenuta connessione
      Serial.println(contWPS);
    }
    if(connessoWPS)                                                 // Connessione avvenuta
    {
      Serial.println("Connesso al WIFI!");
      sendDataServer();                                             // Invio dati al server tramite POST
      ControlTimeWake = 14;                                         // |
      numeroDisplay = 1;                                            // |   Resetto i parametri
      statoBLT = false;                                             // |   del device
      smartphoneConnect = false;                                    // |
    }
    if(contWPS > 149)                                               // Tempo di non avvenuta connessione superata
    {
      esp_wifi_wps_disable();                                       // Disabilito la funzione WPS
      Serial.println("Chiusura connessione WPS...");
      Serial.println("Troppo tempo per la risposta!");
      delay(200);
      ControlTimeWake = 14;                                         // |
      numeroDisplay = 1;                                            // |  Resetto i parametri
      statoBLT = false;                                             // |  del device
      smartphoneConnect = false;                                    // |
    }
  }
  else
  {
    Serial.println("CONNESSIONE GIA' EFFETTUATA!");
    sendDataServer();                                             // Invio dati al server tramite POST
  }
}

//------------------- Connessione ad una Bubblestation o allo smartphone tramite hotspot --------------------------------
void connectStation()
{
  int countStation = 0;
  if(WiFi.status() != WL_CONNECTED)                               // Verifico se il device è già connesso ad una rete
  {
    WiFi.begin(ssid, password);                                   // Effettuo la connessione con credenziali alla bubblestation qualora non fosse già connesso
    while (WiFi.status() != WL_CONNECTED && countStation < 30)    // prova la connessione per 15 secondi
    {
      delay(500);
      Serial.println("Connecting to WiFi..");
      ++countStation;
    }
  }

  if(WiFi.status() == WL_CONNECTED)                             // Si è connessi ad una rete WIFI
  {
    sendDataServer();                                           // Invio dati al server tramite POST
  }
  
  ControlTimeWake = 14;                                         // |
  numeroDisplay = 1;                                            // |   Resetto i parametri
  statoBLT = false;                                             // |   del device
  smartphoneConnect = false;                                    // |
  bubbleStation = false;                                        // |
}
