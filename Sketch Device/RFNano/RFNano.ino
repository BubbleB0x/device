#include <SPI.h>
#include "RF24.h"

const int AccensioneBubbleBox = 4;

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
void setup() {
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
