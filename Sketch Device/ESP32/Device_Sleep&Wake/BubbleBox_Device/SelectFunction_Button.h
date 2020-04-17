/*
 *                Interrupt button ESP32
 *                link: https://lastminuteengineers.com/handling-esp32-gpio-interrupts-tutorial/
 */

//----------- Quando viene premenuto il Bottone 2 e ci si trova nella schermata della connessione BLT (Smartphone o station) 
//----------- vine visualizzata la schermata opportuna

void IRAM_ATTR selectFunction()
{
  if(numeroDisplay == 4)
    {
      Serial.println();
      Serial.println("--------- INVIO DATI SMARTPHONE --->");
      ControlTimeWake = 0;
      numeroDisplay = 8;
      statoBLT = true;
      smartphoneConnect = true;
      bubbleStation = false;
    } 
   if(numeroDisplay == 3)
   {
    Serial.println();
    Serial.println("--------- CONNESSIONE WPS ----->");
    ControlTimeWake = 0;
    numeroDisplay = 7;
    statoBLT = true;
    smartphoneConnect = false;
    bubbleStation = false;
   }
   if(numeroDisplay == 5)
   {
    Serial.println();
    Serial.println("--------- CONNESSIONE STATION ----->");
    ControlTimeWake = 0;
    numeroDisplay = 10;
    statoBLT = true;
    smartphoneConnect = false;
    bubbleStation = true;
   }
}
