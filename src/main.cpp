#include <Arduino.h>
#include <SPI.h>
#include <FT81x.h>

void waitForKeyPress();
void dumpChipID();

void setup() {
  Serial.begin(9600);

  SPI.begin();

  waitForKeyPress();

  Serial.println("Enable display");
  FT81x::init();

  dumpChipID();

  delay(100);

  Serial.println("REG_ID");
  Serial.printf("%x\n", FT81x::read8(FT81x_REG_ID));

  Serial.printf("REG_HCYCLE %i\n", FT81x::read16(FT81x_REG_HCYCLE));
  Serial.printf("REG_HSIZE %i\n", FT81x::read16(FT81x_REG_HSIZE));

  Serial.printf("REG_VCYCLE %i\n", FT81x::read16(FT81x_REG_VCYCLE));
  Serial.printf("REG_VSIZE %i\n", FT81x::read16(FT81x_REG_VSIZE));

  FT81x::drawCircle(240, 240, 160, FT81x_COLOR_RGB(0, 150, 255));
  FT81x::swap();
}

void loop() {
}

void waitForKeyPress()
{
  Serial.println("\nPress a key to continue\n");
  while(!Serial.available());
  while(Serial.available())
  {
    Serial.read();
  }
}

void dumpChipID() {
  digitalWrite(SS, LOW);
  SPI.beginTransaction(FT81x_SPI_SETTINGS);
  SPI.transfer(0x0C);
  SPI.transfer(0x00);
  SPI.transfer(0x00);
  SPI.transfer(0x00); // dummy byte
  Serial.println("Chip ID:");
  Serial.printf("0x0C0000: %x (supposed to be 0x8)\n", SPI.transfer(0x00));
  Serial.printf("0x0C0001: %x (supposed to be 0x12)\n", SPI.transfer(0x00));
  Serial.printf("0x0C0002: %x (supposed to be 0x1)\n", SPI.transfer(0x00));
  Serial.printf("0x0C0003: %x (supposed to be 0x0)\n", SPI.transfer(0x00));
  Serial.println("");
  SPI.endTransaction();
  digitalWrite(SS, HIGH);
}