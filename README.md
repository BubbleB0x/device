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
9. Funzionamento

______________________________________________________________________________________________

## Descrizione ##
________________________________________________________________________________________________

Sistema di rilevamento di prossimità per prevenire e rilevare l’espansione del COVID-19 (e altre forme virali), attraverso l’uso di un device e di un App (non essenziale, ma che fornisce funzionalità aggiuntive) smartphone e web. Oltre all’analisi dei dati, questo progetto, effettua soprattutto una raccolta puntuale, completa, precisa e organizzata, con dati pronti per essere analizzati, infatti, il device effettuerà un vero proprio Edge Computing, in modo da gestire lui stesso i dati e presentarli, attraverso più canali comunicativi (Bluetooth smartphone o WiFi), al sistema server. Il nome BubbleBox proviene proprio dall’idea di una bolla virtuale attorno ad ogni individuo che, se violata, da quella di un altro, ottiene ID di quest’ultimo (in modo sicuro e anonimo) e invia la segnalazione ad un server online, dopo essere stato elaborato, per analisi e verifiche future. Se un individuo dovesse essere positivo al COVID-19 allora verranno allertate e controllate tutte le persone con cui è entrato in contatto senza ambiguità alcuna. A differenza di alcuni sistemi di tracciamento già presenti, il nostro fornisce un servizio funzionante in ogni luogo e offline, perché la raccolta avviene prima in locale poi, quando disponibile, viene inoltrata online.

---> [REPORT](https://github.com/BubbleB0x/device/blob/master/doc/report/Relazione_Bubblebox(RignanesePolenta).pdf)

----------------------------------------------------------------------------------------------------

## Moduli e dispositivi utilizzati (singolo device) ##
___________________________________________________________________________________________________

* ESP32
* RF-NANO
* Display LED
* Modulo RTC
* Modulo scheda SD

-----------------------------------------------------------------------------------------------------

## Inizializzazione ##

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
	
* Possibili problematiche: 
	- è possibile che quando si connette l'ESP32 alla porta USB del PC non venga letta
	- aprire `Gestione Dispoitivi`
	- vedere se tra i device presenti c'è qualche problema o warning
	- andare sul problema e cliccare col tasto destro > `Aggiorna Driver`
	- il problema dovrebbe essere rsolto riavviando la IDE di Arduino

_____________________________________________________________________________________________________
	
## Directory Tree ##
_____________________________________________________________________________________________________

* *img*: Logo e altre immagini
* *firmware*:
	* *ESP32*:
		* *Device_SempreAttivi*: Device con ESP32 sempre in funzione (grande spreco di batteria)
		* *Device_Sleep&Wake*: Device con ESP32 in DeepSleep (grande risparmio di batteria)
	* *RFNANO*: Modulo RFNano con NRF24L01 per scandagliare l'area alla ricerca dei device vicini (consuma meno del modulo ESP32 e viene usato solo per scandagliare l'area) funziona a supporto dell'ESP32
* *doc* : 
	* *business_plan*
	* *report*
	* *altri documenti*
* *scheme*: Schemi sul funzionamento del device
* *Library*: Contiene tutte le librerie che servono ai componenti per poter funzionare
* *circuits*

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
		- Aprire Arduino IDE
		- Inserire la scheda ESP32
		- Arduino IDE > Strumenti > Scheda > `ESP32 Dev Model`
		- Arduino IDE > Strumenti > Partition Scheme > `NO OTA (2MB APP/2MB SPIFFS)`
		- Selezionare Porta
		- Installare sketch contenuto nella directory `firmware/ESP32/Device_Sleep&Wake`
	
_______________________________________________________________________________________________________

## Consumi costi e caratteristiche componenti ##
________________________________________________________________________________________________________

| *Componente* | *Consumo Wake* |  *Consumo DeepSleep*  | *Prezzo* | *Caratteristiche* |
| -----------|---------|-------------|--------|-----------------|
|   ESP32    | 50mA    |  0,05 mA     |   9€   |   Wi-Fi & BLE   |
|   RF-NANO  | 19mA    |    0,05 mA   |   8€   |   Radio Wave    |
|Display OLED| 30mA    |   0,9 mA     |   8€   |   Display 0.96" |
| Lettore SD |  30mA    |   5 mA      |   5€   |   Data Storage  |
| Modulo RTC |0,8mA    | 0,08 mA     |   6€   | Orologio e data |
|  |   *Totale*  |  *Totale* |  *Totale*  |  |
|   - | 130 mA   |  6,08 mA     |   36€ |   -   |


## Connessioni Dispositivi(Implementazione Device) ##
_________________________________________________________________________________________________________

<img src="https://github.com/BubbleB0x/device/blob/master/circuits/Circuito_Fritzing.png"/>

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
|   GPIO 26    | -   |  -   |   -  |   -   | -  |  OK(Sel. Opzione Display) |
|   GPIO 2    | D4(Wake)  |  -   |   -  |   -   | -  |  - |
|   GPIO 15    | -   |  -   |   -  |   -   | OK(WAKE)  |  - |
|   3V    | -   |  VCC  |   VCC  |   -   | VCC  |  VCC |
|   5V    | -   |  -  |   -  |   VCC   | -  |  - |
|   GND    | -   |  GND  |   GND  |   GND   | GND  |  GND |

<img src="https://github.com/BubbleB0x/device/blob/master/circuits/Circuito_Semplice_2.jpg"/>

## Funzionamento ##

<img src="https://github.com/BubbleB0x/device/blob/master/scheme/Funzionamento_Generico.PNG"/>

<img src="https://github.com/BubbleB0x/device/blob/master/scheme/Funzionamento_Device.PNG"/>

### Flow-Chart ###

<img src="https://github.com/BubbleB0x/device/blob/master/scheme/Funzionamento_FlowChart.png"/>
