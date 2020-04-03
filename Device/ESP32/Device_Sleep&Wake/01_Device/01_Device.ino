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

// Nome, MAC Address, Rssi e Manifattura del dispositivo trovato
String MAC;
String Name;
String Manufacturer;
int Rssi_device = 0;

// MAC Address BLE di questo dispositivo
String MyMAC;

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

void setup() 
{
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
  accendiDisplay();
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

// Accensione del display e visualizzazione della data e ora
void accendiDisplay()
{
  if (ControlTimeWake < 10)
  {
    display.displayOn();
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.setFont(ArialMT_Plain_24);
    display.drawRect(25, 15, 80, 35);
    display.drawString(64, 20, "00:00");
    display.setFont(ArialMT_Plain_10);
    display.drawString(64, 50, "01-01-2000");
    display.display();
  }
  else
  {
    display.displayOn();
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.setFont(ArialMT_Plain_24);
    display.drawCircle(64, 20, 30);
    display.drawString(64, 20, "10");
    display.setFont(ArialMT_Plain_10);
    display.drawString(64, 50, "PERSONE INCONTRATE");
    display.display();
  }
  
}

// Spegnimento diplay
void spegniDisplay()
{
  display.displayOff();
}