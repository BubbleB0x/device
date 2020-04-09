/*
 * ------ Sito per creare BitMap delle immagini in XBM:
 * 
 *        https://www.online-utility.org/image/convert/to/XBM
 *        
 * ------------------------------------------------------------------------------------------
 */

const uint8_t Logo_Init_bits[] PROGMEM = {
  0x00, 0x00, 0x80, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xFF, 0x01, 
  0x00, 0x00, 0x00, 0x00, 0xF8, 0xFB, 0x07, 0x00, 0x00, 0x00, 0x00, 0x3C, 
  0xE0, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x1E, 0xFE, 0x1F, 0x00, 0x00, 0x00, 
  0x00, 0x8F, 0xFF, 0x1F, 0x00, 0x00, 0x00, 0x00, 0xC7, 0xFF, 0x3F, 0x00, 
  0x00, 0x00, 0x80, 0xE3, 0xFF, 0x7F, 0x00, 0x00, 0x00, 0x80, 0xF3, 0xFF, 
  0x7F, 0x00, 0x00, 0x00, 0x80, 0xF9, 0xFF, 0x7F, 0x00, 0x00, 0x00, 0xC0, 
  0xFD, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xC0, 0xFD, 0xFF, 0xFF, 0x00, 0x00, 
  0x00, 0xC0, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x80, 0xDF, 0xFF, 0xFF, 0xFF, 
  0x00, 0x00, 0xC0, 0xDF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xE0, 0xDC, 0xFF, 
  0xFF, 0xFF, 0x00, 0x00, 0x70, 0x9E, 0xFF, 0xFF, 0x7F, 0x00, 0x00, 0x38, 
  0xBF, 0xFF, 0xFF, 0x7F, 0x00, 0x00, 0x98, 0xBF, 0xFF, 0xFF, 0x7F, 0x00, 
  0x00, 0x98, 0x7F, 0xFF, 0xFF, 0xBF, 0x01, 0x00, 0xD8, 0x7F, 0xFE, 0xFF, 
  0x9F, 0x07, 0x00, 0xD8, 0xFF, 0xFE, 0xFF, 0xDF, 0x1F, 0x00, 0xF8, 0xFF, 
  0xFD, 0xFF, 0xE7, 0x19, 0x00, 0xF8, 0xFF, 0xF3, 0xFF, 0xF3, 0x3B, 0x00, 
  0xF0, 0xFF, 0xE1, 0xFF, 0xF8, 0x77, 0x00, 0xF0, 0xFF, 0x01, 0x1E, 0xFE, 
  0x67, 0x00, 0xE0, 0xFF, 0x00, 0xC0, 0xFF, 0x67, 0x00, 0x80, 0x3F, 0x00, 
  0xC0, 0xFF, 0x6F, 0x00, 0x00, 0x0A, 0x00, 0xC0, 0xFF, 0x6F, 0x00, 0x00, 
  0x00, 0x00, 0xC0, 0xFF, 0x67, 0x00, 0x00, 0x00, 0x00, 0x80, 0xFF, 0x77, 
  0x00, 0x00, 0xC0, 0x0F, 0x80, 0xFF, 0x3F, 0x00, 0x00, 0xF0, 0x1F, 0x00, 
  0xFF, 0x3F, 0x00, 0x00, 0x38, 0x3E, 0x00, 0xFF, 0x1F, 0x00, 0x00, 0x98, 
  0x7F, 0x00, 0xFE, 0x0F, 0x00, 0x00, 0xCC, 0x7F, 0x00, 0xF8, 0x03, 0x00, 
  0x00, 0xEC, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0xEC, 0xFF, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0xFC, 0xFF, 0xC0, 0x01, 0x00, 0x00, 0x00, 0xFC, 0xFF, 
  0xF0, 0x07, 0x00, 0x00, 0x00, 0xFC, 0x7F, 0x38, 0x0F, 0x00, 0x00, 0x00, 
  0xF8, 0x7F, 0xDC, 0x1F, 0x00, 0x00, 0x00, 0xF0, 0x3F, 0xEC, 0x1F, 0x00, 
  0x00, 0x00, 0xE0, 0x1F, 0xEC, 0x3F, 0x00, 0x00, 0x00, 0xC0, 0x07, 0xFC, 
  0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x1F, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0xFC, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x0F, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0xF0, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x03, 
  0x00, 0x00,
};
const uint8_t Bubble_Trovato_bits[] PROGMEM = {
  0x40, 0x00, 0xF8, 0x03, 0x1C, 0x07, 0x06, 0x0C, 0x16, 0x0C, 0x0A, 0x08, 
  0x02, 0x0C, 0x06, 0x0C, 0x0C, 0x06, 0x1C, 0x07, 0xF0, 0x01, 0x00, 0x00,
};
const uint8_t Bluetooth_Logo_bits[] PROGMEM = {
  0x00, 0x00, 0x40, 0x00, 0xC0, 0x00, 0x40, 0x01, 0x50, 0x01, 0xE0, 0x00, 
  0x40, 0x00, 0xE0, 0x00, 0x50, 0x01, 0x40, 0x01, 0xC0, 0x00, 0x40, 0x00, 
  0x00, 0x00,
};
const uint8_t SmartphoneSend_Logo_bits[] PROGMEM = {
  0x00, 0x00, 0xFC, 0xFF, 0xFF, 0x0F, 0x00, 0x00, 0x00, 0xFE, 0xFF, 0xFF, 
  0x1F, 0x00, 0x00, 0x00, 0xFF, 0x87, 0xF8, 0x1F, 0x00, 0x00, 0x00, 0xFF, 
  0xBF, 0xFF, 0x1F, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x1F, 0x00, 0x00, 
  0x00, 0x47, 0x22, 0x28, 0x19, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x18, 
  0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x07, 0x0F, 
  0x00, 0x18, 0x00, 0x00, 0x00, 0xC3, 0x0F, 0x00, 0x18, 0x00, 0x00, 0x00, 
  0xF7, 0x0F, 0x00, 0x18, 0x00, 0x00, 0x00, 0xFF, 0x0F, 0x00, 0x18, 0x00, 
  0x00, 0x00, 0xFF, 0x0F, 0x00, 0x18, 0x00, 0x00, 0x80, 0xFF, 0x07, 0x00, 
  0x18, 0x00, 0x00, 0xE0, 0xFF, 0x01, 0x00, 0x18, 0x00, 0x00, 0xF8, 0xFF, 
  0x00, 0x00, 0x18, 0x00, 0x00, 0xFE, 0x7F, 0x00, 0x00, 0x7F, 0x00, 0x00, 
  0xFF, 0x3F, 0x00, 0x80, 0xFF, 0x01, 0xC0, 0xFF, 0x0F, 0x00, 0xC0, 0xFF, 
  0x01, 0xE0, 0xFF, 0x07, 0x00, 0xC0, 0xFF, 0x01, 0xF0, 0xFF, 0x07, 0x00, 
  0x80, 0xFF, 0x01, 0xF8, 0xFF, 0x07, 0x00, 0x80, 0xFF, 0x00, 0xFC, 0xFF, 
  0x87, 0x03, 0x00, 0x00, 0x00, 0xFC, 0xFF, 0x83, 0x1F, 0x80, 0xFF, 0x00, 
  0xFC, 0xFF, 0x07, 0x78, 0xC0, 0xFF, 0x01, 0xFE, 0xFF, 0x83, 0xE3, 0xC0, 
  0xFF, 0x01, 0xFE, 0xFF, 0x83, 0xCF, 0xC1, 0xFF, 0x01, 0xFE, 0xFF, 0x07, 
  0x9C, 0x81, 0xFF, 0x01, 0xFE, 0xFF, 0x87, 0x31, 0x03, 0xBB, 0x00, 0xFE, 
  0xFF, 0x87, 0x67, 0x06, 0x00, 0x00, 0xFE, 0xFF, 0x03, 0xCE, 0x86, 0xFF, 
  0x00, 0xFE, 0xFF, 0x07, 0xDC, 0xC6, 0xFF, 0x01, 0xFE, 0xFF, 0x03, 0xD8, 
  0xC6, 0xFF, 0x01, 0xFE, 0xFF, 0x07, 0x98, 0xCC, 0xFF, 0x01, 0xFE, 0xFF, 
  0x07, 0x10, 0x80, 0xFF, 0x00, 0xFC, 0xFF, 0x07, 0x00, 0x00, 0x00, 0x00, 
  0xFC, 0xFF, 0x07, 0x00, 0x00, 0x7E, 0x00, 0xF8, 0xFF, 0x03, 0x00, 0x80, 
  0xFF, 0x01, 0xF8, 0xFF, 0x07, 0x00, 0xC0, 0xFF, 0x01, 0xF0, 0xFF, 0x03, 
  0x00, 0xC0, 0xFF, 0x01, 0xC0, 0xFF, 0x07, 0x00, 0xC0, 0xFF, 0x01, 0x80, 
  0xFF, 0x03, 0x00, 0x80, 0xFF, 0x00, 0x00, 0xFC, 0x07, 0x00, 0x00, 0x18, 
  0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0xFF, 0xFF, 
  0xFF, 0x1F, 0x00, 0x00, 0x00, 0xFF, 0x3F, 0xFF, 0x1F, 0x00, 0x00, 0x00, 
  0xFF, 0x1F, 0xFF, 0x1F, 0x00, 0x00, 0x00, 0xFF, 0x1F, 0xFF, 0x1F, 0x00, 
  0x00, 0x00, 0xFE, 0xFF, 0xFF, 0x1F, 0x00, 0x00, 0x00, 0xFC, 0xFF, 0xFF, 
  0x0F, 0x00,
};
const uint8_t Smartphone_Logo_bits[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x80, 0x99, 0xA6, 0x01, 0xF8, 0xFF, 0xFF, 0x0F, 
  0xF8, 0x7F, 0xFF, 0x1F, 0xFC, 0xFF, 0xFF, 0x1F, 0xFC, 0xFF, 0xFF, 0x1F, 
  0xFC, 0xFF, 0xFF, 0x1F, 0x04, 0x00, 0x00, 0x10, 0x04, 0x00, 0x00, 0x10, 
  0x04, 0x00, 0x00, 0x10, 0x04, 0x00, 0x00, 0x10, 0x04, 0x00, 0x00, 0x10, 
  0x04, 0x00, 0x00, 0x10, 0x04, 0x40, 0x00, 0x10, 0x04, 0xC0, 0x00, 0x10, 
  0x04, 0xC0, 0x01, 0x10, 0x04, 0xC0, 0x03, 0x10, 0x04, 0xC0, 0x03, 0x10, 
  0x04, 0xC0, 0x07, 0x10, 0x04, 0xC0, 0x0E, 0x10, 0x04, 0xC0, 0x1C, 0x10, 
  0x04, 0xC6, 0x3C, 0x10, 0x04, 0xCF, 0x78, 0x10, 0x04, 0xCE, 0x3C, 0x10, 
  0x04, 0xDC, 0x1E, 0x10, 0x04, 0xF8, 0x0E, 0x10, 0x04, 0xF0, 0x07, 0x10, 
  0x04, 0xF0, 0x03, 0x10, 0x04, 0xE0, 0x01, 0x10, 0x04, 0xE0, 0x03, 0x10, 
  0x04, 0xF0, 0x03, 0x10, 0x04, 0xF8, 0x07, 0x10, 0x04, 0xF8, 0x0E, 0x10, 
  0x04, 0xDC, 0x1C, 0x10, 0x04, 0xCE, 0x3C, 0x10, 0x04, 0xC7, 0x78, 0x10, 
  0x04, 0xC2, 0x3C, 0x10, 0x04, 0xC0, 0x1C, 0x10, 0x04, 0xC0, 0x0E, 0x10, 
  0x04, 0xC0, 0x07, 0x10, 0x04, 0xC0, 0x03, 0x10, 0x04, 0xC0, 0x03, 0x10, 
  0x04, 0xC0, 0x01, 0x10, 0x04, 0xC0, 0x00, 0x10, 0x04, 0x40, 0x00, 0x10, 
  0x04, 0x00, 0x00, 0x10, 0x04, 0x00, 0x00, 0x10, 0x04, 0x00, 0x00, 0x10, 
  0x04, 0x00, 0x00, 0x10, 0x04, 0x00, 0x00, 0x10, 0x04, 0x00, 0x00, 0x10, 
  0x04, 0x00, 0x00, 0x10, 0x04, 0x00, 0x00, 0x10, 0xFC, 0xFF, 0xFF, 0x1F, 
  0xFC, 0xFF, 0xFF, 0x1F, 0xFC, 0xFF, 0xFF, 0x1F, 0xF8, 0xFF, 0xFF, 0x0F, 
  0xE0, 0x7B, 0xFF, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
const uint8_t BLTSend_Logo_bits[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x01, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0xC0, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x0E, 0x00, 0x00, 0x06, 
  0x00, 0x00, 0xC0, 0x1C, 0x00, 0x00, 0x0E, 0x00, 0x00, 0xC0, 0x38, 0x00, 
  0x00, 0x1C, 0x00, 0x00, 0xC0, 0x70, 0x00, 0x00, 0x1C, 0x00, 0x00, 0xE0, 
  0xE0, 0x00, 0x20, 0x38, 0x00, 0x00, 0xC0, 0xC0, 0x01, 0x70, 0x30, 0x00, 
  0x08, 0xC0, 0x80, 0x03, 0x60, 0x70, 0x00, 0x1C, 0xE0, 0x00, 0x07, 0xE0, 
  0x60, 0x00, 0x3C, 0xC0, 0x80, 0x07, 0xC0, 0xE0, 0x00, 0x38, 0xC0, 0x80, 
  0x03, 0xC0, 0xE1, 0x00, 0xE0, 0xE0, 0xC0, 0x01, 0x83, 0xC1, 0x00, 0xE0, 
  0xC1, 0xF0, 0x00, 0x83, 0xC1, 0x00, 0xC0, 0xC1, 0x78, 0x00, 0x87, 0xC3, 
  0x01, 0x80, 0xE7, 0x38, 0x00, 0x86, 0x83, 0x01, 0x00, 0xCF, 0x1E, 0x00, 
  0x0E, 0x83, 0x01, 0x00, 0xDE, 0x0E, 0x00, 0x0E, 0x83, 0x01, 0x00, 0xFC, 
  0x07, 0x00, 0x0C, 0x83, 0x01, 0x00, 0xF8, 0x03, 0x00, 0x0C, 0x87, 0x01, 
  0x00, 0xF0, 0x01, 0x00, 0x0C, 0x87, 0x01, 0x00, 0xE0, 0x00, 0x00, 0x1C, 
  0x83, 0x01, 0x00, 0xF8, 0x03, 0x00, 0x0C, 0x87, 0x01, 0x00, 0xF8, 0x07, 
  0x00, 0x0C, 0x83, 0x01, 0x00, 0xDC, 0x07, 0x00, 0x0C, 0x83, 0x01, 0x00, 
  0xCF, 0x0E, 0x00, 0x0E, 0x83, 0x01, 0x80, 0xE7, 0x3C, 0x00, 0x06, 0x83, 
  0x01, 0x80, 0xC3, 0x78, 0x00, 0x87, 0xC3, 0x01, 0xC0, 0xC1, 0xF0, 0x00, 
  0x87, 0xC3, 0x01, 0xF0, 0xC0, 0xE0, 0x00, 0x83, 0xC1, 0x00, 0x78, 0xE0, 
  0xC0, 0x03, 0xC0, 0xC1, 0x00, 0x38, 0xC0, 0x80, 0x07, 0xC0, 0xE0, 0x00, 
  0x1C, 0xC0, 0x00, 0x07, 0xE0, 0x60, 0x00, 0x08, 0xC0, 0x80, 0x07, 0xE0, 
  0x70, 0x00, 0x00, 0xE0, 0xC0, 0x01, 0x70, 0x70, 0x00, 0x00, 0xC0, 0xE0, 
  0x00, 0x20, 0x38, 0x00, 0x00, 0xC0, 0x70, 0x00, 0x00, 0x38, 0x00, 0x00, 
  0xC0, 0x78, 0x00, 0x00, 0x1C, 0x00, 0x00, 0xE0, 0x1C, 0x00, 0x00, 0x0E, 
  0x00, 0x00, 0xC0, 0x0E, 0x00, 0x00, 0x06, 0x00, 0x00, 0xC0, 0x07, 0x00, 
  0x00, 0x02, 0x00, 0x00, 0xC0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 
  0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 
  0x00, 0x00,
};
const uint8_t BLT_Logo_bits[] PROGMEM = {
  0x00, 0x00, 0xC0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x07, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0xF0, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 
  0x1E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x1C, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x70, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x72, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x60, 0xE6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0xEE, 
  0x01, 0x00, 0x00, 0x00, 0x00, 0x70, 0xCE, 0x03, 0x00, 0x00, 0x00, 0x00, 
  0x60, 0x9E, 0x07, 0x00, 0x00, 0x00, 0x00, 0x70, 0x3E, 0x0F, 0x00, 0x00, 
  0x00, 0x00, 0x70, 0x7E, 0x1E, 0x00, 0x00, 0x00, 0x00, 0x60, 0xF6, 0x1C, 
  0x00, 0x00, 0x00, 0xE0, 0x61, 0xEE, 0x39, 0x00, 0x00, 0x00, 0xF0, 0x73, 
  0xC6, 0x73, 0x00, 0x00, 0x00, 0xB8, 0x77, 0xC6, 0x73, 0x00, 0x00, 0x00, 
  0x38, 0x6F, 0xE6, 0x79, 0x00, 0x00, 0x00, 0x78, 0x7E, 0xFE, 0x3C, 0x00, 
  0x00, 0x00, 0xF0, 0x7C, 0x7E, 0x1E, 0x00, 0x00, 0x00, 0xE0, 0x79, 0x3E, 
  0x0F, 0x00, 0x00, 0x00, 0xC0, 0x73, 0x8E, 0x07, 0x00, 0x00, 0x00, 0x80, 
  0x67, 0xE6, 0x03, 0x00, 0x00, 0x00, 0x00, 0x4F, 0xF2, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x1E, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x3C, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x78, 0x3E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 
  0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0xF2, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x4E, 0xE6, 0x03, 0x00, 0x00, 0x00, 0x00, 0x6F, 0xCE, 0x07, 0x00, 
  0x00, 0x00, 0x80, 0x67, 0x1E, 0x0F, 0x00, 0x00, 0x00, 0xC0, 0x73, 0x7E, 
  0x3E, 0x00, 0x00, 0x00, 0xE0, 0x79, 0xFE, 0x3C, 0x00, 0x00, 0x00, 0xF0, 
  0x7C, 0xE6, 0x71, 0x00, 0x00, 0x00, 0x70, 0x7E, 0xCE, 0x73, 0x00, 0x00, 
  0x00, 0x38, 0x6F, 0xE6, 0x39, 0x00, 0x00, 0x00, 0xB8, 0x77, 0xF6, 0x3C, 
  0x00, 0x00, 0x00, 0xF8, 0x63, 0x7E, 0x1E, 0x00, 0x00, 0x00, 0xF0, 0x61, 
  0x3E, 0x0F, 0x00, 0x00, 0x00, 0x40, 0x60, 0x9E, 0x07, 0x00, 0x00, 0x00, 
  0x00, 0x70, 0xCE, 0x03, 0x00, 0x00, 0x00, 0x00, 0x60, 0xE6, 0x01, 0x00, 
  0x00, 0x00, 0x00, 0x60, 0xF2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x78, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x60, 0x1E, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x0F, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0xE0, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x01, 0x00, 
  0x00, 0x00,
};
/* ------- ICONE DI CONNESSIONE WIFI E WPS DEPRECATE PER MOTIVI DI SPAZIO SU MICROCONTROLLORE ------
const uint8_t Wave2_Logo_bits[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0xFC, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x80, 0xFF, 0xFF, 
  0x0F, 0x00, 0x00, 0x00, 0xF0, 0xFF, 0xFF, 0x3F, 0x00, 0x00, 0x00, 0xFC, 
  0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x7F, 0xF8, 0xFF, 0x03, 0x00, 
  0x80, 0xFF, 0x01, 0x00, 0xFE, 0x07, 0x00, 0xE0, 0x7F, 0x00, 0x00, 0xF0, 
  0x1F, 0x00, 0xF0, 0x0F, 0x00, 0x00, 0xC0, 0x3F, 0x00, 0xF8, 0x07, 0x00, 
  0x00, 0x80, 0x7F, 0x00, 0xFC, 0x01, 0xF0, 0x3F, 0x00, 0xFE, 0x00, 0xFE, 
  0x00, 0xFF, 0xFF, 0x01, 0xFC, 0x01, 0x7F, 0xC0, 0xFF, 0xFF, 0x0F, 0xF8, 
  0x03, 0x1F, 0xE0, 0xFF, 0xFF, 0x1F, 0xE0, 0x03, 0x1F, 0xF8, 0x7F, 0xFA, 
  0x7F, 0xE0, 0x03, 0x0E, 0xFC, 0x07, 0x80, 0xFF, 0xC0, 0x01, 0x00, 0xFE, 
  0x01, 0x00, 0xFE, 0x01, 0x00, 0x00, 0x7F, 0x00, 0x00, 0xF8, 0x03, 0x00, 
  0x80, 0x3F, 0x00, 0x00, 0xF0, 0x07, 0x00, 0x80, 0x0F, 0xE0, 0x1F, 0xC0, 
  0x07, 0x00, 0x80, 0x07, 0xF8, 0x7F, 0x80, 0x07, 0x00, 0x00, 0x06, 0xFE, 
  0xFF, 0x81, 0x03, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x03, 0x00, 0x00, 0x00, 
  0x80, 0xFF, 0xFE, 0x07, 0x00, 0x00, 0x00, 0xC0, 0x1F, 0xE0, 0x0F, 0x00, 
  0x00, 0x00, 0xC0, 0x07, 0x80, 0x0F, 0x00, 0x00, 0x00, 0xC0, 0x03, 0x00, 
  0x0F, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 
  0x80, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x0F, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0xE0, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x1F, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0xF0, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 
  0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x1F, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0xE0, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x0F, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00,
};
const uint8_t Wave_Logo_bits[] PROGMEM = {
  0xC0, 0x0F, 0x00, 0x70, 0x38, 0x00, 0x18, 0xE1, 0x00, 0xC4, 0x9F, 0x01, 
  0x30, 0x30, 0x00, 0x98, 0x47, 0x00, 0xC0, 0x1C, 0x00, 0x20, 0x12, 0x00, 
  0x80, 0x07, 0x00, 0x80, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x07, 0x00, 
  0x00, 0x02, 0x00,
};
const uint8_t WiFi_Logo_bits[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xFF, 0x07, 0x00, 0x00, 0x00,
  0x00, 0x00, 0xE0, 0xFF, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0xFF,
  0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0xFF, 0xFF, 0x00, 0x00, 0x00,
  0x00, 0x00, 0xFE, 0xFF, 0xFF, 0x01, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF,
  0xFF, 0x03, 0x00, 0x00, 0x00, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00,
  0x00, 0xFF, 0xFF, 0xFF, 0x07, 0xC0, 0x83, 0x01, 0x80, 0xFF, 0xFF, 0xFF,
  0x01, 0x00, 0x07, 0x00, 0xC0, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x0C, 0x00,
  0xC0, 0xFF, 0xFF, 0x7C, 0x00, 0x60, 0x0C, 0x00, 0xC0, 0x31, 0x46, 0x7C,
  0xFC, 0x77, 0x08, 0x00, 0xE0, 0x23, 0xC6, 0x3C, 0xFC, 0x67, 0x18, 0x00,
  0xE0, 0x23, 0xE4, 0x3F, 0x1C, 0x00, 0x18, 0x00, 0xE0, 0x23, 0x60, 0x3C,
  0x1C, 0x70, 0x18, 0x00, 0xE0, 0x03, 0x60, 0x3C, 0x1C, 0x70, 0x18, 0x00,
  0xE0, 0x07, 0x60, 0x3C, 0xFC, 0x73, 0x18, 0x00, 0xE0, 0x87, 0x70, 0x3C,
  0xFC, 0x73, 0x18, 0x00, 0xE0, 0x87, 0x70, 0x3C, 0x1C, 0x70, 0x18, 0x00,
  0xE0, 0x87, 0x70, 0x3C, 0x1C, 0x70, 0x18, 0x00, 0xE0, 0x8F, 0x71, 0x3C,
  0x1C, 0x70, 0x18, 0x00, 0xC0, 0xFF, 0xFF, 0x3F, 0x00, 0x00, 0x08, 0x00,
  0xC0, 0xFF, 0xFF, 0x1F, 0x00, 0x00, 0x0C, 0x00, 0x80, 0xFF, 0xFF, 0x1F,
  0x00, 0x00, 0x06, 0x00, 0x80, 0xFF, 0xFF, 0x0F, 0x00, 0x00, 0x07, 0x00,
  0x00, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x00, 0xF8, 0xFF, 0xFF,
  0xFF, 0x7F, 0x00, 0x00, 0x00, 0x00, 0xFE, 0xFF, 0xFF, 0x01, 0x00, 0x00,
  0x00, 0x00, 0xFC, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0xFF,
  0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xFF, 0x1F, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x80, 0xFF, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  };
*/
