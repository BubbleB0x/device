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
        Serial.write(file.read());
        file.close();
        return true;
    }
}

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

    nomeFile = "/contacts_local/contacts" + NomeFile + ".txt";
    if(!readFile(SD, nomeFile.c_str()))
    {
      writeFile(SD, nomeFile.c_str(), "");
    }
}

void scriviContatto(String contact, String Ora, String DataFile)
{
  if(Ora != "" && DataFile != "")
  {
    String inserimento = Ora + "|" + MyMAC + "|" + contact + "\n"; 
    String namefile = "/contacts_local/contacts" + DataFile + ".txt";
    appendFile(SD, namefile.c_str(), inserimento.c_str());
  }
}
