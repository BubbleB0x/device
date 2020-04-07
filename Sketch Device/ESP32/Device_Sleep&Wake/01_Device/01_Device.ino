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
int numeroDisplay = 0;
//--------------------------DISPLAY SETUP ON/OFF--------------------------------------------------
//--------------------------------------------------------------------------------

//------------------------LOGHI DISPLAY----------------------------------------------------------
const uint8_t Logo_Init_bits[] PROGMEM = {
  0x00, 0x00, 0x80, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xFF, 0x01, 
  0x00, 0x00, 0x00, 0x00, 0xF8, 0xFB, 0x07, 0x00, 0x00, 0x00, 0x00, 0x3C, 
  0xE0, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x1E, 0xFE, 0x1F, 0x00, 0x00, 0x00, 
  0x00, 0x8F, 0xFF, 0x1F, 0x00, 0x00, 0x00, 0x00, 0xC7, 0xFF, 0x3F, 0x00, 
  0x00, 0x00, 0x80, 0xE3, 0xFF, 0x7F, 0x00, 0x00, 0x00, 0x80, 0xF3, 0xFF, 
  0x7F, 0x00, 0x00, 0x00, 0x80, 0xF9, 0xFF, 0x7F, 0x00, 0x00, 0x00, 0xC0, 
  0xFD, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xC0, 0xFD, 0xFF, 0xFF, 0x00, 0x00, 
  0x00, 0xC0, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x80, 0xDF, 0xFF, 0xFF, 0xFF, 
  0x00, 0x00, 0xC0, 0xDF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xE0, 0xDC, 0xFF, 
  0xFF, 0xFF, 0x00, 0x00, 0x70, 0x9E, 0xFF, 0xFF, 0x7F, 0x00, 0x00, 0x38, 
  0xBF, 0xFF, 0xFF, 0x7F, 0x00, 0x00, 0x98, 0xBF, 0xFF, 0xFF, 0x7F, 0x00, 
  0x00, 0x98, 0x7F, 0xFF, 0xFF, 0xBF, 0x01, 0x00, 0xD8, 0x7F, 0xFE, 0xFF, 
  0x9F, 0x07, 0x00, 0xD8, 0xFF, 0xFE, 0xFF, 0xDF, 0x1F, 0x00, 0xF8, 0xFF, 
  0xFD, 0xFF, 0xE7, 0x19, 0x00, 0xF8, 0xFF, 0xF3, 0xFF, 0xF3, 0x3B, 0x00, 
  0xF0, 0xFF, 0xE1, 0xFF, 0xF8, 0x77, 0x00, 0xF0, 0xFF, 0x01, 0x1E, 0xFE, 
  0x67, 0x00, 0xE0, 0xFF, 0x00, 0xC0, 0xFF, 0x67, 0x00, 0x80, 0x3F, 0x00, 
  0xC0, 0xFF, 0x6F, 0x00, 0x00, 0x0A, 0x00, 0xC0, 0xFF, 0x6F, 0x00, 0x00, 
  0x00, 0x00, 0xC0, 0xFF, 0x67, 0x00, 0x00, 0x00, 0x00, 0x80, 0xFF, 0x77, 
  0x00, 0x00, 0xC0, 0x0F, 0x80, 0xFF, 0x3F, 0x00, 0x00, 0xF0, 0x1F, 0x00, 
  0xFF, 0x3F, 0x00, 0x00, 0x38, 0x3E, 0x00, 0xFF, 0x1F, 0x00, 0x00, 0x98, 
  0x7F, 0x00, 0xFE, 0x0F, 0x00, 0x00, 0xCC, 0x7F, 0x00, 0xF8, 0x03, 0x00, 
  0x00, 0xEC, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0xEC, 0xFF, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0xFC, 0xFF, 0xC0, 0x01, 0x00, 0x00, 0x00, 0xFC, 0xFF, 
  0xF0, 0x07, 0x00, 0x00, 0x00, 0xFC, 0x7F, 0x38, 0x0F, 0x00, 0x00, 0x00, 
  0xF8, 0x7F, 0xDC, 0x1F, 0x00, 0x00, 0x00, 0xF0, 0x3F, 0xEC, 0x1F, 0x00, 
  0x00, 0x00, 0xE0, 0x1F, 0xEC, 0x3F, 0x00, 0x00, 0x00, 0xC0, 0x07, 0xFC, 
  0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x1F, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0xFC, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x0F, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0xF0, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x03, 
  0x00, 0x00,
};
const uint8_t Bubble_Trovato_bits[] PROGMEM = {
  0x40, 0x00, 0xF8, 0x03, 0x1C, 0x07, 0x06, 0x0C, 0x16, 0x0C, 0x0A, 0x08, 
  0x02, 0x0C, 0x06, 0x0C, 0x0C, 0x06, 0x1C, 0x07, 0xF0, 0x01, 0x00, 0x00,
};
const uint8_t Bluetooth_Logo_bits[] PROGMEM = {
  0x00, 0x00, 0x40, 0x00, 0xC0, 0x00, 0x40, 0x01, 0x50, 0x01, 0xE0, 0x00, 
  0x40, 0x00, 0xE0, 0x00, 0x50, 0x01, 0x40, 0x01, 0xC0, 0x00, 0x40, 0x00, 
  0x00, 0x00,
};
const uint8_t Wave_Logo_bits[] PROGMEM = {
  0xC0, 0x0F, 0x00, 0x70, 0x38, 0x00, 0x18, 0xE1, 0x00, 0xC4, 0x9F, 0x01, 
  0x30, 0x30, 0x00, 0x98, 0x47, 0x00, 0xC0, 0x1C, 0x00, 0x20, 0x12, 0x00, 
  0x80, 0x07, 0x00, 0x80, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x07, 0x00, 
  0x00, 0x02, 0x00,
};
const uint8_t WiFi_Logo_bits[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xFF, 0x07, 0x00, 0x00, 0x00,
  0x00, 0x00, 0xE0, 0xFF, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0xFF,
  0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0xFF, 0xFF, 0x00, 0x00, 0x00,
  0x00, 0x00, 0xFE, 0xFF, 0xFF, 0x01, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF,
  0xFF, 0x03, 0x00, 0x00, 0x00, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00,
  0x00, 0xFF, 0xFF, 0xFF, 0x07, 0xC0, 0x83, 0x01, 0x80, 0xFF, 0xFF, 0xFF,
  0x01, 0x00, 0x07, 0x00, 0xC0, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x0C, 0x00,
  0xC0, 0xFF, 0xFF, 0x7C, 0x00, 0x60, 0x0C, 0x00, 0xC0, 0x31, 0x46, 0x7C,
  0xFC, 0x77, 0x08, 0x00, 0xE0, 0x23, 0xC6, 0x3C, 0xFC, 0x67, 0x18, 0x00,
  0xE0, 0x23, 0xE4, 0x3F, 0x1C, 0x00, 0x18, 0x00, 0xE0, 0x23, 0x60, 0x3C,
  0x1C, 0x70, 0x18, 0x00, 0xE0, 0x03, 0x60, 0x3C, 0x1C, 0x70, 0x18, 0x00,
  0xE0, 0x07, 0x60, 0x3C, 0xFC, 0x73, 0x18, 0x00, 0xE0, 0x87, 0x70, 0x3C,
  0xFC, 0x73, 0x18, 0x00, 0xE0, 0x87, 0x70, 0x3C, 0x1C, 0x70, 0x18, 0x00,
  0xE0, 0x87, 0x70, 0x3C, 0x1C, 0x70, 0x18, 0x00, 0xE0, 0x8F, 0x71, 0x3C,
  0x1C, 0x70, 0x18, 0x00, 0xC0, 0xFF, 0xFF, 0x3F, 0x00, 0x00, 0x08, 0x00,
  0xC0, 0xFF, 0xFF, 0x1F, 0x00, 0x00, 0x0C, 0x00, 0x80, 0xFF, 0xFF, 0x1F,
  0x00, 0x00, 0x06, 0x00, 0x80, 0xFF, 0xFF, 0x0F, 0x00, 0x00, 0x07, 0x00,
  0x00, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x00, 0xF8, 0xFF, 0xFF,
  0xFF, 0x7F, 0x00, 0x00, 0x00, 0x00, 0xFE, 0xFF, 0xFF, 0x01, 0x00, 0x00,
  0x00, 0x00, 0xFC, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0xFF,
  0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xFF, 0x1F, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x80, 0xFF, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  };

//---------------------------------------------------------------------------

// Accensione del display e visualizzazione della data e ora
void accendiDisplay()
{
  if(bootCount > 0 && numeroDisplay == 0)
  {
    ++numeroDisplay;
  }
  
  switch (numeroDisplay)
  {
    case 0:
    display.displayOn();
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.drawXbm(42, 2, 50, 50, Logo_Init_bits);
    display.setFont(ArialMT_Plain_10);
    display.drawString(64, 50, "BUBBLE BOX");
    display.display();
    delay(2000);
    ++numeroDisplay;
    break;
    
    case 1:
    display.displayOn();
    display.clear();
    display.drawXbm(95, 0, 19, 13, Wave_Logo_bits);
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

    case 2:
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

    case 3:
    display.displayOn();
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.drawXbm(34, 14, 60, 36, WiFi_Logo_bits);
    display.setFont(ArialMT_Plain_10);
    display.drawString(64, 50, "CONNESSIONE WPS");
    display.display();
    break;
  }
}

// Spegnimento diplay
void spegniDisplay()
{
  numeroDisplay = 1;
  display.displayOff();
}

void IRAM_ATTR controlloCambioDisplay()
{
  ++numeroDisplay;
  ControlTimeWake = 0; 
  if(numeroDisplay > 3)
   {
     numeroDisplay = 1;  
   }
}

void displayBubbleBoxContatto()
{
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_10);
  display.drawXbm(0, 0, 13, 12, Bubble_Trovato_bits);
  display.drawString(15, 0, " +");
  display.display();
}

void setup() 
{
  setSDCard(); // Setto l'SD card
  
  BubbleBoxTrovato = false; // setto la variabile del ritrovamento dispositivo a false
  
  pinMode(statoDisplay, INPUT_PULLUP);
  attachInterrupt(statoDisplay, controlloCambioDisplay, HIGH);
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
  
  //controlloCambioDisplay(); // Controllo per vedere se è stato premuto il tasto di cambio schermo
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