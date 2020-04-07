//----------------------SLEEP & WAKE ESP32----------------------------------------
//--------------------------------------------------------------------------------

#define BUTTON_PIN_BITMASK 0x8004 // GPIOs 2 and 15

// Settagio dello sleep&wake del device e partenza device
// Metodo per capire in che modo è avvenuto lo Sleep&Wake del device
void print_wakeup_reason()
{
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch(wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP : Serial.println("Wakeup caused by ULP program"); break;
    default : Serial.printf("Wakeup was not caused by deep sleep: %d\n",wakeup_reason); break;
  }
}

// Metodo per capire con quale touchpad è avvenuto il risveglio del device
void print_GPIO_wake_up(){
  int GPIO_reason = esp_sleep_get_ext1_wakeup_status();
  Serial.print("GPIO that triggered the wake up: GPIO ");
  Serial.println((log(GPIO_reason))/log(2), 0);
}

void setSleepWake()
{
  // Incremento il conteggio del boot del device
  ++bootCount;
  Serial.println("Boot number: " + String(bootCount));

  //Stampa del motivo del risveglio del dispositivo
  print_wakeup_reason();
  print_GPIO_wake_up();

  // Setta l'interrupt del device con il TouchPad 3 (GPIO15)
  esp_sleep_enable_ext1_wakeup(BUTTON_PIN_BITMASK,ESP_EXT1_WAKEUP_ANY_HIGH);

  Serial.println("Spegnimento...");
  esp_deep_sleep_start();
}

void callback(){
  //placeholder callback function
}
