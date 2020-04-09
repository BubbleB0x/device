//--------------------------BLE ESP32--------------------------------------------
//--------------------------------------------------------------------------------

// Librerie ESP32 BLE 
#include <BLEAdvertisedDevice.h>
#include <BLEDevice.h>
#include <BLEScan.h>
#include <BLEUtils.h>
#include <Arduino.h>
#include <BLESecurity.h>

// Caratteristiche rete BLE del decvice
//-- #define SERVICE_UUID "4FAFC201-1FB5-459E-8FCC-C5C9C331914B"      //Service UUID BUBBLESTATION
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define SERVICE_UUID "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"           // Service UUID BUBBLEBOX
#define CHARACTERISTIC_UUID_RX "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
BLECharacteristic *pCharacteristic;
BLEServer *pServer;
BLEService *pService;
BLEScan* pBLEScan;

String RitornoMAC = "";
String BubbleBox_UUID = "6e400001-b5a3-f393-e0a9-e50e24dcca9e";
String BubbleStation_UUID = "4fafc201-1fb5-459e-8fcc-c5c9c331914b";

/*
 * Scansione area per il ritrovamento di altri device BLE
 */
String scanArea()
{
  RitornoMAC = "";
  Serial.print("\n#############################################################");
  Serial.print("\n###### My Address MAC BLE: ");
  Serial.print(MyMAC);
  Serial.print("\n#############################################################");
  pBLEScan = BLEDevice::getScan();
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
    if(device.haveServiceUUID())
    {
      UUID = device.getServiceUUID().toString().c_str();
      Serial.print(" | UUID:");
      Serial.print(UUID);
    }

    // Controllo del probabile contatto con un'altra persona che possiede un altro BubbleBox
    if(UUID == BubbleBox_UUID && Rssi_device > -75)
    {
      BubbleBoxTrovato = true;
      scriviContatto(MAC);
      Serial.println("\n\n------------------------CONSIDERA IL SEGNALE----------------------------");
      Serial.println("\n----------------------------CONTATTO AVVENUTO!----------------------------");
    }
    if(UUID == BubbleStation_UUID)
    {
      Serial.println("\n-------------------------BUBBLESTATION TROVATA---------------------------");
      RitornoMAC = MAC;
    }
    UUID = "";
  }
  Serial.print("\n__________________________________________________________________________");
  return RitornoMAC;
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
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ |
                      BLECharacteristic::PROPERTY_WRITE
                      );
  pCharacteristic->setValue("I'm BubbleBox");
  pService->start();
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  BLEDevice::startAdvertising();
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
