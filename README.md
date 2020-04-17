# BUBBLEBOX DEVICE #

## INDICE ##
_________________________________________________________________________________________
1. Descrizione
2. Moduli e dispositivi utilizzati
3. inizializzazione
4. Directory Tree
5. Firmware Schede
6. Consumi, costi e caratteristiche
8. Connessione moduli(implementazione device)

______________________________________________________________________________________________

## Descrizione ##
________________________________________________________________________________________________

Sistema di rilevamento di prossimità per prevenire e rilevare l’espansione del COVID-19, 
attraverso l’uso di un device (da realizzare) o smartphone (con una App attraverso l’ausilio di tecnologie Bluetooth).
Oltre all’analisi dei dati, questo progetto, effettua soprattutto una raccolta puntuale,
completa e organizzata, con dati pronti per essere analizzati.
Il nome BubbleBox proviene proprio dall’idea di una bolla virtuale attorno ad ogni individuo che, 
se violata, da quella di un altro, ottiene ID di quest’ultimo e invia la segnalazione ad un server online per analisi e verifiche future. 
Se un individuo dovesse essere positivo al COVID-19 allora verranno allertate
e controllate tutte le persone con cui è entrato in contatto senza ambiguità alcuna. 
A differenza di alcuni sistemi di tracciamento già presenti, 
il nostro fornisce un servizio funzionante in ogni luogo e offline, perché la raccolta avviene 
prima in locale poi, quando disponibile, viene inoltrata online.

----------------------------------------------------------------------------------------------------

## Moduli e dispositivi utilizzati (singolo device)##
___________________________________________________________________________________________________

* ESP32
* RF-NANO
* Display LED
* Modulo RTC
* Modulo GSM [NON SUPPORTATO] >>>> Comunicazione Esterna sotituita in altro modo!
* Modulo scheda SD

-----------------------------------------------------------------------------------------------------

## Inizializzazione##

____________________________________________________________________________________________________

* Installazione [IDE Arduino](https://www.arduino.cc/en/Main/Software)
* Installazione CORE ESP32:
	- Arduino IDE > `File` > `Impostazioni` > `Schede` > URL Schede > `https://dl.espressif.com/dl/package_esp32_index.json`
	- Arduino IDE > `Strumenti` > `Scheda` > `ESP32 Dev Model`
	- Arduino IDE > `Strumenti` > `Partition Scheme `> `NO OTA (2MB APP/2MB SPIFFS)`
* Installazione Librerie:
	* Installazione [librerie ESP32](https://github.com/espressif/arduino-esp32)
		- Il link contiene tantissime librery per il device ESP32, tuttavia, nella repo è presente solamente la libvreria utile che viene utilizzata (per facilitare l'inizializzazione)
		- Arduino IDE > Sketch > #Include libreria > Aggiungi libreria da file .ZIP
	* Installazione [librerie NRF24L01](https://github.com/maniacbug/RF24)
		- Arduino IDE > Sketch > #Include libreria > Aggiungi libreria da file .ZIP
	
	----------> TUTTE LE LIBRERIE CHE SERVONO SONO INSERITE NELLA DIRECTORY `/Library`

_____________________________________________________________________________________________________
	
## Directory Tree ##
_____________________________________________________________________________________________________

* *Logo*: Logo del progetto e del device
* *Skecth Device*:
	* *ESP32*:
		* *Device_SempreAttivi*: Device con ESP32 sempre in funzione (grande spreco di batteria)
		* *Device_Sleep&Wake*: Device con ESP32 in DeepSleep (grande risparmio di batteria)
	* *RFNANO*: Modulo RFNano con NRF24L01 per scandagliare l'area alla ricerca dei device vicini (consuma meno del modulo ESP32 e viene usato solo per scandagliare l'area) funziona a supporto dell'ESP32
* *Business Plan e Presentazione* : Business plan e presentazione dell'intero progetto
* *Schema*: Immagine rappresentativa del funzionamento del device
* *Library*: Contiene tutte le librerie che servono ai componenti per poter funzionare

_______________________________________________________________________________________________________

## Firmware Schede ##
________________________________________________________________________________________________________
* Clonare la repo
	- `git clone https://orteip94@bitbucket.org/bbteam_/bubblebox-device.git`
* Installare gli sketch sulle schede:
	- RFNANO:
		- Aprire Arduino IDE
		- Inserire la scheda RFNAno
		- Arduino IDE > Strumenti > Scheda > Arduino Nano > `Processore:ATMega328P`
		- Selezionare Porta
		- Installare sketch contenuto nella directory `Sketch Device/RFNano`
	- ESP32:
		- Aprire `ESP32/Device_Sleep&Wake/ESP_WPS.h`
		- Modificare le righe 34 e 35 con le credenziali della BUBBLESTATION
		- Salvare e chiudere
		- Inserire la scheda ESP32
		- Arduino IDE > Strumenti > Scheda > `ESP32 Dev Model`
		- Arduino IDE > Strumenti > Partition Scheme > `NO OTA (2MB APP/2MB SPIFFS)`
		- Selezionare Porta
		- Installare sketch contenuto nella directory `Sketch Device/ESP32/Device_Sleep&Wake`
	
_______________________________________________________________________________________________________

## Consumi costi e caratteristiche componenti ##
________________________________________________________________________________________________________

| *Componente* | *Consumo Wake* |  *Consumo DeepSleep*  | *Prezzo* | *Caratteristiche* |
| -----------|---------|-------------|--------|-----------------|
|   ESP32    | 50mA    |  0,05 mA     |   9€   |   Wi-Fi & BLE   |
|   RF-NANO  | 19mA    |    0,05 mA   |   8€   |   Radio Wave    |
|Display OLED| 40mA    |   0,9 mA     |   8€   |   Display 0.96" |
| Lettore SD |  1mA    |   0 mA      |   5€   |   Data Storage  |
| Modulo RTC |0,8mA    | 0,08 mA     |   6€   | Orologio e data |
|Modulo GSM(NO) |  200mA    | 1 mA      | 9€     | Comunicazione dati tramite scheda SIM |
|  |   *Totale*  |  *Totale* |  *Totale*  |  |
|   - | 310,8(110) mA   |  2,08 mA     |   45€ (36€) |   -   |


## Connessioni Dispositivi(Implementazione Device) ##
_________________________________________________________________________________________________________

| *ESP32* | *RFNANO* |  *RTC*  | *DISPLAY* | *SD CARD* | *Bottone 1* | *Bottone 2*|
| -----------|---------|-------------|--------|-----------------|---------|----------|
|   GPIO 25    | D4   |  -    |   -  |   -   |  - |  -  |
|   GPIO 21    | -   |  SCL    |   SCL  |   -   |  -  |  - |
|   GPIO 22    | -   |  SDA  |   SDA  |   -   | -  |  - |
|   GPIO 19    | -   |  -    |   -  |   MISO  | -  |  - |
|   GPIO 23    | -   |  -    |   -  |   MOSI   | -  |  - |
|   GPIO 18    | -   |  -   |   -  |   SCK   | -  |  - |
|   GPIO 5    | -   |  -   |   -  |   CS   | -  |  - |
|   GPIO 14    | -   |  -   |   -  |   -   | OK(Cambio Display)  |  - |
|   GPIO 26    | -   |  -   |   -  |   -   | -  |  OK(Cambio Display) |
|   GPIO 2    | -   |  -   |   -  |   -   | -  |  OK(WAKE) |
|   GPIO 15    | -   |  -   |   -  |   -   | OK(WAKE)  |  - |
|   3V    | -   |  VCC  |   VCC  |   -   | VCC  |  VCC |
|   5V    | -   |  -  |   -  |   VCC   | -  |  - |
|   GND    | -   |  GND  |   GND  |   GND   | GND  |  GND |

[Circuito Completo](https://bitbucket.org/bbteam_/bubblebox-device/src/master/Circuito/Circuito_Completo.jpg)