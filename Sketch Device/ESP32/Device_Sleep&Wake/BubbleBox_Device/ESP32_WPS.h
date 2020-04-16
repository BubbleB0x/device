/*
 *        Library github: https://github.com/espressif/arduino-esp32/tree/master/libraries/WiFi/examples/WPS
 *        
 */

#include "WiFi.h"
#include "esp_wps.h"
#include "HTTPClient.h"
/*
Change the definition of the WPS mode
from WPS_TYPE_PBC to WPS_TYPE_PIN in
the case that you are using pin type
WPS
*/
#define ESP_WPS_MODE      WPS_TYPE_PBC
#define ESP_MANUFACTURER  "BUBBLEBOX"
#define ESP_MODEL_NUMBER  "ESP32"
#define ESP_MODEL_NAME    "BUBBLE IOT"
#define ESP_DEVICE_NAME   "BUBBLEBOX DEVICE"

#include <base64.h>

//----------- JSON -----------------------------------------------------------------------------------------------------------------------
#include <ArduinoJson.h>
DynamicJsonDocument doc(1024);

static esp_wps_config_t config;

//------------------------- Connessione alla WIFI tramite WPS -----------------------------------------------------------------------------
bool connessoWPS = false;
const char* ssid = "Linkem1_05B76F";
const char* password =  "Casa1234";

//--------------- Caratteristiche server ---------------------
const char* serverNamePost = "http://37.77.97.144:9200/devices/blast/";
const char* serverNameGetToken = "http://37.77.97.144:9200/loginDevice/";
HTTPClient http;
const char* pass = "prova2";

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

//---------------- Ottenimento token per effettuare le POST al server --------------------------
String getToken()
{
  http.begin(serverNameGetToken);                                                  // Inizializzazione chiamata server per ottenere il token
  String MAC_PASS = MyMAC + ":::" + pass;                                          // Stringa da mandare durante la chiamata POST per ricevere il token
  MAC_PASS = base64::encode(MAC_PASS);                                             // Conversione in base64
  http.addHeader("authorization", "Basic " + MAC_PASS);
  http.addHeader("content-length", "0");
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int httpResponseCode = http.POST("");
  Serial.println(httpResponseCode);
  if(httpResponseCode == 200)
  {
    String payload = http.getString();
    deserializeJson(doc, payload);
    JsonObject obj = doc.as<JsonObject>();
    String access_token = obj["access_token"];
    Serial.println(access_token);
    http.end();
    return access_token;
  }
  http.end();
  return "";
}

//---------------- Invio dati al server con tutti i contatti ----------------
void sendDataServer()
{
  // Da implementare
  numeroDisplay = 8;
  accendiDisplay("", "", "", "");
  delay(2000);
  String bufferFile;
  String bodyContacts = "";
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
  
  String Token = getToken();
  //------------------------- POST -----------------------------------------------------------------------------------------
  http.begin(serverNamePost);                                                     // Inizializzazione chiamata server
  http.addHeader("authorization", "Bearer " + Token);                             // Header per effettuare la POST al server --> Autorizzazione | ContentType 
  http.addHeader("Content-Type", "application/json");                             // Invio del JSON tramite POST 
  int httpResponseCode = http.POST("{\"blast\":[" + bodyContacts + "]}");         // Body JSON POST HTTP Client
  //int httpResponseCode = http.GET();
  Serial.println(httpResponseCode);                                               // Codice di risposta della richiesta POST 
  http.end();                                                                     // Chiusura HTTP REST
  if(httpResponseCode == 200)
  {
    // -----------> Inserimento avvenuto con successo all'interno del server!
    //------------> Bisogna cancellare il file contacts_all.txt con tutti i contatti al suo interno [DA FARE IL PRIMA POSSIBILE!]
  }
  //--------------------------------------------------------------------------------------------------------------------------
  delay(5000);
}

//------------------------------ Funzione di connessione alla rete WIFI tramite WPS -------------------------------------------
void connectWPS(){
/*
  Serial.println("Connessione WPS in corso...");

  WiFi.onEvent(WiFiEvent);
  WiFi.mode(WIFI_MODE_STA);

  Serial.println("Starting WPS");

  wpsInitConfig();
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
  */
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  sendDataServer();                                             // Invio dati al server tramite POST
  ControlTimeWake = 14;                                         // |
  numeroDisplay = 1;                                            // |   Resetto i parametri
  statoBLT = false;                                             // |   del device
  smartphoneConnect = false;
}
