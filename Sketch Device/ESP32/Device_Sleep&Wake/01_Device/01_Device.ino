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
// Librerie ESP32 BLE 
#include <BLEAdvertisedDevice.h>
#include <BLEDevice.h>
#include <BLEScan.h>
#include <BLEUtils.h>
#include <Arduino.h>
#include <BLESecurity.h>

// Librerie Displey OLED
#include <Wire.h>               // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h"        // legacy: #include "SSD1306.h"
SSD1306Wire display(0x3c, 21, 22);

// Librerie Modulo scheda SD per memorizzare i dati
#include "FS.h"
#include "SD.h"
#include "SPI.h"

// Nome, MAC Address, Rssi e Manifattura del dispositivo trovato
String MAC;
String Name;
String Manufacturer;
int Rssi_device = 0;

// MAC Address BLE di questo dispositivo
String MyMAC;

bool BubbleBoxTrovato = false;

// Caratteristiche rete BLE del decvice
#define SERVICE_UUID "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_RX "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
BLECharacteristic *pCharacteristic;
BLEServer *pServer;
BLEService *pService;

// Caratteristiche e specifiche dello Sleep&Wake
#define Threshold 40 // Sensibilità all'attivazione ---> NON UTILIZZATO (PER IL MOMENTO)
touch_pad_t touchPin; // -------> NON UTILIZZATO
#define BUTTON_PIN_BITMASK 0x8004 // GPIOs 2 and 15
RTC_DATA_ATTR int bootCount = 0; // Conteggio boot device
int ControlTimeWake = 0; // Controllo tempo di Wake
const int RF_Nano = 25;

// Bottone che cambia la schermata del sisplay OLED
const int statoDisplay = 14;
const int clockChange = 12;
int numeroDisplay = 0;

void setup() 
{
  setSDCard(); // Setto l'SD card
  
  BubbleBoxTrovato = false; // setto la variabile del ritrovamento dispositivo a false
  
  numeroDisplay = 0;
  pinMode(statoDisplay, INPUT);
  pinMode(clockChange, INPUT);
  display.init();
  display.flipScreenVertically();
  
  pinMode(RF_Nano, INPUT);
  ControlTimeWake = 0;
  Serial.begin(115200);
  enableBLE(); //Abilitare il Bluetooth Low Energy
}

void loop() 
{
  ++ControlTimeWake;
  
  controlloCambioDisplay(); // Controllo per vedere se è stato premuto il tasto di cambio schermo
  accendiDisplay(); // ACCENDO IL DISPLAY
  
  // Scansione area per trovare i device con BLE nelle vicinanze
  scanArea(); 
  // Disconnesione dei dispositivi che si connettono
  disconnectedDeviceBLE();

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

//--------------------------BLE ESP32--------------------------------------------
//--------------------------------------------------------------------------------
/*
 * Scansione area per il ritrovamento di altri device BLE
 */
void scanArea()
{
  Serial.print("\n#############################################################");
  Serial.print("\n###### My Address MAC BLE: ");
  Serial.print(MyMAC);
  Serial.print("\n#############################################################");
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setActiveScan(true);  
  // Attivo la scansione BLE ogni secondo
  BLEScanResults results = pBLEScan->start(1); 
  Serial.println("\n-----------------------------DISPOSITIVI--------------------------------");
  Serial.print("\nDispositivi trovati:");
  Serial.print(results.getCount());
  
  // Controllo singolarmente ogni dispositivo trovato attraverso lo scanning BLE
  for (int i = 0; i < results.getCount(); i++) 
  {
    Serial.println("\n_______________________________________________________________________");
    BLEAdvertisedDevice device = results.getDevice(i);
    Serial.print("\nDevice Address ");
    Serial.print(i + 1);
    Serial.print(": ");
    MAC = device.getAddress().toString().c_str();
    Serial.print(MAC);
    Serial.print(" | Rssi: ");
    Rssi_device = device.getRSSI();
    Serial.print(Rssi_device);
    Name = device.getName().c_str();
    Serial.print(" | Nome Device: ");
    Serial.print(Name);
    Manufacturer = device.getManufacturerData().c_str();
    Serial.print(" | Manufacturer Device: ");
    Serial.print(Manufacturer);

    // Controllo del probabile contatto con un'altra persona che possiede un altro BubbleBox
    if(Name == "BubbleBox" && Rssi_device > -75)
    {
      BubbleBoxTrovato = true;
      scriviContatto(MAC);
      Serial.println("\n\n------------------------CONSIDERA IL SEGNALE----------------------------");
      Serial.println("\n----------------------------CONTATTO AVVENUTO!----------------------------");
    }
  }
  Serial.print("\n__________________________________________________________________________");
}
/*
 * Attivazione del BLE e di tutti i servizi annessi
 */
void enableBLE()
{
  BLEAdvertisementData pAdvertisementData;
  BLEDevice::init("BubbleBox"); // Init del BLE e nome device --> BubbleBox
  pServer = BLEDevice::createServer();
  pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID_TX,
                      BLECharacteristic::PROPERTY_NOTIFY
                      );
  pAdvertisementData.setName("BubbleBox");
  pAdvertisementData.setManufacturerData("BubbleBox_Device");
  pService->start();
  pServer->getAdvertising()->start();
  BLEAddress  ADD = BLEDevice::getAddress();
  MyMAC = ADD.toString().c_str();
  Serial.print("\n\nMy Address:");
  Serial.print(MyMAC);
  Serial.print("\n------------------------------------------------");
}

// Disconnette i dispositivi bluetooth che si connettono all'ESP-32
void disconnectedDeviceBLE()
{
  pServer->disconnect(pServer->getConnId());
}

//----------------------SLEEP & WAKE ESP32----------------------------------------
//--------------------------------------------------------------------------------
// Settagio dello sleep&wake del device e partenza device
void setSleepWake()
{
  // Incremento il conteggio del boot del device
  ++bootCount;
  Serial.println("Boot number: " + String(bootCount));

  //Stampa del motivo del risveglio del dispositivo
  print_wakeup_reason();
  print_GPIO_wake_up();

  // Setta l'interrupt del device con il TouchPad 3 (GPIO15)
  esp_sleep_enable_ext1_wakeup(BUTTON_PIN_BITMASK,ESP_EXT1_WAKEUP_ANY_HIGH);

  Serial.println("Spegnimento...");
  esp_deep_sleep_start();
}

void callback(){
  //placeholder callback function
}

// Metodo per capire in che modo è avvenuto lo Sleep&Wake del device
void print_wakeup_reason()
{
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch(wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP : Serial.println("Wakeup caused by ULP program"); break;
    default : Serial.printf("Wakeup was not caused by deep sleep: %d\n",wakeup_reason); break;
  }
}

// Metodo per capire con quale touchpad è avvenuto il risveglio del device
void print_GPIO_wake_up(){
  int GPIO_reason = esp_sleep_get_ext1_wakeup_status();
  Serial.print("GPIO that triggered the wake up: GPIO ");
  Serial.println((log(GPIO_reason))/log(2), 0);
}

//--------------------------DISPLAY SETUP ON/OFF--------------------------------------------------
//--------------------------------------------------------------------------------
// Accensione del display e visualizzazione della data e ora
void accendiDisplay()
{
  switch (numeroDisplay)
  {
    case 0:
    display.displayOn();
    display.clear();
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

    case 1:
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

    case 2:
    display.displayOn();
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.setFont(ArialMT_Plain_16);
    display.drawCircle(64, 20, 30);
    display.drawString(64, 20, "CLOCK");
    display.setFont(ArialMT_Plain_10);
    display.drawString(64, 50, "SETTAGGIO OROLOGIO");
    display.display();
    break; 

    case 3:
    display.displayOn();
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.setFont(ArialMT_Plain_16);
    display.drawCircle(64, 20, 30);
    display.drawString(64, 20, "DATA");
    display.setFont(ArialMT_Plain_10);
    display.drawString(64, 50, "SETTAGGIO DATA");
    display.display();
    break; 
    break;

    case 5:
    display.displayOn();
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.setFont(ArialMT_Plain_24);
    display.drawRect(25, 15, 80, 35);
    display.drawString(64, 20, "00:00");
    display.setFont(ArialMT_Plain_10);
    display.drawString(64, 50, "SETTAGGIO ORA");
    display.display();
    break; 

    case 6:
    display.displayOn();
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.setFont(ArialMT_Plain_24);
    display.drawRect(25, 15, 120, 35);
    display.drawString(64, 20, "01-01-2000");
    display.setFont(ArialMT_Plain_10);
    display.drawString(64, 50, "SETTAGGIO DATA");
    display.display();
    break; 
  }
}

// Spegnimento diplay
void spegniDisplay()
{
  display.displayOff();
}

void controlloCambioDisplay()
{
  if(digitalRead(clockChange) == HIGH && numeroDisplay == 2)
  {
    ControlTimeWake = 0;
    numeroDisplay = 5;
  }
  if(digitalRead(clockChange) == HIGH && numeroDisplay == 3)
  {
    ControlTimeWake = 0;
    numeroDisplay = 6;
  }
  if(digitalRead(statoDisplay) == HIGH)
  {
    ControlTimeWake = 0;
    ++numeroDisplay;  
    if(numeroDisplay > 3)
    {
      numeroDisplay = 0;  
    }
  }
}

void displayBubbleBoxContatto()
{
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_10);
  display.drawString(64, 0, "BUBBLEBOX TROVATO!");
  display.display();
}

//-----------------------SD CARD DATA STORAGE & SETUP------------------------------------
//--------------------------------------------------------------------------------
void setSDCard()
{
  if(!SD.begin()){
        Serial.println("Card Mount Failed");
        return;
    }
    uint8_t cardType = SD.cardType();

    if(cardType == CARD_NONE){
        Serial.println("No SD card attached");
        return;
    }
    uint64_t cardSize = SD.cardSize() / (1024 * 1024);
    Serial.printf("SD Card Size: %lluMB\n", cardSize);

    writeFile(SD, "/contacts2020_04_04.txt", "");
}

void scriviContatto(String contact)
{
  String inserimento = "Mio MAC: " + MyMAC + " | CONTACT: " + contact + " | DATA: " + "Data \n"; 
  appendFile(SD, "/contacts2020_04_04.txt", inserimento.c_str());
}

void writeFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Writing file: %s\n", path);

    File file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("Failed to open file for writing");
        return;
    }
    if(file.print(message)){
        Serial.println("File written");
    } else {
        Serial.println("Write failed");
    }
    file.close();
}

void appendFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Appending to file: %s\n", path);

    File file = fs.open(path, FILE_APPEND);
    if(!file){
        Serial.println("Failed to open file for appending");
        return;
    }
    if(file.print(message)){
        Serial.println("Message appended");
    } else {
        Serial.println("Append failed");
    }
    file.close();
}