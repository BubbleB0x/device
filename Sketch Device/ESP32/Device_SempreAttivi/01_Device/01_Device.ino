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

#include <BLEAdvertisedDevice.h>
#include <BLEDevice.h>
#include <BLEScan.h>
#include <BLEUtils.h>
#include <Arduino.h>
#include <BLESecurity.h>

/*
 * Nome e MAC dei dispositivi cercati
 */
String MAC;
String Name;
String Manufacturer;
int Rssi_device = 0;

// MAC Address BLE di questo dispositivo
String MyMAC;
// Controllo per il ritrovamento, in zona, di un device BubbleBox
bool deviceBubble = false;

// Caratteristiche rete BLE del decvice
#define SERVICE_UUID "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"  // BubbleBox Device
// #define SERVICE_UUID "4FAFC201-1FB5-459E-8FCC-C5C9C331914B"  // BubbleBox Station
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
BLECharacteristic *pCharacteristic;
BLEServer *pServer;
BLEService *pService;

void setup() 
{
  Serial.begin(115200);
  enableBLE(); //Abilitare il Bluetooth Low Energy
}

void loop() 
{
  // Scansione area per trovare i device con BLE nelle vicinanze
  String A = scanArea(); 
  Serial.print(A);
  // Disconnesione dei dispositivi che si connettono
  disconnectedDeviceBLE();
}

//----------------------------------ESP32 BLE-----------------------------------------
//------------------------------------------------------------------------------------
String scanArea()
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
    if(device.haveServiceUUID())
    {
      Serial.print(" | UUID:");
      Serial.print(device.getServiceUUID().toString().c_str());
    }

    // Controllo del probabile contatto con un'altra persona che possiede un altro BubbleBox
    if(Name == "BubbleBox" && Rssi_device > -75)
    {
      Serial.println("\n\n------------------------CONSIDERA IL SEGNALE----------------------------");
      Serial.println("\n----------------------------CONTATTO AVVENUTO!----------------------------");
      return "OK";
    }
  }
  Serial.print("\n__________________________________________________________________________");
  return "NO";
}

/*
 * Attivazione del BLE e di tutti i servizi annessi
 */
void enableBLE()
{
  BLEAdvertisementData pAdvertisementData;
  BLEDevice::init("BubbleStation"); // Init del BLE e nome device --> BubbleBox
  pServer = BLEDevice::createServer();
  pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ |
                      BLECharacteristic::PROPERTY_WRITE
                      );
  pCharacteristic->setValue("I'm BubbleStation");
  pService->start();
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  BLEDevice::startAdvertising();
  BLEAddress  ADD = BLEDevice::getAddress();
  MyMAC = ADD.toString().c_str();
  Serial.print("\n\nMy Address:");
  Serial.print(MyMAC);
  Serial.print("\n----------------------------------------------");
}

// Disconnette i dispositivi bluetooth che si connettono all'ESP-32
void disconnectedDeviceBLE()
{
  pServer->disconnect(pServer->getConnId());
}
