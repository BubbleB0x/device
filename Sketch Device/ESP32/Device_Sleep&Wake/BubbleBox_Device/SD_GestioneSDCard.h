//-----------------------SD CARD DATA STORAGE & SETUP------------------------------------
//--------------------------------------------------------------------------------

/*
 *    Connettere Il modulo SD Card all'ESP32
 *    
 *    SD CARD   -   ESP32
 *      VCC           V5
 *      GND          GND
 *      MISO        GPIO 19
 *      MOSI        GPIO 23
 *      SCK         GPIO 18
 *      CS          GPIO 5
 *      
 *      Libreria contenuta nelle library ufficiali dell'ESP32 ESSPRESSIF: https://github.com/espressif/arduino-esp32/tree/master/libraries/SD
 */
 
// Librerie Modulo scheda SD per memorizzare i dati
#include "FS.h"
#include "SD.h"
#include "SPI.h"

String nomeFile;
String nomeFile2;
String bufferFile;

String oraUltimoContatto;             // Ora del contatto
String minutoUltimoContatto;          // Minuto del contattoù
String nomeUltimoContatto;            // Nome del contatto

int numeroContatti = 0;

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

void createDir(fs::FS &fs, const char * path){
    Serial.printf("Creating Dir: %s\n", path);
    if(fs.mkdir(path)){
        Serial.println("Dir created");
    } else {
        Serial.println("mkdir failed");
    }
}

bool readFile(fs::FS &fs, const char * path){
    Serial.printf("Reading file: %s\n", path);

    File file = fs.open(path);
    if(!file){
        Serial.println("Failed to open file for reading");
        return false;
    }

    Serial.print("Read from file: ");
    while(file.available()){
        file.close();
        return true;
    }
}

//------------- SETTAGGIO SD CARD ---------------------------
void setSDCard(String NomeFile)
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

    nomeFile = "/contacts" + NomeFile + ".txt";                  // Nome del file
    if(!readFile(SD, nomeFile.c_str()))                          // Se il file giornaliero esiste non se ne crea un altro
    {
      writeFile(SD, nomeFile.c_str(), "");                       // creazione del file dis storage giornaliero
    }
    nomeFile2 = "/contacts_all.txt";                              // Nome del file
    if(!readFile(SD, nomeFile2.c_str()))                         // Se il file giornaliero esiste non se ne crea un altro
    {
      writeFile(SD, nomeFile2.c_str(), "");                      // creazione del file dis storage giornaliero
    }
}

//------------- Controllo il file riga per riga per verificare i contatti e non considerare quelli in meno di 10 minuti --------------------
bool readFileByLine(fs::FS &fs, const char * path, String contact){
    Serial.printf("Reading file: %s\n", path);
    nomeUltimoContatto = "";
    oraUltimoContatto = "";
    minutoUltimoContatto = "";

    File file = fs.open(path);
    
    Serial.print("Read from file: ");

    //------------- CONTROLLO IL CONTENUTO DEL FILE PER VERIFICARE L'ULTIMO CONTATTO AVVENUTO DA UN DETERMINATO MAC ADDRESS -----------------
    while(file.available())
    {
        bufferFile = file.readStringUntil('\n');
        nomeUltimoContatto = bufferFile.substring(35, 52);              // Nome del contatto
        if(nomeUltimoContatto == contact.substring(35, 52))
        {
          oraUltimoContatto = bufferFile.substring(11, 13);                // Ora del contatto
          minutoUltimoContatto = bufferFile.substring(14, 16);             // Minuto del contatto
        }
    }
    file.close();
    //-------------- CONSIDERO L'ULTIMO CONTATTO AVVENUTO CON IL DISPOSITIVO BUBBLEBOX TROVATO ------------------
    if(oraUltimoContatto == contact.substring(11,13))
    {
      if(minutoUltimoContatto != contact.substring(14,16))
      {
        int delta = contact.substring(14,16).toInt() - minutoUltimoContatto.toInt();
        if(delta > 10)                                                   // Controllo lo scarto di tempo tra il contatto e l'ultima volta che avvenuto lo stesso contatto
        {                                                                // --> Considero 10 min
          return true;
        }
      }
      return false;
    }
    else
    {
      return true;
    }
}

//---------------- Salvataggio sul file in locale del contatto avvenuto -----------------------------
void scriviContatto(String contact, String Ora, String DataFile)
{
  if(Ora != "" && DataFile != "")
  {
    String inserimento = DataFile + "|" + Ora + "|" + MyMAC + "|" + contact + "\n"; 
    String namefile = "/contacts" + DataFile + ".txt";
    if(readFileByLine(SD, namefile.c_str(), inserimento))                     // Lettura intero file per verificare il contenuto 
    {
      appendFile(SD, namefile.c_str(), inserimento.c_str());                  // Scrittura contatto in coda al file
      appendFile(SD, "/contacts_all.txt", inserimento.c_str());
    }
  }
}

//-------- Lettura del file per verificare il numero dei contatti avvenuti in giornata ----------------
String readCountContact(fs::FS &fs, const char * path){
  numeroContatti = 0;
    Serial.printf("Reading file: %s\n", path);

    File file = fs.open(path);
    if(!file){
        Serial.println("Failed to open file for reading");
        return (String)numeroContatti;
    }

    Serial.print("Read from file: ");
    while(file.available()){
        bufferFile = file.readStringUntil('\n');        // veridfico che il file contine delle righe --> ogni riga equivale ad un contatto
        ++numeroContatti;                               // Il numero dei contatti sale a +1
    }
    file.close();
    return (String)numeroContatti;
}

//------------- CONTEGGIO CONTATTI -------------------------------------------------------------
String countContatti(String DataFile)
{
  String namefile = "/contacts" + DataFile + ".txt";
  return readCountContact(SD, namefile.c_str());        // Restituisco il numero di righe contenute nel file odierno --> equivale al numero di contatti avvenuti in giornata
}
