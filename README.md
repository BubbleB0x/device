# BUBBLEBOX DEVICE #
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
* Modulo GSM

-----------------------------------------------------------------------------------------------------

## Inizializzazione##

____________________________________________________________________________________________________

* Installazione [IDE Arduino](https://www.arduino.cc/en/Main/Software)
* Installazione CORE ESP32:
	- Arduino IDE > File > Impostazioni > Schede > URL Schede > "https://dl.espressif.com/dl/package_esp32_index.json"
	- Arduino IDE > Strumenti > Scheda > Gestore Schede > ESP32
	- Arduino IDE > Strumenti > Scheda > DOIT ESP32 DEVKIT V1 
* Installazione [librerie ESP32](https://github.com/espressif/arduino-esp32)
	- Il link contiene tantissime librery per il device ESP32, tuttavia, nella repo è presente solamente la libvreria utile che viene utilizzata (per facilitare l'inizializzazione)
	- Arduino IDE > Sketch > #Include libreria > Aggiungi libreria da file .ZIP
* Installazione [librerie NRF24L01](https://github.com/maniacbug/RF24)
	- Arduino IDE > Sketch > #Include libreria > Aggiungi libreria da file .ZIP

_____________________________________________________________________________________________________
	
## Directory Tree ##
_____________________________________________________________________________________________________

* *Logo*: Logo del progetto e del device
* *Device*: Moduli che compongono il device
	* *ESP32*:
		* *Device_SempreAttivi*: Device con ESP32 sempre in funzione (grande spreco di batteria)
		* *Device_Sleep&Wake*: Device con ESP32 in DeepSleep (grande risparmio di batteria)
	* *RFNANO*: Modulo RFNano con NRF24L01 per scandagliare l'area alla ricerca dei device vicini (consuma meno del modulo ESP32 e viene usato solo per scandagliare l'area) funziona a supporto dell'ESP32
* *Business PLan e Presentazione* : Business plan e presentazione dell'intero progetto
* *Schema*: Immagine rappresentativa del funzionamento del device

_______________________________________________________________________________________________________

## Firmware Schede ##
________________________________________________________________________________________________________
* Clonare la repo
	- `git clone https://orteip94@bitbucket.org/bbteam_/bubblebox-device.git`
* Installare i firmware sulle schede:
	- Aprire Arduino IDE
	- Inserire la scheda RFNAno
	- Arduino IDE > Strumenti > Scheda > Arduino Nano > Processore:ATMega328P
	- Installare sketch contenuto nella directory `Device/RFNano`
	- Inserire la scheda ESP32
	- Arduino IDE > Strumenti > Scheda > DOIT ESP32 DEVKIT V1 
	- Installare sketch contenuto nella directory `Device/ESP32/Device_Sleep&Wake`