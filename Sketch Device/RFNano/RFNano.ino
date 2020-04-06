/*
 * --------------------------BUBBLEBOX ver. 1.0--------------------------------------
 * 
 * Device RF-NANO
 * 
 * Pietro Rignanese & Andrea Polenta 2020
 * 
 * UNIVPM
 * 
 * prof: Aldo Franco Dragoni
 * 
 */

// Librerie per la gestione del modulo wirless
#include <SPI.h>
#include "RF24.h"

// Librerie per lo sleep&wake
#include "LowPower.h"

// Segnale di accensione dell'ESP32
const int AccensioneBubbleBox = 4;

// Settaggio modulo radio
RF24 radio(10, 9); //CE - CSN
const uint64_t pipe = 0xE8E8F0F0E1LL;

//----------STRUTTURA DATI DA MANDARE E RICEVERE---------------
struct package 
{
  char id[20] = "00:01:11:23"; // MAC Address Device
  unsigned long timeStamp;
};

// Strutture dati da ricevere e mandare
typedef struct package Package;
Package dataRicev;
Package dataTransmit;
//--------------------------------------------------------------


// Tempo di accensione del device
int clockTime;

void setup() {
  clockTime = 0;
  
  pinMode(AccensioneBubbleBox, OUTPUT);
  Serial.begin(115200);
  delay(1000);

  radio.begin();
  radio.setChannel(115);
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_250KBPS);
  radio.openReadingPipe(1, pipe);
  radio.startListening();
  radio.powerDown();
}

void loop() 
{
  ++clockTime;
  sleepWake();
  
  ricezione();
  delay(500);
  radio.stopListening();
  trasmissione();
}

//--------------RICEZIONE MESSAGGI------------------------
void ricezione()
{
  if(radio.available()) {
    while (radio.available()){
      radio.read(&dataRicev, sizeof(dataRicev));
    }
    clockTime = 0;
    digitalWrite(AccensioneBubbleBox, HIGH);
    Serial.println("________________RICEZIONE__________________ ");
    Serial.println("Package: ");
    Serial.print("ID:");
    Serial.print(dataRicev.id);
    Serial.print("\nYour/My Time:");
    Serial.print(dataRicev.timeStamp);
    Serial.println("\n------------------------------");
  }
}

//--------------TRASMISSIONE MESSAGGI-------------------------
void trasmissione()
{
  unsigned long now = millis();
  dataTransmit.timeStamp = now;
  digitalWrite(AccensioneBubbleBox, LOW);
  Serial.println("\n\n________________TRASMISSIONE___________________");
  Serial.println("Package: ");
  Serial.print("ID:");
  Serial.print(dataTransmit.id);
  Serial.print("\nMy Time:");
  Serial.print(dataTransmit.timeStamp);
  Serial.println("\n--------------------------------");

  radio.openWritingPipe(pipe);
  radio.write(&dataTransmit, sizeof(dataTransmit));
  radio.openReadingPipe(1, pipe);
  radio.startListening();
}

//----------------------SLEEP & WAKE  MODULO----------------------
void sleepWake()
{
  // Quando il modulo non ha trovato alcun dispositivo nelle vicinanze, si spegne per 4 secondi e poi riparte
  // Appena trova un device nelle vicinanze comincia a trasmettere senza mai fermarsi e tiene acceso l'ESP32
  if (clockTime > 15)
  {
    clockTime = clockTime - 1;
    LowPower.powerDown(SLEEP_4S, ADC_OFF, BOD_OFF); 
  }
}
